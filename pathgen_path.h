#ifndef PATHGEN_PATH_H
#define PATHGEN_PATH_H
#include <Eina.h>
#include "pathgen_node.h"

typedef struct pathgen_path Pathgen_Path;
struct pathgen_path {
   Evas_Object *world;
   Pathgen_Node *start, *end, *current;
   Eina_List *open, *closed;
   int iter;
};

/*******
* Path *
********/

Pathgen_Path *
pathgen_path_create(Evas_Object *world, Pathgen_Node *start, Pathgen_Node *end);

void
pathgen_path_del(Pathgen_Path *path);

void
pathgen_path_info(Pathgen_Path *path);


/***********************
* Path Smart Callbacks *
***********************/
void
pathgen_path_search_complete( void *data, Evas_Object *world, void *event_info );

/**************
* Path Timers *
***************/

Eina_Bool
pathgen_path_search_fast(void *data);

Eina_Bool
pathgen_path_search_slow(void *data);

Eina_Bool
pathgen_path_walk(void *data);

Eina_Bool
pathgen_path_walk_slow(void *data);

/*****************
* Path Functions *
*****************/

Pathgen_Node *
pathgen_path_best(Pathgen_Path *path);


#endif /*PATHGEN_PATH_H*/
