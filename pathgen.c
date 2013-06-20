#include "pathmap.h"

pathgen_node *
pathgen_node_create(pathgen_path *parent_path, pathgen_node *parent_node, int x, int y)
{
   node *n = malloc(sizeof(pathgen_node));
   n->parent_path = parent_path;
   n->parent_node = parent_node;
   n->x = x;
   n->y = y;
   n->m = x + parent_path->parent_map->w * y;
   n->f = MAX_INT
   return n;
}


Eina_Bool
pathgen_path_create(void *data)
{
   pathgen_map *parent_map = (pathgen_map *)data;

   Eina_List *l;
   void *list_data;

   /* create a new path */
   pathgen_path *path = malloc(sizeof(pathgen_path);
   path->parent_map = parent_map;
   path->start = pathgen_node_create(NULL, 0, 0);

   /* set start and end points */

   ecore_timer_add(5.0, pathgen_path_step_go, path);
   return EINA_TRUE;
}

Eina_Bool
pathgen_path_step_next(void *data)
{
   pathgen_path *parent_path = (pathgen_path *)data;
   return EINA_TRUE;
}
