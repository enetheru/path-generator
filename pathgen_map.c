#include "pathgen_map.h"
#include "pathgen_path.h"
#include "pathgen_node.h"


Pathgen_Map *
pathgen_map_create(Evas_Object *world)
{
   int *pixels, i;
   if(!world)return NULL;
   Pathgen_Map *map = malloc(sizeof(Pathgen_Map));

   pathgen_world_size_get(world, &map->w, &map->h);

   /* create image for visual */
   map->visual = evas_object_image_filled_add(evas_object_evas_get(world));
   evas_object_image_size_set(map->visual, map->w, map->h);
   evas_object_image_smooth_scale_set(map->visual, EINA_FALSE);
   evas_object_image_alpha_set(map->visual, EINA_TRUE);

   /* create data */
   pixels = malloc(map->w * map->h *4);
   evas_object_image_data_set(map->visual, pixels);
   evas_object_image_pixels_dirty_set(map->visual, EINA_TRUE);
   for(i=0; i < map->w * map->h; i++)
   {
      pixels[i] = 0x00000000;
   }
   return map;
}

Eina_Bool
_pathgen_map_del(Pathgen_Map *map)
{
   return EINA_TRUE;
}

void
pathgen_map_info(Pathgen_Map *map)
{
   fprintf(stderr,
      "==== Map Info ====\n"
      "Address\t%p\n"
      "visual\t%p\n"
      "world\t%p\n"
      "size\t%ix%i\n",
      map,
      map->visual,
      map->parent_world,
      map->w, map->h);

   return;
}

void
pathgen_map_paint(Pathgen_Map *map, unsigned int x, unsigned int y,
   unsigned int color)
{
   int *pixels;
   int mem;
   /* check that the coordinates are within bounds */
   if( !(0 < x < map->w && 0 < y < map->h) )return;

   pixels = (int *)evas_object_image_data_get(map->visual, EINA_TRUE);
   mem = x + map->w * y;
   pixels[mem] = color;
  
}

void
pathgen_map_paint_path(Pathgen_Map *map, Pathgen_Path *path)
{
   Pathgen_Node *current;
   current = path->end;
   while(current != path->start)
   {
      current = current->parent_node;
      pathgen_map_paint(map, current->x, current->y, 0xFFFF0000);
   }
   evas_object_image_pixels_dirty_set(map->visual, EINA_TRUE);
   evas_object_smart_changed(map->parent_world);
   return;
}
