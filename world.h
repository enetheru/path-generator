#include <Ecore_Evas.h>
#include "pathgen.h"

#define __UNUSED__
#define _pathgen_world_type "Pathgen_World"
#define EVT_CHILDREN_NUMBER_CHANGED "children,changed"
#define EVT_ZOOM "zoom"
#define EVT_HEAT_RESET "heat,reset"
#define EVT_HEAT_CLEAR "heat,clear"

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

static void
_pathgen_world_remove_do(Pathgen_World_Data *priv,
                              Evas_Object *child,
                              int idx);

/* remove child element, return its pointer( or NULL on errors) */
Evas_Object *
pathgen_world_remove(Evas_Object *o, Evas_Object *child);

/* set to return any previous object set to the height of the
 * world or NULL, if any (or on errors) */
Evas_Object *
pathgen_world_set_height(Evas_Object *o, Evas_Object *height);

/* add a new world to canvas */
Evas_Object *
pathgen_world_add( Evas *evas);

/* get some attributes */
void
pathgen_world_size_get(Evas_Object *world, int *w, int *h);


static void
_pathgen_world_zoom(
   void *data __UNUSED__,
   Evas_Object *o,
   void *event_info )
{
   int w,h, mult = 10;
   Evas_Event_Mouse_Wheel *info = event_info;

   evas_object_geometry_get(o, NULL, NULL, &w, &h);

   if(evas_key_modifier_is_set(info->modifiers, "Alt"))mult=100;
   if(evas_key_modifier_is_set(info->modifiers, "Control"))
      evas_object_size_hint_min_set(o, (w+(w/mult)*info->z), (h+(h/mult)*info->z));
}

/* create a heat map object */
static void *
_pathgen_world_heatmap_reset(
   void *event_data __UNUSED__,
   Evas_Object *o,
   void *event_info __UNUSED__)
{
   Evas_Object *heat, *height;
   int w,h,i,j;
   void *data;
   fprintf(stderr, "heat reset callback called\n");

   PATHGEN_WORLD_DATA_GET_OR_RETURN_VAL(o, priv, NULL);
   heat = priv->children[PG_HEAT];
   height = priv->children[PG_HEIGHT];

   if(!height)
      return NULL;

   if(heat)
   {
      fprintf(stderr, "deleting old heatmap\n");
      _pathgen_world_remove_do(priv, priv->children[PG_HEAT], PG_HEAT);
      evas_object_del(heat);
   }

   /* calculate data requirements */
   evas_object_image_size_get(height, &w, &h);
   int *mem = malloc(w*h*4);
   // homogenise memory
   for(i=0; i< w*h; i++) mem[i] = 0x00000000;

   heat = evas_object_image_filled_add(evas_object_evas_get(o));
   evas_object_image_alpha_set(heat, EINA_TRUE);
   evas_object_image_size_set(heat, w, h);
   evas_object_image_data_set(heat, mem);
   evas_object_image_smooth_scale_set(heat, EINA_FALSE);
   evas_object_show(heat);

   priv->children[PG_HEAT] = heat;

   evas_object_smart_member_add(heat, o);
   evas_object_smart_changed(o);

   return heat;
}

static void *
_pathgen_world_heatmap_clear(
   void *event_data __UNUSED__,
   Evas_Object *o,
   void *event_info __UNUSED__)
{
   return;
}

static void
_clear_heat_map();

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

   priv->border = evas_object_rectangle_add(evas_object_evas_get(o));
   evas_object_color_set(priv->border, 200, 200, 200, 255);
   evas_object_smart_member_add(priv->border, o);

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

   evas_object_resize(priv->children[PG_HEIGHT], w, h);
   evas_object_move(priv->children[PG_HEIGHT], x, y);

   evas_object_resize(priv->children[PG_HEAT], w, h);
   evas_object_move(priv->children[PG_HEAT], x, y);

   evas_object_resize(priv->children[PG_VISUAL], w, h);
   evas_object_move(priv->children[PG_VISUAL], x, y);

//   if (priv->children[0])
//     {
//        evas_object_move(priv->children[0], x + 3, y + 3);
//        evas_object_resize(priv->children[0], (w / 2) - 3, (h / 2) - 3);
//     }
//
//   if (priv->children[1])
//     {
//        evas_object_move(priv->children[1], x + (w / 2), y + (h / 2));
//        evas_object_resize(priv->children[1], (w / 2) - 3, (h / 2) - 3);
//     }
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


