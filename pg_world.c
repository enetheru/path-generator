#include "pg_data.h"

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
   
   return world;
}

PG_Node *
pg_world_get_node(PG_World *world, int x, int y, int z)
{
   return NULL;
}
