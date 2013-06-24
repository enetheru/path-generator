#ifndef PATHGEN_MISC_H
#define PATHGEN_MISC_H

#include <Evas.h>

static Evas_Object *
generate_random_image(Evas *evas, int w, int h)
{
   Evas_Object *image;
   int *pixels, i, value;
   int mask = 0x000000FF;

   pixels = malloc(w*h*4);
   if(!pixels)return NULL;

   /* create default 100x100 randomly generated maze */
   image = evas_object_image_filled_add(evas);
   evas_object_image_size_set(image, w, h);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   /* generating the contents of the image */   
   for(i = 0; i < w * h; i++)
   {
      /* build random grey image */
      value = rand() & mask;
      value = value | 0xFF000000 | (value * 0x00010000) | (value * 0x00000100);
      pixels[i] = value;
   }
   evas_object_image_data_set(image, pixels);

   return image;
}

#endif /*PATHGEN_MISC_H*/
