#ifndef PATHGEN_WORLD_H
#define PATHGEN_WORLD_H
#include <Ecore_Evas.h>
#include "pathgen_path.h"
#include "pathgen_node.h"


#define __UNUSED__
#define _pathgen_world_type "Pathgen_World"

/* events */
#define EVT_ZOOM "zoom"
#define EVT_SIM_START "sim,start"
#define EVT_SIM_FINISHED "sim,finished"
#define EVT_SIM_TRAVELER_NEW "sim,traveler,new"
#define EVT_PATH_SEARCH_COMPLETE "path,search,complete"

#define EVT_WORLD_GENERATE "world,generate"

typedef struct _Pathgen_World_Data Pathgen_World_Data;
/*
 * This structure augments clipped smart object's instance data,
 * providing extra members required by our example smart object's
 * implementation.
 */
struct _Pathgen_World_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Evas_Object *background, *height, *interest, *pathmap,
      *teleport, *heatmap, *search, *path;
   int w,h;


   /* == interface variables == */
   /* display settings */
   Eina_Bool i_display_height,
             i_display_heatmap,
             i_display_search,
             i_display_path;
   float     i_display_speed;

   /* sim */
   int path_count,
       path_fade_count,
       i_sim_path_max,
       i_sim_search_iter_max,
       i_sim_path_fade_strength,
       i_sim_path_fade_interval;

   /* world */
   float i_world_height_mult;

   /* world generation */
   int i_worldgen_w,
       i_worldgen_h,
       i_worldgen_density;

   /* path Searching */
   Eina_Bool i_path_search_diagonal;

   /* Path walk */
   Evas_Object *i_path_walk_brush;
   int   i_path_tread_weight,
         i_path_climb_up_min,
         i_path_climb_up_max,
         i_path_climb_down_min,
         i_path_climb_down_max,
         i_path_follow_min,
         i_path_follow_max;

   /* path influence variables */
   float i_path_distance_start_mult,
         i_path_distance_goal_mult;

   double (*distance_from_start)(Pathgen_Node *, Pathgen_Node *);
   double (*distance_to_goal)(Pathgen_Node *, Pathgen_Node *);

   /* == world constants that are used in calculations == */
   float c_euclid_max;
   int   c_manhat_max;

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


/************************
* World Functions *
************************/
Evas_Object *
pathgen_world_add( Evas *evas);

void
pathgen_world_size_get(Evas_Object *world, int *w, int *h);

void
pathgen_world_height_set(Evas_Object *o, Evas_Object *height);

int
pathgen_world_height_get_xy(Evas_Object *world, int x, int y);

void
pathgen_world_search_set(Evas_Object *world, Evas_Object *new);

Eina_Bool
pathgen_world_prepare(Evas_Object *world);

/************************
* Smart Callbacks *
************************/

/* world */
static void
_pathgen_world_zoom( void *data, Evas_Object *o, void *event_info );

static void
_pathgen_world_generate( void *data, Evas_Object *o, void *event_info);

/* sim */
static void
_pathgen_sim_start( void *data, Evas_Object *o, void *event_info );

static void
_pathgen_sim_traveler_new( void *data, Evas_Object *world, void *event_info );

static void
_pathgen_sim_finished( void *data, Evas_Object *o, void *event_info );

static void
_pathgen_path_search_complete( void *data, Evas_Object *world, void *event_info );


#endif /*PATHGEN_WORLD*/
