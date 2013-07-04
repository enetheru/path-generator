#ifndef NOISE_H
#define NOISE_H

#include "image.h"

static void 
image_paint_noise(Evas_Object *image, int density)
{
   /* i,j are iterators */
   int i, j;

   /* width and height */
   int  w, h;

   /* pixel x and y */
   float  px, py;

   /* nerest x and y */
   int nx, ny;

   /* bounds */
   int top, bot, lft, rht;

   /* distances */
   float interval, ratio1, ratio2;

   uint32_t pix1, pix2, pix3, tl, tr, bl, br;

   /* grid */
   uint32_t grid[density+1][density+1];

   /* interval */
   interval = 1.0 / density;

   /* pixel data */
   uint32_t *pixels;

   for(i=0; i<=density; i++)
      for(j=0; j<=density; j++)
         grid[i][j] = rand();

   evas_object_image_size_get(image ,&w, &h);

   pixels = evas_object_image_data_get(image, EINA_TRUE);


   /*nearest interpolation */
//   for(i = 0; i < w; i++) for(j = 0; j < h; j++)
//   {
//      px = (float)i / (float)w;
//      py = (float)j / (float)h;

      /* grid locations that bound the pixel */
//      nx = roundf(px / interval);
//      ny = roundf(py / interval);

      /* set color of pixel to data at grid location */
//      pixels[i + w * j] = grid[nx][ny];
//   }

   /* linear interpolation */
  for(i = 0; i < w; i++) for(j = 0; j < h; j++)
   {
      px = (float)i / (float)w;
      py = (float)j / (float)h;

      /* grid locations that bound the pixel */
      lft = floor(px / interval);
      top = floor(py / interval);
      rht = lft+1;
      bot = top+1;
      ratio1 = (px * density) - lft;
      ratio2 = (py * density) - top;

      /* interpolate x positions */
      tl = pixel_multiply_float(grid[top][lft], fabs(ratio1-1));
      tr = pixel_multiply_float(grid[top][rht], ratio1);
      bl = pixel_multiply_float(grid[bot][lft], fabs(ratio1-1));
      br = pixel_multiply_float(grid[bot][rht], ratio1);

      /* join values together */
      pix1 = pixel_add(tl,tr);
      pix2 = pixel_add(bl,br);

      /* interpalate y positions */
      pix1 = pixel_multiply_float(pix1, fabs(ratio2-1));
      pix2 = pixel_multiply_float(pix2, ratio2);

      /* join values together */
      pix3 = pixel_add(pix1,pix2);

      /* set color of pixel to data at grid location */
      pixels[i + w * j] = pixel_desaturate(pix3) | 0xFF000000;
   }

   /* pixel coordinates expressed from 0-1 */
   float u, v;
   /* vector from the top left bounding grid location to our pixel */
   float vx, vy;
   /* bounding walls of our location, top bottom left right */
   int l, r, t, b;
   /* quadratic interpolation */
   for(i = 0; i < w; i++) for(j = 0; j < h; j++)
   {
      /* get the x and y position of the pixel in UV coords */
      u = (float)i / (float)w;
      v = (float)j / (float)h;

      /* grid locations that bound the pixel */
      l = floor(u / interval);
      t = floor(v / interval);
      r = l+1;
      b = t+1;

      /* vector to the pixel from the top left bounding grid location */
      vx = (u * density) - l;
      vy = (v * density) - t;
      if(vx < 0.5)
         vx = 2 * vx * vx;
      else
         vx = (-2 * vx * vx) + (4 * vx)-1;

      if(vy < 0.5)
         vy = 2 * vy * vy;
      else
         vy = (-2 * vy * vy) + (4 * vy)-1;

      /* interpolate x positions */
      tl = pixel_multiply_float(grid[t][l], fabs(vx-1));
      tr = pixel_multiply_float(grid[t][r], vx);
      bl = pixel_multiply_float(grid[b][l], fabs(vx-1));
      br = pixel_multiply_float(grid[b][r], vx);

      /* join values together */
      pix1 = pixel_add(tl,tr);
      pix2 = pixel_add(bl,br);

      /* interpalate y positions */
      pix1 = pixel_multiply_float(pix1, fabs(vy-1));
      pix2 = pixel_multiply_float(pix2, vy);

      /* join values together */
      pix3 = pixel_add(pix1,pix2);

      /* set color of pixel to data at grid location */
      pixels[i + w * j] = pixel_desaturate(pix3) | 0xFF000000;
   }


   evas_object_image_data_set(image, pixels);
   evas_object_image_data_update_add(image, 0, 0, w, h);
}

#endif /* NOISE_H */
