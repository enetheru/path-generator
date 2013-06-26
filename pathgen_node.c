#include "pathgen_node.h"

Pathgen_Node *
pathgen_node_create(Evas_Object *world, int x, int y)
{
   Pathgen_Node *node;
   int w, h;
   if(!world)return NULL;
   pathgen_world_size_get(world, &w, &h);
   if(!(0 < x < w && 0 < y < h))return NULL; 
   node = malloc(sizeof(Pathgen_Node));
   node->world = world;
   node->parent = NULL;
   node->x = x;
   node->y = y;
   node->f = 0;
   return node;
}

void
pathgen_node_info(Pathgen_Node *node)
{
   fprintf(stderr, "n: w=%p, p=%p, (%3i,%3i), f:%li\n",
      node->world, node->parent,
      node->x, node->y, node->f);      
   return;
}
