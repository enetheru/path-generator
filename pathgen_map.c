#include "pathgen_map.h"

Pathgen_Map *
pathgen_map_create(Evas_Object *world)
{
   if(!world)return NULL;
   Pathgen_Map *map = malloc(sizeof(Pathgen_Map));

   pathgen_world_size_get(world, &map->w, &map->h);

   /* create image for visual */
   map->visual = evas_object_image_filled_add(evas_object_evas_get(world));
   evas_object_image_size_set(map->visual, map->w, map->h);
   evas_object_image_smooth_scale_set(map->visual, EINA_FALSE);
   evas_object_image_alpha_set(map->visual, EINA_TRUE);
   evas_object_show(map->visual);

   /* create data */
   int *data = malloc(map->w * map->h *4);
   evas_object_image_data_set(map->visual, data);
   evas_object_image_pixels_dirty_set(map->visual, EINA_TRUE);
   int i=0;
   int number;
   for(; i < map->w * map->h; i++)
   {
      data[i] = 0x00000000;
   }
   evas_object_resize(map->visual, 100, 100);

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
