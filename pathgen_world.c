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
   PATHGEN_WORLD_DATA_GET(o, priv);
   evas_object_show(priv->background);
   if(priv->i_display_height)
   evas_object_show(priv->height);
   evas_object_show(priv->interest);
   evas_object_show(priv->path);
   evas_object_show(priv->teleport);
   if(priv->i_display_heatmap)
      evas_object_show(priv->heatmap);
   if(priv->i_display_search)
      evas_object_show(priv->search);
   if(priv->i_display_path)
      evas_object_show(priv->path);

   _pathgen_world_parent_sc->show(o);
}

static void
_pathgen_world_smart_hide(Evas_Object *o)
{
   PATHGEN_WORLD_DATA_GET(o, priv);
   evas_object_hide(priv->background);
   if(priv->i_display_height)
      evas_object_hide(priv->height);
   evas_object_hide(priv->interest);
   evas_object_hide(priv->path);
   evas_object_hide(priv->teleport);
   if(!priv->i_display_heatmap)
      evas_object_hide(priv->heatmap);
   if(!priv->i_display_search)
      evas_object_hide(priv->search);
   if(!priv->i_display_path)
      evas_object_hide(priv->path);

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

   evas_object_resize(priv->height, w, h);
   evas_object_move(priv->height, x, y);

//   evas_object_resize(priv->interest, w, h);
//   evas_object_move(priv->interest, x, y);
//   evas_object_stack_above(priv->interest, priv->height);

//   evas_object_resize(priv->path, w, h);
//   evas_object_move(priv->path, x, y);
//   evas_object_stack_above(priv->path, priv->interest);

//   evas_object_resize(priv->teleport, w, h);
//   evas_object_move(priv->teleport, x, y);
//   evas_object_stack_above(priv->teleport, priv->path);

   evas_object_resize(priv->heatmap, w, h);
   evas_object_move(priv->heatmap, x, y);

   evas_object_resize(priv->search, w, h);
   evas_object_move(priv->search, x, y);

   evas_object_resize(priv->path, w, h);
   evas_object_move(priv->path, x, y);
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

   priv->hueristic = &hueristic_dijkstra;

   /* set default variables */
   /* display */
   priv->i_display_search = I_DISPLAY_SEARCH_DEFAULT;
   priv->i_display_path = I_DISPLAY_PATH_DEFAULT;
   priv->i_display_heatmap = I_DISPLAY_HEATMAP_DEFAULT;
   /* sim */
   priv->i_sim_travelers = I_SIM_TRAVELERS_DEFAULT;
   /* world */
   priv->i_world_gen_w = I_WORLD_GEN_W_DEFAULT;
   priv->i_world_gen_h = I_WORLD_GEN_H_DEFAULT;
   priv->i_world_gen_density = I_WORLD_GEN_DENSITY_DEFAULT;
   priv->i_world_height_mult = I_WORLD_HEIGHT_MULT_DEFAULT;
   /* path */
   priv->i_path_search_iter_max = I_PATH_SEARCH_ITER_MAX_DEFAULT;
   priv->i_path_search_iter_speed = I_PATH_SEARCH_ITER_SPEED_DEFAULT;
   priv->i_path_search_diagonal = I_PATH_SEARCH_DIAGONAL_DEFAULT;
   priv->i_path_walk_strength = I_PATH_WALK_STRENGTH_DEFAULT;
   priv->i_path_walk_degrade = I_PATH_WALK_DEGRADE_DEFAULT;
   priv->i_path_walk_degrade_int = I_PATH_WALK_DEGRADE_INT_DEFAULT;

   priv->i_path_inf_dist_manhat = I_PATH_INF_DIST_MANHAT_DEFAULT;
   priv->i_path_inf_dist_euclid = I_PATH_INF_DIST_EUCLID_DEFAULT;
   priv->i_path_inf_desasc = I_PATH_INF_DESASC_DEFAULT;
   priv->i_path_inf_path = I_PATH_INF_PATH_DEFAULT;

   return world;
}

/* set to return any previous object set to the height of the
 * world or NULL, if any (or on errors) */
void
pathgen_world_height_set(Evas_Object *world, Evas_Object *new)
{
   fprintf(stderr, "setting new height map.\n");
   Evas_Object *old;

   PATHGEN_WORLD_DATA_GET(world, priv);
   old = priv->height;

   if(!new)
   {
      fprintf(stderr, "no heightmap specefied.\n");
      return;
   }

   if(old == new)
   {
      fprintf(stderr, "Maps must be unique objects\n");
      return;
   }

   if(old)
   {
      fprintf(stderr, "Deleting old heightmap.\n");
      /* delete existing height */
      evas_object_smart_member_del(old);
      evas_object_del(old);
   }

   // Assign new object
   evas_object_smart_member_add(new, world);
   priv->height = new;

   evas_object_stack_above(priv->height, priv->background);

   evas_object_image_size_get(priv->height, &(priv->w), &(priv->h));
   evas_object_show(priv->height);

   evas_object_smart_changed(world);

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

   if(!priv->height)return EINA_FALSE;

   /* preparing heatmap*/
   if(priv->heatmap)
   {
      evas_object_image_size_get(priv->heatmap, &w, &h);
      if(!(w == priv->w && h == priv->h))
      {
         evas_object_smart_member_del(priv->heatmap);
         evas_object_del(priv->heatmap);
         priv->heatmap == NULL;
      }
   }
   if(!priv->heatmap)
   {
      priv->heatmap = image_generate_color(evas, priv->w, priv->h, 0x00000000);

      evas_object_smart_member_add(priv->heatmap, world);
      evas_object_stack_above(priv->heatmap, priv->height);
      evas_object_show(priv->heatmap);
   }

   /* preparing visual */
   if(priv->search)
   {
      evas_object_image_size_get(priv->search, &w, &h);
      if(!(w == priv->w && h == priv->h))
      {
         evas_object_smart_member_del(priv->search);
         evas_object_del(priv->search);
         priv->search == NULL;
      }
   }
   if(!priv->search)
   {
      priv->search = image_generate_color(evas, priv->w, priv->h, 0x00000000);

      evas_object_smart_member_add(priv->search, world);
      evas_object_stack_above(priv->search, priv->heatmap);
      evas_object_show(priv->search);
   }

   /* preparing visual */
   if(priv->path)
   {
      evas_object_image_size_get(priv->path, &w, &h);
      if(!(w == priv->w && h == priv->h))
      {
         evas_object_smart_member_del(priv->path);
         evas_object_del(priv->path);
         priv->path == NULL;
      }
   }
   if(!priv->path)
   {
      priv->path = image_generate_color(evas, priv->w, priv->h, 0x00000000);

      evas_object_smart_member_add(priv->path, world);
      evas_object_stack_above(priv->path, priv->search);
      evas_object_show(priv->path);
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
   if(!priv->height)return 0;
   evas_object_image_size_get(priv->height, &w, &h);
   if(!(0 < x < w && 0 < y < h))return 0; 
   pixels = evas_object_image_data_get(priv->height, EINA_FALSE);
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
      priv->i_world_gen_w, priv->i_world_gen_h);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);
   image_paint_noise(image, priv->i_world_gen_density);
   image_fill_func(image, pixel_desaturate, 0);
   pathgen_world_height_set(o, image);
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

   if(!priv->height)
   {
      fprintf(stderr, "no height map, cannot run sim.\n");
      return;
   }

   ui = evas_object_name_find(evas, "sim,start");
   elm_object_disabled_set(ui, EINA_TRUE);

   ui = evas_object_name_find(evas, "world,generate");
   elm_object_disabled_set(ui, EINA_TRUE);

   ui = evas_object_name_find(evas, "world,height,load");
   elm_object_disabled_set(ui, EINA_TRUE);

   pathgen_world_prepare(world);

   priv->travelers=0;
   priv->i_path_walk_degrade_count = 0;
   evas_object_smart_callback_call(world, EVT_SIM_TRAVELER_NEW, NULL);
}

static void 
_pathgen_sim_traveler_new( void *data, Evas_Object *world, void *event_info )
{
   Pathgen_Node *start, *end;
   Pathgen_Path *path;

   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(priv->travelers >= priv->i_sim_travelers)
   {
      fprintf(stderr, "max travelers reached\n");
      evas_object_smart_callback_call(world, EVT_SIM_FINISHED, NULL);
      return;
   }
   priv->travelers++;

   if(priv->i_path_walk_degrade_count < priv->i_path_walk_degrade_int)
   {
      priv->i_path_walk_degrade_count++;
   }
   else
   {
      image_fill_func(priv->heatmap, pixel_subtract,
         (uint32_t)priv->i_path_walk_degrade<<24);
      priv->i_path_walk_degrade_count = 0;
   }


   /* create start and end points */
   start = pathgen_node_create(world, rand() % priv->w, rand() % priv->h);
   end = pathgen_node_create(world, rand() % priv->w, rand() % priv->h);
   if(priv->i_display_search)
   {
      image_paint_node(priv->search, start, 0xFFFF0000);
      image_paint_node(priv->search, end, 0xFF00FF00);
   }

   /* new path */
   path = pathgen_path_create(world, start, end);

   if(priv->i_display_search) /* walk the path slowly */
   {
      image_fill_func(priv->search, NULL, 0x00000000);
      ecore_timer_add(priv->i_path_search_iter_speed, pathgen_path_search, path);
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


   if(priv->i_display_path)
   {
      image_fill_func(priv->path, NULL, 0x00000000);
      ecore_timer_add(priv->i_path_search_iter_speed, pathgen_path_walk_slow, path);
   }
   else
   {
      image_paint_path(priv->heatmap, path, (uint32_t)(priv->i_path_walk_strength)<<24);
      evas_object_smart_callback_call(o, EVT_SIM_TRAVELER_NEW, NULL);
   }
   evas_object_smart_changed(o);
}

