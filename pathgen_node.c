#include "pathgen_node.h"

Pathgen_Node *
pathgen_node_create(Pathgen_Map *parent_map, Pathgen_Node *parent_node, int x, int y, int f)
{
   Pathgen_Node *n = malloc(sizeof(Pathgen_Node));
   n->parent_map = (Pathgen_Map *)parent_map;
   n->parent_node = (Pathgen_Node *)parent_node;
   n->x = x;
   n->y = y;
   n->m = x + parent_map->w * y;
   n->f = f;
   return n;
}

void
pathgen_node_paint(Pathgen_Node *node, int color)
{
   Pathgen_Map *map = node->parent_map;
   int *data = (int *)evas_object_image_data_get(map->visual, EINA_TRUE);
   data[node->m] = color;
}

void
pathgen_node_info(Pathgen_Node *node, char *prefix)
{
#ifdef VERBOSE
   fprintf(stderr, "%sn:%p, %p, %p, (%3i,%3i), m:%6i, f:%lo\n", prefix,
      node->parent_map, node->parent_node, node,
      node->x, node->y, node->m, node->f);      
#endif
   return;
}
