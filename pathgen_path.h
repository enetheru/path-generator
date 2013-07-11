#ifndef PATHGEN_PATH_H
#define PATHGEN_PATH_H
#include <Eina.h>
#include "pathgen_node.h"

typedef struct pathgen_path Pathgen_Path;
struct pathgen_path {
   Evas_Object *world;
   Pathgen_Node *start, *end, *current, *n[8];
   int gx, gy; // goal position
   Eina_List *open, *closed;
   int iter;
};

/*******
* Path *
********/

Pathgen_Path *
pathgen_path_create(Evas_Object *world, Pathgen_Node *start, int gx, int gy);

void
pathgen_path_del(Pathgen_Path *path);

void
pathgen_path_info(Pathgen_Path *path);

/**************
* Path Timers *
***************/

Eina_Bool
pathgen_path_search(void *data);

Eina_Bool
pathgen_path_walk(void *data);

Eina_Bool
pathgen_path_walk_slow(void *data);

/*****************
* Path Functions *
*****************/

Pathgen_Node *
pathgen_path_best(Pathgen_Path *path);

double
pathgen_path_g(Pathgen_Path *path, int x, int y);

double
pathgen_path_h(Pathgen_Path *path, int x, int y, int elev, int avoid);

#endif /*PATHGEN_PATH_H*/
