#ifndef PATHGEN_PATH_H
#define PATHGEN_PATH_H
#include <Eina.h>
#include "pathgen_map.h"
#include "pathgen_node.h"

typedef struct pathgen_path Pathgen_Path;
struct pathgen_path {
   Evas_Object *world;
   Pathgen_Node *start, *end, *current;
   Eina_List *open, *closed;
   int step_counter, step_count;
   float step_speed;
};


Pathgen_Path *
pathgen_path_create(Evas_Object *world, Pathgen_Node *start, Pathgen_Node *end);

void
pathgen_path_del(Pathgen_Path *path);

void
pathgen_path_info(Pathgen_Path *path);

/* search for the solution */
void
pathgen_path_search(Pathgen_Path *path);

/* search iteration, -1=fail, 0=continue, 1=solved*/
int
pathgen_path_search_iter(Pathgen_Path *path);

/* search slowly for the solution painting the texture as we go */
Eina_Bool
pathgen_path_search_slow(void *data);


/* old path solving code */
Eina_Bool
pathgen_path_step_next(void *data);

Eina_Bool
pathgen_path_step_trace(void *data);

/*******************
* Solver Functions *
*******************/

Pathgen_Node *
pathgen_path_best(Pathgen_Path *path);

void
pathgen_path_neighbours(Pathgen_Path *path);

#endif /*PATHGEN_PATH_H*/
