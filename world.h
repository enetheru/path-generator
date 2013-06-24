#include <Ecore_Evas.h>
#include "pathgen.h"

#define __UNUSED__
#define _pathgen_world_type "Pathgen_World"
#define EVT_CHILDREN_NUMBER_CHANGED "children,changed"
#define EVT_ZOOM "zoom"
#define EVT_HEAT_RESET "heat,reset"
#define EVT_HEAT_CLEAR "heat,clear"
#define EVT_SIM_START "sim,start"
#define EVT_SIM_STOP "sim,stop"
#define EVT_SIM_RESET "sim,reset"
#define EVT_WORLD_GENERATE "world,generate"

#define PG_BASE 0
#define PG_HEIGHT 1
#define PG_INTEREST 2
#define PG_PATH 3
#define PG_TELEPORT 4
#define PG_HEAT 5
#define PG_VISUAL 6

static const Evas_Smart_Cb_Description _smart_callbacks[] = 
{
   {EVT_CHILDREN_NUMBER_CHANGED, "i"},
   {EVT_HEAT_RESET, "i"}, //deletes object and creates a now one
   {EVT_HEAT_CLEAR, "i"}, //zeroes data
   {EVT_ZOOM, "i"},
   {EVT_SIM_START, "i"},
   {EVT_SIM_STOP, "i"},
   {EVT_SIM_RESET, "i"},
   {EVT_WORLD_GENERATE, "i"},
   {NULL, NULL}
};

typedef struct _Pathgen_World_Data Pathgen_World_Data;
/*
 * This structure augments clipped smart object's instance data,
 * providing extra members required by our example smart object's
 * implementation.
 */
struct _Pathgen_World_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Evas_Object *children[7], *border;
   Pathgen_Map *pathmap;
   int w,h;
   int child_count;
};

#define PATHGEN_WORLD_DATA_GET(o, ptr) \
  Pathgen_World_Data * ptr = evas_object_smart_data_get(o)

#define PATHGEN_WORLD_DATA_GET_OR_RETURN(o, ptr)        \
  PATHGEN_WORLD_DATA_GET(o, ptr);                       \
  if (!ptr)                                                  \
    {                                                        \
       fprintf(stderr, "No widget data for object %p (%s)!", \
               o, evas_object_type_get(o));                  \
       fflush(stderr);                                       \
       abort();                                              \
       return;                                               \
    }

#define PATHGEN_WORLD_DATA_GET_OR_RETURN_VAL(o, ptr, val) \
  PATHGEN_WORLD_DATA_GET(o, ptr);                         \
  if (!ptr)                                                    \
    {                                                          \
       fprintf(stderr, "No widget data for object %p (%s)!",   \
               o, evas_object_type_get(o));                    \
       fflush(stderr);                                         \
       abort();                                                \
       return val;                                             \
    }

EVAS_SMART_SUBCLASS_NEW(_pathgen_world_type, _pathgen_world,
   Evas_Smart_Class, Evas_Smart_Class,
   evas_object_smart_clipped_class_get, _smart_callbacks);

/************************
* World Functions *
************************/
Evas_Object *
pathgen_world_add( Evas *evas);

void *
pathgen_world_height_get(Evas_Object *world);

void
pathgen_world_size_get(Evas_Object *world, int *w, int *h);

Evas_Object *
pathgen_world_set_height(Evas_Object *o, Evas_Object *height);

/* remove child element, return its pointer( or NULL on errors) */
Evas_Object *
pathgen_world_remove(Evas_Object *o, Evas_Object *child);

static void
_pathgen_world_remove_do(Pathgen_World_Data *priv,
   Evas_Object *child, int idx);

/************************
* World Smart Callbacks *
************************/
static void
_pathgen_world_generate( void *data, Evas_Object *world, void *event_info );

static void
_pathgen_world_zoom( void *data, Evas_Object *o, void *event_info );

static void *
_pathgen_world_heatmap_reset(
   void *event_data, Evas_Object *o, void *event_info);

static void *
_pathgen_world_heatmap_clear(
   void *event_data, Evas_Object *o, void *event_info);


/**********************
* Sim Smart Callbacks *
***********************/

static void
_pathgen_sim_start( void *data, Evas_Object *o, void *event_info );

static void
_pathgen_sim_stop( void *data, Evas_Object *o, void *event_info );

static void
_pathgen_sim_reset( void *data, Evas_Object *o, void *event_info );


/********
* Other *
********/

static void
_on_child_del(void *data,
              Evas *evas __UNUSED__,
              Evas_Object *o,
              void *einfo __UNUSED__)
{
   Evas_Object *pathgen_world = data;
   long idx;

   PATHGEN_WORLD_DATA_GET(pathgen_world, priv);

   idx = (long)evas_object_data_get(o, "index");
   idx--;

   priv->children[idx] = NULL;

   evas_object_smart_member_del(o);
   evas_object_smart_changed(pathgen_world);
}


static void
_pathgen_world_child_callbacks_unregister(Evas_Object *obj)
{
   evas_object_data_set(obj, "index", NULL);
   evas_object_event_callback_del(obj, EVAS_CALLBACK_FREE, _on_child_del);
}

static void
_pathgen_world_child_callbacks_register(Evas_Object *o,
                                             Evas_Object *child,
                                             long idx)
{
   evas_object_event_callback_add(child, EVAS_CALLBACK_FREE, _on_child_del, o);
   evas_object_data_set(child, "index", (void *)(++idx));
}

/* create and setup our pathgen_world internals*/
static void
_pathgen_world_smart_add(Evas_Object *o)
{
   EVAS_SMART_DATA_ALLOC(o, Pathgen_World_Data);
   _pathgen_world_parent_sc->add(o);
}

static void
_pathgen_world_smart_del(Evas_Object *o)
{
   PATHGEN_WORLD_DATA_GET(o, priv);

   if (priv->children[0])
     {
        _pathgen_world_child_callbacks_unregister(priv->children[0]);
        priv->children[0] = NULL;
     }

   if (priv->children[1])
     {
        _pathgen_world_child_callbacks_unregister(priv->children[1]);
        priv->children[1] = NULL;
     }

   _pathgen_world_parent_sc->del(o);
}

static void
_pathgen_world_smart_show(Evas_Object *o)
{
   PATHGEN_WORLD_DATA_GET(o, priv);

   if (priv->children[0]) evas_object_show(priv->children[0]);
   if (priv->children[1]) evas_object_show(priv->children[1]);
   evas_object_show(priv->border);

   _pathgen_world_parent_sc->show(o);
}

static void
_pathgen_world_smart_hide(Evas_Object *o)
{
   PATHGEN_WORLD_DATA_GET(o, priv);

   if (priv->children[0]) evas_object_hide(priv->children[0]);
   if (priv->children[1]) evas_object_hide(priv->children[1]);
   evas_object_hide(priv->border);

   _pathgen_world_parent_sc->hide(o);
}

static void
_pathgen_world_smart_resize(Evas_Object *o,
                                 Evas_Coord w,
                                 Evas_Coord h)
{
   Evas_Coord ow, oh;
   evas_object_geometry_get(o, NULL, NULL, &ow, &oh);
   if ((ow == w) && (oh == h)) return;

   /* this will trigger recalculation */
   evas_object_smart_changed(o);
}

/* act on child objects' properties, before rendering */
static void
_pathgen_world_smart_calculate(Evas_Object *o)
{
   Evas_Coord x, y, w, h;

   PATHGEN_WORLD_DATA_GET_OR_RETURN(o, priv);
   evas_object_geometry_get(o, &x, &y, &w, &h);

   evas_object_resize(priv->border, w, h);
   evas_object_move(priv->border, x, y);
   evas_object_lower(priv->border);

   evas_object_resize(priv->children[PG_HEIGHT], w, h);
   evas_object_move(priv->children[PG_HEIGHT], x, y);
   evas_object_stack_above(priv->children[PG_HEIGHT], priv->border);

   evas_object_resize(priv->children[PG_HEAT], w, h);
   evas_object_move(priv->children[PG_HEAT], x, y);
   evas_object_stack_above(priv->children[PG_HEAT], priv->children[PG_HEIGHT]);

   evas_object_resize(priv->children[PG_VISUAL], w, h);
   evas_object_move(priv->children[PG_VISUAL], x, y);
   evas_object_stack_above(priv->children[PG_VISUAL], priv->children[PG_HEAT]);
}

/* setting our smart interface */
static void
_pathgen_world_smart_set_user(Evas_Smart_Class *sc)
{
  /* specializing these two */
  sc->add = _pathgen_world_smart_add;
  sc->del = _pathgen_world_smart_del;
  sc->show = _pathgen_world_smart_show;
  sc->hide = _pathgen_world_smart_hide;
  /* clipped smart object has no hook on resizing or calculations */
  sc->resize = _pathgen_world_smart_resize;
  sc->calculate = _pathgen_world_smart_calculate;
}

/* BEGINS example smart object's own interface */


static void
_pathgen_world_remove_do(Pathgen_World_Data *priv,
                              Evas_Object *child,
                              int idx)
{
   priv->children[idx] = NULL;
   priv->child_count--;
   _pathgen_world_child_callbacks_unregister(child);
   evas_object_smart_member_del(child);
}


