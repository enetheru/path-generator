#include <math.h>
#include <Evas.h>
#include "pathgen_node.h"

Pathgen_Node *
pathgen_node_create(Evas_Object *world, int x, int y)
{
   Pathgen_Node *node;
   int i, w, h;
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
   for(i=0; i<8; i++)node->n[i] =NULL;
   node->open = EINA_FALSE;
   node->closed = EINA_FALSE;
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

double
pathgen_node_dist_euclid(int x1, int y1, int x2, int y2)
{
   int dx, dy;
   dx = abs(x1 - x2);
   dy = abs(y1 - y2);
   return sqrt(dx * dx + dy * dy);
}

double
pathgen_node_dist_manhat(int x1, int y1, int x2, int y2)
{
   int dx, dy;
   dx = abs(x1 - x2);
   dy = abs(y1 - y2);
   return dx + dy;
}

double
pathgen_node_dist_diagon(int x1, int y1, int x2, int y2)
{
   int dx, dy;
   dx = abs(x1 - x2);
   dy = abs(y1 - y2);
   return fmax(dx, dy);
}
