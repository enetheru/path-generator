#include <math.h>
#include <Evas.h>
#include "r_image.h"
#include "r_pixel.h"

Evas_Object *
image_generate_color(Evas *evas, int w, int h, uint32_t color)
{
   Evas_Object *image;
   uint32_t *pixels, value;
   int i;

   image = evas_object_image_filled_add(evas);
   evas_object_image_size_set(image, w, h);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);
   evas_object_image_alpha_set(image, EINA_TRUE);

   image_func_fill(image, NULL, color);
   return image;
}

void
image_func_pixel(Evas_Object *image,
   int x, int y,
   uint32_t (*process)(uint32_t, uint32_t, float),
   uint32_t color)
{
   int w, h;
   uint32_t *pixels;

   if(!image)return;
   evas_object_image_size_get(image, &w, &h);
   if(!(0 < x < w && 0 < y < h))return;
   pixels = evas_object_image_data_get(image, EINA_TRUE);

   if(process)pixels[x+w*y] = process(pixels[x+w*y], color, 0);
   else pixels[x+w*y] = color;

   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0, 0, w, h);
}

void
image_func_fill(Evas_Object *image,
   uint32_t (*process)(uint32_t, uint32_t, float),
   uint32_t color)
{
   int i, w, h;
   uint32_t *pixels;

   if(!image)return;
   evas_object_image_size_get(image, &w, &h);
   pixels = evas_object_image_data_get(image, EINA_TRUE);
   for(i=0; i < w*h; i++)
   {
      if(process)pixels[i] = process(pixels[i], color, 0);
      else pixels[i] = color;
   }
   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0, 0, w, h);
}

void
image_func_image(Evas_Object *image_o,
   int x, int y,
   uint32_t (*process)(uint32_t, uint32_t, float),
   Evas_Object *image_b,
   float strength)
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
      pixels_o[j] = process(pixels_o[j],pixels_b[i], strength);
   }
   evas_object_image_data_set(image_o, pixels_o);
   evas_object_image_data_update_add(image_o, 0, 0, wo, ho);
}

uint32_t
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
