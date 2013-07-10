#include "i_defaults.h"
#include "pathgen_world.h"
#include "pathgen_path.h"
#include "pathgen_node.h"
#include "r_image.h"
#include "r_noise.h"
#include "r_pixel.h"

static const Evas_Smart_Cb_Description _smart_callbacks[] = 
{
   {EVT_ZOOM, "i"},

   {EVT_SIM_START, "i"},
   {EVT_SIM_FINISHED, "i"},
   {EVT_SIM_TRAVELER_NEW, "i"},

   {EVT_PATH_SEARCH_COMPLETE, "i"},

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
   int i;
   PATHGEN_WORLD_DATA_GET(o, priv);
   evas_object_show(priv->background);
   for(i=0; i<8; i++)
      if(priv->i_display_[i])evas_object_show(priv->l[i]);

   _pathgen_world_parent_sc->show(o);
}

static void
_pathgen_world_smart_hide(Evas_Object *o)
{
   int i;
   PATHGEN_WORLD_DATA_GET(o, priv);
   evas_object_hide(priv->background);
   for(i=0; i<8; i++)
   {
      if(priv->i_display_[i])evas_object_show(priv->l[i]);
   }

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

   evas_object_resize(priv->l[0], w, h);
   evas_object_move(priv->l[0], x, y);

//   evas_object_resize(priv->interest, w, h);
//   evas_object_move(priv->interest, x, y);
//   evas_object_stack_above(priv->interest, priv->l[0]);

//   evas_object_resize(priv->l[7], w, h);
//   evas_object_move(priv->l[7], x, y);
//   evas_object_stack_above(priv->l[7], priv->interest);

//   evas_object_resize(priv->teleport, w, h);
//   evas_object_move(priv->teleport, x, y);
//   evas_object_stack_above(priv->teleport, priv->l[7]);

   evas_object_resize(priv->l[5], w, h);
   evas_object_move(priv->l[5], x, y);

   evas_object_resize(priv->l[6], w, h);
   evas_object_move(priv->l[6], x, y);

   evas_object_resize(priv->l[7], w, h);
   evas_object_move(priv->l[7], x, y);
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

/*******************
* Public Interface *
*******************/

/* add a new world to canvas */
Evas_Object *
pathgen_world_add( Evas *evas)
{
   Evas_Object * world = evas_object_smart_add(evas, _pathgen_world_smart_class_new());

   /* add callbacks */
   evas_object_smart_callback_add( world, EVT_ZOOM,
      (Evas_Smart_Cb) _pathgen_world_zoom, NULL);

   evas_object_smart_callback_add( world, EVT_SIM_START,
      (Evas_Smart_Cb) _pathgen_sim_start, NULL);

   evas_object_smart_callback_add( world, EVT_SIM_FINISHED,
      (Evas_Smart_Cb) _pathgen_sim_finished, NULL);

   evas_object_smart_callback_add( world, EVT_SIM_TRAVELER_NEW,
      (Evas_Smart_Cb) _pathgen_sim_traveler_new, NULL);

   evas_object_smart_callback_add( world, EVT_PATH_SEARCH_COMPLETE,
      (Evas_Smart_Cb) _pathgen_path_search_complete, NULL);

   evas_object_smart_callback_add( world, EVT_WORLD_GENERATE,
      (Evas_Smart_Cb) _pathgen_world_generate, NULL);

   /* get the private data struct */
   PATHGEN_WORLD_DATA_GET(world, priv);

   /* add the background object */
   priv->background = evas_object_rectangle_add(evas);
   evas_object_color_set(priv->background, 200, 200, 200, 255);
   evas_object_smart_member_add(priv->background, world);
   evas_object_lower(priv->background);

   /* add the brush object */
   priv->i_path_walk_brush = evas_object_image_add(evas);
   evas_object_image_file_set(priv->i_path_walk_brush, "5x5.png", NULL);

   /* set default variables */
   /* display */
   priv->i_display_[0] = I_DISPLAY_HEIGHT_DEFAULT;
   priv->i_display_[5] = I_DISPLAY_HEATMAP_DEFAULT;
   priv->i_display_[6] = I_DISPLAY_SEARCH_DEFAULT;
   priv->i_display_[7] = I_DISPLAY_PATH_DEFAULT;
   priv->i_display_speed = I_DISPLAY_SPEED_DEFAULT;

   /* sim */
   priv->path_count = 0;
   priv->path_fade_count = 0;
   priv->i_sim_path_max = I_SIM_PATHS_MAX_DEFAULT;
   priv->i_sim_search_iter_max = I_SIM_SEARCH_ITER_MAX_DEFAULT;
   priv->i_sim_path_fade_strength = I_SIM_PATH_FADE_STRENGTH_DEFAULT;
   priv->i_sim_path_fade_interval = I_SIM_PATH_FADE_INTERVAL_DEFAULT;

   /* world */
   priv->i_world_height_mult = I_WORLD_HEIGHT_MULT_DEFAULT;

   /* worldgen */
   priv->i_worldgen_w = I_WORLDGEN_W_DEFAULT;
   priv->i_worldgen_h = I_WORLDGEN_H_DEFAULT;
   priv->i_worldgen_density = I_WORLDGEN_DENSITY_DEFAULT;

   /* path */
   priv->i_path_search_diagonal = I_PATH_SEARCH_DIAGONAL_DEFAULT;

   priv->i_path_tread_weight = I_PATH_TREAD_WEIGHT_DEFAULT;

   priv->i_path_climb_up_tolerance = I_PATH_CLIMB_UP_TOLERANCE_DEFAULT;
   priv->i_path_climb_up_limit = I_PATH_CLIMB_UP_LIMIT_DEFAULT;
   priv->i_path_climb_up_value = I_PATH_CLIMB_UP_VALUE_DEFAULT;

   priv->i_path_climb_down_tolerance = I_PATH_CLIMB_DOWN_TOLERANCE_DEFAULT;
   priv->i_path_climb_down_limit = I_PATH_CLIMB_DOWN_LIMIT_DEFAULT;
   priv->i_path_climb_down_value = I_PATH_CLIMB_DOWN_VALUE_DEFAULT;

   priv->i_path_follow_value = I_PATH_FOLLOW_VALUE_DEFAULT;

   priv->i_path_distance_start_mult = I_PATH_DISTANCE_START_MULT_DEFAULT;
   priv->i_path_distance_goal_mult = I_PATH_DISTANCE_GOAL_MULT_DEFAULT;

   priv->distance_from_start = pathgen_node_dist_euclid;
   priv->distance_to_goal = pathgen_node_dist_euclid;

   return world;
}


Eina_Bool
pathgen_world_prepare(Evas_Object *world)
{
   int w, h;
   Evas *evas;
   fprintf(stderr, "preparing world for sim.\n");
   if(!world)return EINA_FALSE;
   PATHGEN_WORLD_DATA_GET(world, priv);
   evas = evas_object_evas_get(world);

   if(!priv->l[0])return EINA_FALSE;

   /* preparing heatmap*/
   if(priv->l[5])
   {
      evas_object_image_size_get(priv->l[5], &w, &h);
      if(!(w == priv->w && h == priv->h))
      {
         evas_object_smart_member_del(priv->l[5]);
         evas_object_del(priv->l[5]);
         priv->l[5] == NULL;
      }
   }
   if(!priv->l[5])
   {
      priv->l[5] = image_generate_color(evas, priv->w, priv->h, 0x00000000);

      evas_object_smart_member_add(priv->l[5], world);
      evas_object_stack_above(priv->l[5], priv->l[0]);
      evas_object_show(priv->l[5]);
   }

   /* preparing visual */
   if(priv->l[6])
   {
      evas_object_image_size_get(priv->l[6], &w, &h);
      if(!(w == priv->w && h == priv->h))
      {
         evas_object_smart_member_del(priv->l[6]);
         evas_object_del(priv->l[6]);
         priv->l[6] == NULL;
      }
   }
   if(!priv->l[6])
   {
      priv->l[6] = image_generate_color(evas, priv->w, priv->h, 0x00000000);

      evas_object_smart_member_add(priv->l[6], world);
      evas_object_stack_above(priv->l[6], priv->l[5]);
      evas_object_show(priv->l[6]);
   }

   /* preparing visual */
   if(priv->l[7])
   {
      evas_object_image_size_get(priv->l[7], &w, &h);
      if(!(w == priv->w && h == priv->h))
      {
         evas_object_smart_member_del(priv->l[7]);
         evas_object_del(priv->l[7]);
         priv->l[7] == NULL;
      }
   }
   if(!priv->l[7])
   {
      priv->l[7] = image_generate_color(evas, priv->w, priv->h, 0x00000000);

      evas_object_smart_member_add(priv->l[7], world);
      evas_object_stack_above(priv->l[7], priv->l[6]);
      evas_object_show(priv->l[7]);
   }

   evas_object_smart_changed(world);
   return EINA_TRUE;
}

void
pathgen_world_size_get(Evas_Object *world, int *w, int *h)
{
   PATHGEN_WORLD_DATA_GET(world, priv);

   *w = priv->w;
   *h = priv->h;   
}

int
pathgen_world_height_get_xy(Evas_Object *world, int x, int y)
{
   int w, h, k, *pixels;
   if(!world)return 0;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[0])return 0;
   evas_object_image_size_get(priv->l[0], &w, &h);
   if(!(0 < x < w && 0 < y < h))return 0; 
   pixels = evas_object_image_data_get(priv->l[0], EINA_FALSE);
   k =  (pixels[x+w*y] & 0x000000FF) * priv->i_world_height_mult;
//   fprintf(stderr, "height at (%i, %i) is %i\n", x,y,k);
   return k;
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
   if(w+h < 128)
   {
      evas_object_size_hint_min_set(o, w+1, h+1);
      return;
   }

   if(evas_key_modifier_is_set(info->modifiers, "Alt"))mult=100;
   if(evas_key_modifier_is_set(info->modifiers, "Control"))
      evas_object_size_hint_min_set(o,
         (w+(w/mult)*info->z), (h+(h/mult)*info->z) );
}

static void
_pathgen_world_generate(
   void *data,
   Evas_Object *o,
   void *event_info)
{
   Evas *evas;
   Evas_Object *image;
   PATHGEN_WORLD_DATA_GET(o, priv);
   evas = evas_object_evas_get(o);

   image = evas_object_image_filled_add(evas);
   evas_object_image_size_set(image,
      priv->i_worldgen_w, priv->i_worldgen_h);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);
   image_paint_noise(image, priv->i_worldgen_density);
   image_func_fill(image, pixel_desaturate, 0);
   pathgen_world_layer_set(o, image, 0);
}

/**********************
* Sim Smart Callbacks *
***********************/
static void
_pathgen_sim_start( void *data, Evas_Object *world, void *event_info )
{
   fprintf(stderr, "want to start sim\n");
   Evas_Object *ui;
   Evas *evas;
   Pathgen_Path *path;

   PATHGEN_WORLD_DATA_GET(world, priv);
   evas = evas_object_evas_get(world);

   if(!priv->l[0])
   {
      fprintf(stderr, "no height map, cannot run sim.\n");
      return;
   }

   pathgen_world_prepare(world);

   priv->path_count=0;
   priv->path_fade_count = 0;
   evas_object_smart_callback_call(world, EVT_SIM_TRAVELER_NEW, NULL);
}

static void 
_pathgen_sim_traveler_new( void *data, Evas_Object *world, void *event_info )
{
   Pathgen_Node *start, *goal;
   Pathgen_Path *path;

   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(priv->path_count >= priv->i_sim_path_max)
   {
      fprintf(stderr, "max travelers reached\n");
      evas_object_smart_callback_call(world, EVT_SIM_FINISHED, NULL);
      return;
   }
   priv->path_count++;

   if(priv->path_fade_count < priv->i_sim_path_fade_interval)
   {
      priv->path_fade_count++;
   }
   else
   {
      image_func_fill(priv->l[5], pixel_subtract,
         (uint32_t)priv->i_sim_path_fade_strength<<24);
      priv->path_fade_count = 0;
      evas_render(evas_object_evas_get(world));
   }


   /* create start and end points */
   start = pathgen_node_create(world, rand() % priv->w, rand() % priv->h);
   goal = pathgen_node_create(world, rand() % priv->w, rand() % priv->h);

   /* new path */
   path = pathgen_path_create(world, start, goal);

   if(priv->i_display_[6]) /* walk the path slowly */
   {
      image_func_fill(priv->l[6], NULL, 0x00000000);
      image_func_pixel(priv->l[6], goal->x, goal->y, NULL, 0xFF00FF00);
      ecore_timer_add(priv->i_display_speed, pathgen_path_search, path);
   }
   else while(pathgen_path_search(path));
}

static void
_pathgen_sim_finished(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "Finished Sim\n");
   Evas *evas;
   Evas_Object *ui;

   evas = evas_object_evas_get(o);

   ui = evas_object_name_find(evas, "sim,start");
   elm_object_disabled_set(ui, EINA_FALSE);

   ui = evas_object_name_find(evas, "world,generate");
   elm_object_disabled_set(ui, EINA_FALSE);

   ui = evas_object_name_find(evas, "world,height,load");
   elm_object_disabled_set(ui, EINA_FALSE);
}

/*****************
* Path Callbacks *
*****************/

static void
_pathgen_path_search_complete( void *data, __UNUSED__
    Evas_Object *o, void *event_info )
{
   Pathgen_Path *path = event_info;
   PATHGEN_WORLD_DATA_GET(o, priv);


   if(priv->i_display_[7])
   {
      image_func_fill(priv->l[7], NULL, 0x00000000);
      ecore_timer_add(priv->i_display_speed, pathgen_path_walk_slow, path);
   }
   else
   {
      while(pathgen_path_walk(path))
         image_func_image(priv->l[5], path->current->x, path->current->y,
            pixel_add, priv->i_path_walk_brush, priv->i_path_tread_weight);
      evas_object_smart_callback_call(o, EVT_SIM_TRAVELER_NEW, NULL);
      pathgen_path_del(path);
   }
   evas_object_smart_changed(o);
}


/* set to return any previous object set to the height of the
 * world or NULL, if any (or on errors) */
void
pathgen_world_layer_set(Evas_Object *world, Evas_Object *new, int id)
{
   int w, h;
   PATHGEN_WORLD_DATA_GET(world, priv);

   if(!new || new == priv->l[id])return;

   evas_object_image_size_get(new, &w, &h);

   if(w != priv->w || h != priv->h && id != 0)
   {
      fprintf(stderr, "ERR: for the moment the dimensions of the image must match the height");
      return;
   }

   if(priv->l[id])
   {
      evas_object_smart_member_del(priv->l[id]);
      evas_object_del(priv->l[id]);
   }

   // Assign new object
   priv->l[id] = new;
   evas_object_smart_member_add(priv->l[id], world);
   if(id == 0)
      evas_object_image_size_get(priv->l[id], &(priv->w), &(priv->h));

   evas_object_show(priv->l[id]);

   evas_object_smart_changed(world);
}
