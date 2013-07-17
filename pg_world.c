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
   evas_object_image_size_set(new, world->width, world->length);
   evas_object_show(world->l[id]);
}

PG_World *
pg_world_new()
{
   int i;
   PG_World *world = malloc(sizeof(PG_World));
   for(i=0;i<7;i++)world->l[i] = NULL;
   world->width = 0;
   world->length = 0;
   world->height = 0;
   
   return world;
}
