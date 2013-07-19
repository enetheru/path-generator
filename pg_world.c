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
   int i;
   PG_World *world = malloc(sizeof(PG_World));
   for(i=0;i<7;i++)world->l[i] = NULL;
   world->width = 512;
   world->length = 512;
   world->height = 512;
   
   pg_world_create_nodes(world);

   return world;
}

PG_Node *
pg_world_node_get(PG_World *world, int x, int y)
{
   /* get the node from the pool */
   return world->nodes[x + world->width * y];
}

void
pg_world_create_nodes(PG_World *world)
{
   int i,j,k,x,y;
   PG_Node *node;
   /* allocate array */
   world->nodes = malloc(world->width * world->length * sizeof(void *));

   for(i=0; i < world->width; i++)
   {
      for(j=0; j < world->length; j++)
      {
         node = malloc(sizeof *node);
         world->nodes[i + world->width * j] = node;
         node->x = i;
         node->y = j;
         //FIXME, give all the nodes a z value too
      }
   } 
   for(i=0; i < world->width; i++)
   {
      for(j=0; j < world->length; j++)
      {
         for(k=0; k<8; k++)
         {
            node = world->nodes[i+world->width*j];
            /* setup new coordinates changes */
                 if(k==0){ x = i  ; y = j-1;}//north
            else if(k==1){ x = i+1; y = j  ;}//east
            else if(k==2){ x = i  ; y = j+1;}//south
            else if(k==3){ x = i-1; y = j  ;}//west
            else if(k==4){ x = i+1; y = j-1;}//north east
            else if(k==5){ x = i-1; y = j-1;}//north west
            else if(k==6){ x = i+1; y = j+1;}//south east
            else if(k==7){ x = i-1; y = j+1;}//south west
            /* skip the neighbour if it is out of bounds*/
                 if(x > world->width-1) continue;
            else if(x < 0) continue;
            else if(y > world->length-1) continue;
            else if(y < 0) continue;

            node->n[k] = world->nodes[x+world->width*y];

         }
      }
   }
}
