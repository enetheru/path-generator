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

#define PG_BASE 0
#define PG_HEIGHT 1
#define PG_INTEREST 2
#define PG_PATH 3
#define PG_TELEPORT 4
#define PG_HEAT 5
#define PG_VISUAL 6


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


/********
* Other *
********/

#endif /*PATHGEN_WORLD*/
