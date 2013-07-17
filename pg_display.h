#ifndef PG_DISPLAY_H
#define PG_DISPLAY_H
#include <Ecore.h>
#include <Ecore_Evas.h>


#define __UNUSED__
#define _pg_display_type "PG_Display"

/* events */
#define EVT_NULL "null"


typedef struct _PG_Display_Data PG_Display_Data;
/*
 * This structure augments clipped smart object's instance data,
 * providing extra members required by our example smart object's
 * implementation.
 */
struct _PG_Display_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Evas_Object *background;
   Evas_Object *l[8];
// 7 path
// 6 search
// 5 heat
// 4 spawnmap
// 3 map
// 2 avoid
// 1 teleport
// 0 height
};

#define PG_DISPLAY_DATA_GET(o, ptr) \
  PG_Display_Data * ptr = evas_object_smart_data_get(o)

#define PG_DISPLAY_DATA_GET_OR_RETURN(o, ptr)        \
  PG_DISPLAY_DATA_GET(o, ptr);                       \
  if (!ptr)                                                  \
    {                                                        \
       fprintf(stderr, "No widget data for object %p (%s)!", \
               o, evas_object_type_get(o));                  \
       fflush(stderr);                                       \
       abort();                                              \
       return;                                               \
    }

#define PG_DISPLAY_DATA_GET_OR_RETURN_VAL(o, ptr, val) \
  PG_DISPLAY_DATA_GET(o, ptr);                         \
  if (!ptr)                                                    \
    {                                                          \
       fprintf(stderr, "No widget data for object %p (%s)!",   \
               o, evas_object_type_get(o));                    \
       fflush(stderr);                                         \
       abort();                                                \
       return val;                                             \
    }

Evas_Object *
pg_display_add( Evas *evas);

void
pg_display_layer_set(Evas_Object *disp, Evas_Object *new, int id);

#endif /*PG_DISPLAY*/
