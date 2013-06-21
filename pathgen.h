#include <Eina.h>
#include <Evas.h>
#include <Ecore.h>
#include <limits.h>

#ifndef PG_PATHGEN_H
#define PG_PATHGEN_H

typedef struct pathgen_map Pathgen_Map;
typedef struct pathgen_path Pathgen_Path;
typedef struct pathgen_node Pathgen_Node;

struct pathgen_map {
   Evas_Object *parent_world;
   Evas_Object *visual;
   Evas_Object *result;
   int w,h;
};

struct pathgen_path {
   Pathgen_Map *parent_map;// the pathmap that this path belongs to
   Pathgen_Node *start, *end, *current;
   Eina_List *open, *closed;
   int step_counter, step_count;
   float step_speed;
};

struct pathgen_node {
   Pathgen_Map *parent_map;
   Pathgen_Node *parent_node;
   int x, y, m;
   long f;
};

Pathgen_Map *
pathgen_map_create(Evas_Object *world);

Pathgen_Node *
pathgen_node_create(Pathgen_Map *parent_map, Pathgen_Node *parent_node, int x, int y, int f);

Eina_Bool
pathgen_path_step_next(void *data);

Eina_Bool
pathgen_path_step_trace(void *data);


Pathgen_Path *
pathgen_path_create(Pathgen_Map *map, Pathgen_Node *start, Pathgen_Node *end); 

Eina_Bool
pathgen_path_walk(void *data);

Eina_Bool
pathgen_pathmap_del(Pathgen_Map *map);

static void
_pathgen_node_paint(Pathgen_Node *node, int color);

void
pathgen_map_info(Pathgen_Map *map);

void
pathgen_path_info(Pathgen_Path *path);

void
pathgen_node_info(Pathgen_Node *node, char *prefix);

#endif
