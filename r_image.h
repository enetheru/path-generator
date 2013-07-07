#ifndef IMAGE_H
#define IMAGE_H
#include <Evas.h>
#include "pathgen_path.h"
#include "pathgen_node.h"

void
image_func_image(Evas_Object *image_o,
   int x, int y,
   uint32_t (*process)(uint32_t, uint32_t),
   Evas_Object *image_b);

void
image_func_pixel(Evas_Object *image,
   int x, int y,
   uint32_t (*process)(uint32_t, uint32_t),
   uint32_t b);

Evas_Object *
image_generate_random(Evas *evas, int w, int h);

Evas_Object *
image_generate_color(Evas *evas, int w, int h, uint32_t color);

void
image_fill_func(Evas_Object *image,
   uint32_t (*process)(uint32_t, uint32_t),
   uint32_t color);

void
image_paint_pixel(Evas_Object *image, int x, int y, uint32_t color);

void
image_paint_path(Evas_Object *image, Pathgen_Path *path, uint32_t color);

void
image_paint_node(Evas_Object *image, Pathgen_Node *node, uint32_t color);

uint32_t
image_pixel_value_get(Evas_Object *image, int x, int y, uint32_t mask, int shift);

#endif /* IMAGE_H */
