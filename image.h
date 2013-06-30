#ifndef PATHGEN_IMAGE_H
#define PATHGEN_IMAGE_H

#include <Evas.h>
#include "image.h"
#include "pathgen_path.h"
#include "pathgen_node.h"

static Evas_Object *
image_generate_random(Evas *evas, int w, int h)
{
   Evas_Object *image;
   int *pixels, i, value;
   int mask = 0x000000FF;

   /* create default randomly generated image */
   image = evas_object_image_filled_add(evas);
   evas_object_image_size_set(image, w, h);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   pixels = evas_object_image_data_get(image, EINA_TRUE);
   /* generating the contents of the image */   
   for(i = 0; i < w * h; i++)
   {
      /* build random grey image */
      value = rand() & mask;
      value = value | 0xFF000000 | (value * 0x00010000) | (value * 0x00000100);
      pixels[i] = value;
   }
   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0, 0, w, h);

   return image;
}

static Evas_Object *
image_generate_color(Evas *evas, int w, int h, int color)
{
   Evas_Object *image;
   int *pixels, i, value;

   /* create default 100x100 randomly generated maze */
   image = evas_object_image_filled_add(evas);
   evas_object_image_size_set(image, w, h);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);
   evas_object_image_alpha_set(image, EINA_TRUE);

   pixels = evas_object_image_data_get(image, EINA_TRUE);
   /* generating the contents of the image */   
   for(i = 0; i < w * h; i++)
   {
      /* build random grey image */
      pixels[i] = color;
   }
   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0, 0, w, h);
   return image;
}

static void
image_fill_color(Evas_Object *image, int color)
{
   int i, w, h;
   int *pixels;
   if(!image)return;
   evas_object_image_size_get(image, &w, &h);
   pixels = evas_object_image_data_get(image, EINA_TRUE);
   for(i=0; i < w*h; i++)
   {
      pixels[i] = color;
   }
   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0, 0, w, h);
}

static void
image_paint_pixel(Evas_Object *image, int x, int y, int color)
{
   int w, h, *pixels;
   if(!image)return;
   evas_object_image_size_get(image, &w, &h);
   if(!(0 < x < w && 0 < y < h))return;
   pixels = evas_object_image_data_get(image, EINA_TRUE);
   pixels[x+w*y] = color;
   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0,0,w,h);
}

static void
image_paint_path(Evas_Object *image, Pathgen_Path *path, int color)
{
   while(pathgen_path_walk(path))
      image_paint_pixel(image, path->current->x, path->current->y, color);      
}

static void
image_paint_node(Evas_Object *image, Pathgen_Node *node, int color)
{
   if(!image || !node)return;
   image_paint_pixel(image, node->x, node->y, color);
}

static uint32_t
image_pixel_value_get(Evas_Object *image, Evas_Coord x, Evas_Coord y, uint32_t mask, unsigned int shift)
{
   unsigned int w, h;
   uint32_t *pixels;
   if(!image)return 0;
   evas_object_image_size_get(image, &w, &h);
   if(!(0 < x < w && 0 < y < h))return 0; 
   pixels = evas_object_image_data_get(image, EINA_FALSE);
   evas_object_image_data_set(image, pixels);
   return (pixels[x+w*y] & mask)>>shift;
}


#endif /* PATHGEN_IMAGE_H */
