#ifndef PATHGEN_WORLD_H
#define PATHGEN_WORLD_H
#include "pathgen_map.h"
#include <Ecore_Evas.h>


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
#define EVT_SIM_TRAVELER_NEW "sim,traveler,new"

typedef struct _Pathgen_World_Data Pathgen_World_Data;
/*
 * This structure augments clipped smart object's instance data,
 * providing extra members required by our example smart object's
 * implementation.
 */
struct _Pathgen_World_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Evas_Object *background, *height, *interest, *path,
      *teleport, *heat, *visual;
   Pathgen_Map *pathmap;
   int w,h;
   int travelers;

   /* interface variables */
   int i_path_search_iter_max,
       i_world_travelers;
   float i_path_search_iter_speed;
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
pathgen_world_visual_set(Evas_Object *world, Evas_Object *new);

Evas_Object *
pathgen_world_visual_get(Evas_Object *world);

/************************
* World Smart Callbacks *
************************/
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

static void
_pathgen_sim_traveler_new( void *data, Evas_Object *world, void *event_info );


/********
* Other *
********/

Eina_Bool
pathgen_world_travel(void *data);

#endif /*PATHGEN_WORLD*/
