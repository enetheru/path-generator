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
   {EVT_SIM_TRAVELER_NEW, "i"},
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

   evas_object_resize(priv->heat, w, h);
   evas_object_move(priv->heat, x, y);

   evas_object_resize(priv->visual, w, h);
   evas_object_move(priv->visual, x, y);
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

   evas_object_smart_callback_add( world, "sim,traveler,new",
      (Evas_Smart_Cb) _pathgen_sim_traveler_new, NULL);

   /* get the private data struct */
   PATHGEN_WORLD_DATA_GET(world, priv);

   /* add the background object */
   priv->background = evas_object_rectangle_add(evas);
   evas_object_color_set(priv->background, 200, 200, 200, 255);
   evas_object_smart_member_add(priv->background, world);
   evas_object_lower(priv->background);

   /* set default variables */
   priv->i_path_search_iter_max = 10000;
   priv->i_path_search_iter_speed = 0.001;
   priv->i_world_travelers = 10;

   return world;
}

void
pathgen_world_info(Evas_Object *world)
{
   PATHGEN_WORLD_DATA_GET(world, priv);
   fprintf(stderr, "w: b=%p, h=%p, i=%p, p=%p, t=%p ,h=%p, v=%p, w=%i, h=%i\n",
      priv->background, priv->height, priv->interest, priv->path,
      priv->teleport, priv->heat, priv->visual, priv->w, priv->h);
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
      evas_object_del(old);
   }

   // Assign new object
   evas_object_smart_member_add(new, world);
   priv->height = new;

   evas_object_stack_above(priv->height, priv->background);

   evas_object_image_size_get(priv->height, &(priv->w), &(priv->h));
   evas_object_show(priv->height);
   
   evas_object_smart_changed(world);

   evas_object_smart_callback_call(world, EVT_HEAT_RESET, new);   
}

Evas_Object *
pathgen_world_visual_get(Evas_Object *world)
{
   PATHGEN_WORLD_DATA_GET(world, priv);
   return priv->visual;
}

Evas_Object *
pathgen_world_heat_get(Evas_Object *world)
{
   PATHGEN_WORLD_DATA_GET(world, priv);
   return priv->heat;
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

   /* preparing heat*/
   if(priv->heat)
   {
      evas_object_image_size_get(priv->heat, &w, &h);
      if(w == priv->w && h == priv->h)
         image_fill_color(priv->heat, 0x00000000);
      else
      {
         evas_object_del(priv->heat);
         priv->heat == NULL;
      }
   }
   if(!priv->heat)
   {
      priv->heat = image_generate_color(evas, priv->w, priv->h, 0x00000000);

      evas_object_smart_member_add(priv->heat, world);
      evas_object_stack_above(priv->heat, priv->height);
      evas_object_show(priv->heat);
   }

   /* preparing visual */
   if(priv->visual)
   {
      evas_object_image_size_get(priv->visual, &w, &h);
      if(w == priv->w && h == priv->h)
         image_fill_color(priv->visual, 0x00000000);
      else
      {
         evas_object_del(priv->visual);
         priv->visual == NULL;
      }
   }
   if(!priv->visual)
   {
      priv->visual = image_generate_color(evas, priv->w, priv->h,0x00000000);

      evas_object_smart_member_add(priv->visual, world);
      evas_object_stack_above(priv->visual, priv->heat);
      evas_object_show(priv->visual);
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
   k =  pixels[x+w*y] & 0x000000FF;
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

   if(evas_key_modifier_is_set(info->modifiers, "Alt"))mult=100;
   if(evas_key_modifier_is_set(info->modifiers, "Control"))
      evas_object_size_hint_min_set(o,
         (w+(w/mult)*info->z), (h+(h/mult)*info->z) );
}

/**********************
* Sim Smart Callbacks *
***********************/
static void
_pathgen_sim_start( void *data, Evas_Object *world, void *event_info )
{
   fprintf(stderr, "want to start sim\n");
   PATHGEN_WORLD_DATA_GET(world, priv);

   if(!priv->height)
   {
      fprintf(stderr, "no height map, cannot run sim.\n");
      return;
   }
   else
   {
      pathgen_world_info(world);
   }

   pathgen_world_prepare(world);

   priv->travelers=0;
   evas_object_smart_callback_call(world, EVT_SIM_TRAVELER_NEW, NULL);

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

static void 
_pathgen_sim_traveler_new( void *data, Evas_Object *world, void *event_info )
{
   Pathgen_Node *start, *end;
   Pathgen_Path *path;

   fprintf(stderr, "want new traveler\n");
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(priv->travelers >= priv->i_world_travelers)
   {
      fprintf(stderr, "max travelers reached\n");
      return;
   }
   priv->travelers++;

   /* create start and end points */
   start = pathgen_node_create(world, rand() % priv->w, rand() % priv->h);
   end = pathgen_node_create(world, rand() % priv->w, rand() % priv->h);
   image_paint_node(priv->visual, start, 0xFFFF0000);
   image_paint_node(priv->visual, end, 0xFF00FF00);

   /* new path */
   path = pathgen_path_create(world, start, end);
   path->step_count = priv->i_path_search_iter_max;
   path->step_speed = priv->i_path_search_iter_speed;

   /* walk the path */
   ecore_timer_add(path->step_speed, pathgen_path_step_next, path);
}

