#ifndef PATHGEN_MAP_H
#define PATHGEN_MAP_H
#include <Eina.h>
#include <Evas.h>

typedef struct pathgen_map Pathgen_Map;
struct pathgen_map {
   Evas_Object *parent_world;
   Evas_Object *visual;
   Evas_Object *result;
   Eina_List *paths;
   int w,h;
};

Pathgen_Map *
pathgen_map_create(Evas_Object *world);

void
pathgen_map_info(Pathgen_Map *map);

Eina_Bool
pathgen_map_del(Pathgen_Map *map);

#endif /*PATHGEN_MAP_H*/