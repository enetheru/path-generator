#include "pathgen_world.h"
#include "pathgen_map.h"
#include "pathgen_path.h"
#include "pathgen_node.h"
#include "misc.h"

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

EVAS_SMART_SUBCLASS_NEW(_pathgen_world_type, _pathgen_world,
   Evas_Smart_Class, Evas_Smart_Class,
   evas_object_smart_clipped_class_get, _smart_callbacks);

/***********************
* evas smart functions *
************************/

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
   _pathgen_world_parent_sc->del(o);
}

static void
_pathgen_world_smart_show(Evas_Object *o)
{
   PATHGEN_WORLD_DATA_GET(o, priv);
   evas_object_show(priv->background);
   evas_object_show(priv->height);
   evas_object_show(priv->interest);
   evas_object_show(priv->path);
   evas_object_show(priv->teleport);
   evas_object_show(priv->heat);
   evas_object_show(priv->visual);

   _pathgen_world_parent_sc->show(o);
}

static void
_pathgen_world_smart_hide(Evas_Object *o)
{
   PATHGEN_WORLD_DATA_GET(o, priv);
   evas_object_hide(priv->background);
   evas_object_hide(priv->height);
   evas_object_hide(priv->interest);
   evas_object_hide(priv->path);
   evas_object_hide(priv->teleport);
   evas_object_hide(priv->heat);
   evas_object_hide(priv->visual);

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

   evas_object_resize(priv->background, w, h);
   evas_object_move(priv->background, x, y);
   evas_object_lower(priv->background);

   evas_object_resize(priv->height, w, h);
   evas_object_move(priv->height, x, y);
   evas_object_stack_above(priv->height, priv->background);

   evas_object_resize(priv->interest, w, h);
   evas_object_move(priv->interest, x, y);
   evas_object_stack_above(priv->interest, priv->height);

   evas_object_resize(priv->path, w, h);
   evas_object_move(priv->path, x, y);
   evas_object_stack_above(priv->path, priv->interest);

   evas_object_resize(priv->teleport, w, h);
   evas_object_move(priv->teleport, x, y);
   evas_object_stack_above(priv->teleport, priv->path);

   evas_object_resize(priv->heat, w, h);
   evas_object_move(priv->heat, x, y);
   evas_object_stack_above(priv->heat, priv->teleport);

   evas_object_resize(priv->visual, w, h);
   evas_object_move(priv->visual, x, y);
   evas_object_stack_above(priv->visual, priv->heat);
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

/* add a new world to canvas */
Evas_Object *
pathgen_world_add( Evas *evas)
{
   Evas_Object * world = evas_object_smart_add(evas, _pathgen_world_smart_class_new());

   /* add callbacks */
   evas_object_smart_callback_add( world, "zoom",
      _pathgen_world_zoom, NULL);

//   evas_object_smart_callback_add( world, "heat,reset",
//      (Evas_Smart_Cb) _pathgen_world_heatmap_reset, NULL);

//   evas_object_smart_callback_add( world, "heat,clear",
//      (Evas_Smart_Cb) _pathgen_world_heatmap_clear, NULL);

   /* add callbacks to sim start/stop/reset functions */
   evas_object_smart_callback_add( world, "sim,start",
      (Evas_Smart_Cb) _pathgen_sim_start, NULL);

//   evas_object_smart_callback_add( world, "sim,stop",
//      (Evas_Smart_Cb) _pathgen_sim_stop, NULL);

//   evas_object_smart_callback_add( world, "sim,reset",
//      (Evas_Smart_Cb) _pathgen_sim_reset, NULL);

   /* get the private data struct */
   PATHGEN_WORLD_DATA_GET(world, priv);

   /* add the background object */
   priv->background = evas_object_rectangle_add(evas);
   evas_object_color_set(priv->background, 200, 200, 200, 255);
   evas_object_smart_member_add(priv->background, world);

   return world;
}

/* set to return any previous object set to the height of the
 * world or NULL, if any (or on errors) */
Evas_Object *
pathgen_world_set_height(Evas_Object *world, Evas_Object *new)
{
   fprintf(stderr, "setting new height map.\n");
   Evas_Object *old;

   PATHGEN_WORLD_DATA_GET_OR_RETURN_VAL(world, priv, NULL);
   old = priv->height;

   if(!new)
   {
      fprintf(stderr, "no heightmap specefied.\n");
      return NULL;
   }

   if(old == new)
   {
      fprintf(stderr, "Maps must be unique objects\n");
      return NULL;
   }

   if(old)
   {
      fprintf(stderr, "Deleting old heightmap.\n");
      /* delete existing height */
      evas_object_del(old);
   }

   // Assign new object
   priv->height = new;
   evas_object_image_size_get(new, &(priv->w), &(priv->h));
//   evas_object_size_hint_min_set(world, priv->w, priv->h);
   evas_object_show(new);
   
   
   evas_object_smart_member_add(new, world);
   evas_object_smart_changed(world);

   evas_object_smart_callback_call(world, EVT_HEAT_RESET, new);   
}

void
pathgen_world_size_get(Evas_Object *world, int *w, int *h)
{
   PATHGEN_WORLD_DATA_GET(world, priv);

   *w = priv->w;
   *h = priv->h;   
}

Pathgen_Map *
pathgen_world_pathmap_set(Evas_Object *world, Pathgen_Map *map)
{
   Pathgen_Map *ret;
   PATHGEN_WORLD_DATA_GET_OR_RETURN_VAL(world, priv, NULL);
   if(!map) return NULL;

   if(priv->pathmap == map)
   {
      fprintf(stderr, "Maps must be unique objects\n");
      return NULL;
   }

   if (priv->pathmap)
   {
         ret = priv->pathmap;
   }

   // Assign new object
   priv->pathmap = map;
   priv->visual = map->visual;
   int number;

   //set the map to have the world as parent
   map->parent_world = world;   
   
   evas_object_smart_member_add(map->visual, world);
   evas_object_show(map->visual);
   evas_object_smart_changed(world);

   return ret;
 
}

void *
pathgen_world_height_get(Evas_Object *world)
{
   PATHGEN_WORLD_DATA_GET(world, priv);
   void * data = evas_object_image_data_get(priv->height, EINA_FALSE);
   return data;
}

/************************
* World Smart Callbacks *
************************/

static void
_pathgen_world_zoom(
   void *data __UNUSED__,
   Evas_Object *o,
   void *event_info )
{
   int w,h, mult = -10;
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
   heat = priv->heat;
   height = priv->height;

   if(!height)
      return NULL;

   if(heat)
   {
      fprintf(stderr, "deleting old heatmap\n");
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

   priv->heat = heat;

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

/**********************
* Sim Smart Callbacks *
***********************/
static void
_pathgen_sim_start( void *data, Evas_Object *world, void *event_info )
{
   fprintf(stderr, "want to start sim\n");
   Pathgen_Map *map;
   Pathgen_Node *start, *end;
   Pathgen_Path *path;
   PATHGEN_WORLD_DATA_GET(world, priv);

   if(!priv->height)
   {
      fprintf(stderr, "cannot run sim, no height map.\n");
      return;
   }

   /* create a pathmap */
   map = pathgen_map_create(world);
   pathgen_world_pathmap_set(world, map);

   /* create start and end points */
   start = pathgen_node_create(priv->pathmap, NULL,
      rand() % priv->w, rand() % priv->h, 0);
   end = pathgen_node_create(priv->pathmap, NULL,
      rand() % priv->w, rand() % priv->h, 0);

   /* new path */
   path = pathgen_path_create(priv->pathmap, start, end);
   path->step_count = 10000; //FIXME
   path->step_speed = 0.0001;//FIXME

   /* walk the path */
   ecore_timer_add(2.0, pathgen_path_walk, path);

   return;
}

static void
_pathgen_sim_stop( void *data, Evas_Object *o, void *event_info )
{
   fprintf(stderr, "want to stop sim\n");
   return;
}

static void
_pathgen_sim_reset( void *data, Evas_Object *o, void *event_info )
{
   fprintf(stderr, "want to reset sim\n");
   return;
}

