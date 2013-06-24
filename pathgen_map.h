#ifndef PATHGEN_MAP_H
#define PATHGEN_MAP_H
#include <Eina.h>
#include <Evas.h>

typedef struct pathgen_node Pathgen_Node;
typedef struct pathgen_path Pathgen_Path;
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

void
pathgen_map_paint(Pathgen_Map *map, unsigned int x, unsigned int y,
   unsigned int color);

void
pathgen_map_paint_path(Pathgen_Map *map, Pathgen_Path *path);

#endif /*PATHGEN_MAP_H*/
