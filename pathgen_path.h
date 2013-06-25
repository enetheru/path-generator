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

Eina_Bool
pathgen_path_step_next(void *data);

Eina_Bool
pathgen_path_step_trace(void *data);

Pathgen_Path *
pathgen_path_create(Evas_Object *world, Pathgen_Node *start, Pathgen_Node *end); 

Eina_Bool
pathgen_path_walk(void *data);

void
pathgen_path_info(Pathgen_Path *path);

#endif /*PATHGEN_PATH_H*/
