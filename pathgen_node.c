#include <Evas.h>
#include "pathgen_node.h"

Pathgen_Node *
pathgen_node_create(Evas_Object *world, int x, int y)
{
   Pathgen_Node *node;
   int w, h;
   if(!world)
   {
      fprintf(stderr,
         "ERR: cannot create node, world not given\n",
         x, y);
      return NULL;
   }
   pathgen_world_size_get(world, &w, &h);
   if(!(0 < x < w && 0 < y < h))
   {
      fprintf(stderr,
         "ERR: cannot create node, (%i, %i) out of bounds\n",
         x, y);
      return NULL; 
   }
   node = malloc(sizeof(Pathgen_Node));
   node->world = world;
   node->parent = NULL;
   node->x = x;
   node->y = y;
   node->f = 0;
//   pathgen_node_info(node);
   return node;
}

/* delete node returning its parent or NULL*/
void
pathgen_node_del(Pathgen_Node *node)
{
   if(!node)return;
   free(node);
}

void
pathgen_node_info(Pathgen_Node *node)
{
   fprintf(stderr,
      "n: w=%p, p=%p, (%3i,%3i), f:%li\n",
      node->world, node->parent,
      node->x, node->y, node->f);      
   return;
}
