#ifndef IMAGE_H
#define IMAGE_H
#include <math.h>
#include <Evas.h>
#include "r_pixel.h"
#include "pathgen_path.h"
#include "pathgen_node.h"


static void
image_fill_color(Evas_Object *image, uint32_t color);

static void
image_func_image(Evas_Object *image_o,
   int x, int y,
   uint32_t (*process)(uint32_t, uint32_t),
   Evas_Object *image_b);

static Evas_Object *
image_generate_random(Evas *evas, int w, int h)
{
   Evas_Object *image;
   uint32_t *pixels, mask, value;
   int i;

   mask = 0x000000FF;

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
image_generate_color(Evas *evas, int w, int h, uint32_t color)
{
   Evas_Object *image;
   uint32_t *pixels, value;
   int i;

   /* create default 100x100 randomly generated maze */
   image = evas_object_image_filled_add(evas);
   evas_object_image_size_set(image, w, h);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);
   evas_object_image_alpha_set(image, EINA_TRUE);

   image_fill_color(image, color);
   return image;
}

static void
image_fill_color(Evas_Object *image, uint32_t color)
{
   int i, w, h;
   uint32_t *pixels;

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
image_fill_function(Evas_Object *image,
   uint32_t (*process)(uint32_t, uint32_t),
   uint32_t color)
{
   int i, w, h;
   uint32_t *pixels;

   if(!image)return;
   evas_object_image_size_get(image, &w, &h);
   pixels = evas_object_image_data_get(image, EINA_TRUE);
   for(i=0; i < w*h; i++)
   {
      pixels[i] = process(pixels[i], color);
   }
   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0, 0, w, h);
}

static void
image_paint_pixel(Evas_Object *image, int x, int y, uint32_t color)
{
   int w, h;
   uint32_t *pixels;

   if(!image)return;
   evas_object_image_size_get(image, &w, &h);
   if(!(0 < x < w && 0 < y < h))return;
   pixels = evas_object_image_data_get(image, EINA_TRUE);
   pixels[x+w*y] = color;
   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0,0,w,h);
}

static void
image_add_pixel(Evas_Object *image, int x, int y, uint32_t color)
{
   int w, h;
   uint32_t  *pixels, pixel;

   if(!image)return;
   evas_object_image_size_get(image, &w, &h);
   if(!(0 < x < w && 0 < y < h))return;
   pixels = evas_object_image_data_get(image, EINA_TRUE);
   pixel = pixels[x+w*y];

   pixels[x+w*y] = pixel_add(pixel, color);

   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0,0,w,h);  
}

static void
image_paint_path(Evas_Object *image, Pathgen_Path *path, uint32_t color)
{
   Evas_Object *brush;
   brush = evas_object_image_filled_add(evas_object_evas_get(image));
   evas_object_image_file_set(brush, "5x5.png", NULL);
   while(pathgen_path_walk(path))
      image_func_image(image, path->current->x, path->current->y,
         pixel_add, brush);      
}

static void
image_paint_node(Evas_Object *image, Pathgen_Node *node, uint32_t color)
{
   if(!image || !node)return;
   image_paint_pixel(image, node->x, node->y, color);
}

static uint32_t
image_pixel_value_get(Evas_Object *image, int x, int y, uint32_t mask, int shift)
{
   int w, h;
   uint32_t *pixels;

   if(!image)return 0;
   evas_object_image_size_get(image, &w, &h);
   if(!(0 < x < w && 0 < y < h))return 0; 
   pixels = evas_object_image_data_get(image, EINA_FALSE);
   evas_object_image_data_set(image, pixels);

   return (pixels[x+w*y] & mask)>>shift;
}

static void
image_func_image(Evas_Object *image_o,
   int x, int y,
   uint32_t (*process)(uint32_t, uint32_t),
   Evas_Object *image_b)
{
   int i, j, wo, ho, wb, hb;
   uint32_t *pixels_o, *pixels_b;

   if(!image_o || !image_b)return;

   evas_object_image_size_get(image_o, &wo, &ho);
   evas_object_image_size_get(image_b, &wb, &hb);

   x = x - wb/2;
   y = y - hb/2;

   if(x+wb > wo || y+hb > ho)return;

   pixels_o = evas_object_image_data_get(image_o, EINA_TRUE);
   pixels_b = evas_object_image_data_get(image_b, EINA_FALSE);
   for(i=0; i < wb*hb; i++)
   {
      j = x + wo * ( i / hb + y) + (i % wb);
      pixels_o[j] = process(pixels_o[j],pixels_b[i]);
   }
   evas_object_image_data_set(image_o, pixels_o);
   evas_object_image_data_update_add(image_o, 0, 0, wo, ho);
}

#endif /* IMAGE_H */