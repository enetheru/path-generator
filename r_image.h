#ifndef IMAGE_H
#define IMAGE_H
#include <Evas.h>

Evas_Object *
image_generate_color(Evas *evas, int w, int h, uint32_t color);

void
image_func_pixel(Evas_Object *image,
   int x, int y,
   uint32_t (*process)(uint32_t, uint32_t),
   uint32_t b);

void
image_func_fill(Evas_Object *image,
   uint32_t (*process)(uint32_t, uint32_t),
   uint32_t color);

void
image_func_image(Evas_Object *image_o,
   int x, int y,
   uint32_t (*process)(uint32_t, uint32_t),
   Evas_Object *image_b);

uint32_t
image_pixel_value_get(Evas_Object *image, int x, int y, uint32_t mask, int shift);

#endif /* IMAGE_H */
