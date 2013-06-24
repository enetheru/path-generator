#include "world.h"
#include "pathgen_map.h"
#include "misc.h"

/* add a new world to canvas */
Evas_Object *
pathgen_world_add( Evas *evas)
{
   Evas_Object * world = evas_object_smart_add(evas, _pathgen_world_smart_class_new());

   /* add callbacks */
   evas_object_smart_callback_add( world, "zoom",
      _pathgen_world_zoom, NULL);

   evas_object_smart_callback_add( world, "heat,reset",
      (Evas_Smart_Cb) _pathgen_world_heatmap_reset, NULL);

   evas_object_smart_callback_add( world, "heat,clear",
      (Evas_Smart_Cb) _pathgen_world_heatmap_clear, NULL);

   /* add callbacks to sim start/stop/reset functions */
   evas_object_smart_callback_add( world, "sim,start",
      (Evas_Smart_Cb) _pathgen_sim_start, NULL);

   evas_object_smart_callback_add( world, "sim,stop",
      (Evas_Smart_Cb) _pathgen_sim_stop, NULL);

   evas_object_smart_callback_add( world, "sim,reset",
      (Evas_Smart_Cb) _pathgen_sim_reset, NULL);

   /* get the private data struct */
   PATHGEN_WORLD_DATA_GET(world, priv);

   /* add the background object */
   priv->border = evas_object_rectangle_add(evas);
   evas_object_color_set(priv->border, 200, 200, 200, 255);
   evas_object_smart_member_add(priv->border, world);

   return world;
}

/* remove child element, return its pointer( or NULL on errors) */
Evas_Object *
pathgen_world_remove(Evas_Object *o, Evas_Object *child)
{
   long idx;

   PATHGEN_WORLD_DATA_GET_OR_RETURN_VAL(o, priv, NULL);

   int i=0;
   while(EINA_TRUE)
   {
      if (i > 5)
      {
         fprintf(stderr, "you are trying to remove something not belonging to"
            " the example smart object!\n");
         return NULL;
      }
      if (priv->children[i] == child) break;
      i++;
   }
      
   idx = (long)evas_object_data_get(child, "index");
   idx--;

   _pathgen_world_remove_do(priv, child, idx);

   evas_object_smart_callback_call(
      o, EVT_CHILDREN_NUMBER_CHANGED, (void *)(long)priv->child_count);
   evas_object_smart_changed(o);

   return child;
}

/* set to return any previous object set to the height of the
 * world or NULL, if any (or on errors) */
Evas_Object *
pathgen_world_set_height(Evas_Object *world, Evas_Object *new)
{
   fprintf(stderr, "setting new height map.\n");
   Evas_Object *old;

   PATHGEN_WORLD_DATA_GET_OR_RETURN_VAL(world, priv, NULL);
   old = priv->children[PG_HEIGHT];

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
      _pathgen_world_remove_do(priv, old, PG_HEIGHT);
      evas_object_del(old);
   }

   // Assign new object
   priv->children[PG_HEIGHT] = new;
   evas_object_image_size_get(new, &(priv->w), &(priv->h));
   evas_object_size_hint_min_set(world, priv->w, priv->h);
   evas_object_show(new);
   
   
   _pathgen_world_child_callbacks_register(world, new, PG_HEIGHT);
   evas_object_smart_member_add(new, world);
   evas_object_smart_changed(world);

   priv->child_count++;
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
         _pathgen_world_remove_do(priv, priv->children[PG_VISUAL], PG_VISUAL);
   }

   // Assign new object
   priv->pathmap = map;
   priv->children[PG_VISUAL] = map->visual;
   int number;

   //set the map to have the world as parent
   map->parent_world = world;   
   
   _pathgen_world_child_callbacks_register(world, map->visual, PG_VISUAL);
   evas_object_smart_member_add(map->visual, world);
   evas_object_show(map->visual);
   evas_object_smart_changed(world);

   priv->child_count++;

   return ret;
 
}

void *
pathgen_world_height_get(Evas_Object *world)
{
   PATHGEN_WORLD_DATA_GET(world, priv);
   void * data = evas_object_image_data_get(priv->children[PG_HEIGHT], EINA_FALSE);
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

   if(!priv->children[PG_HEIGHT])
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

