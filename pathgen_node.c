#include <math.h>
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
   node->z = pathgen_world_height_get_xy(world, x, y);
   node->g = 0;
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
      "n: w=%p, p=%p, (%3i,%3i), f:%f\n",
      node->world, node->parent,
      node->x, node->y, node->g);      
   return;
}

float
pathgen_node_dist_euclid(Pathgen_Node *a, Pathgen_Node *b)
{
   int dx, dy;
   dx = abs(a->x - b->x);
   dy = abs(a->y - b->y);
   return sqrt(dx * dx + dy * dy);
}

int
pathgen_node_dist_manhat(Pathgen_Node *a, Pathgen_Node *b)
{
   int dx, dy;
   dx = abs(a->x - b->x);
   dy = abs(a->y - b->y);
   return dx + dy;
}
