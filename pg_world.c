#include "pg_data.h"
#include "pg_world.h"
#include "pg_node.h"

int
pg_world_height_get(PG_World *world, int x, int y)
{
   if(!world)return -1;
   if(!world->l[1])return -1;
   return image_pixel_value_get(world->l[1], x, y, 0x000000FF, 0);
}

void
pg_world_layer_set(PG_World *world, Evas_Object *new, int id)
{
   if(!new || new == world->l[id])return;

   /* delet old object */
   if(world->l[id])evas_object_del(world->l[id]);

   /* Assign new object */
   world->l[id] = new;
//   evas_object_image_size_set(new, world->width, world->length);
}

PG_World *
pg_world_new()
{
   int i, j, n;
   PG_World *w = malloc(sizeof  *w);
   for(i=0;i<7;i++)w->l[i] = NULL;
   // FIXME, take these from the user interface
   w->width = 512;
   w->length = 512;
   w->height = 512;

   /* create grid of nodes the same size as our world */
   w->nodes = malloc(w->width * w->height * sizeof *w->nodes);
   for(i=0; i < w->width; i++)
      for(j=0; j < w->length; j++)
   {
      n = i+ w->width * j;
      w->nodes[n] = malloc(sizeof **w->nodes);
      w->nodes[n]->x = i;
      w->nodes[n]->y = j;
   }
   return w;
}

PG_Node *
pg_world_node_get(PG_World *world, int x, int y)
{
   /* get the node from the pool */
   return world->nodes[x + world->width * y];
}
