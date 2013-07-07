#ifndef PIXEL_H
#define PIXEL_H

#include <math.h>

#define __UNUSED__

static uint32_t
pixel_add(uint32_t a, uint32_t b, float strength)
{
   uint16_t ar,ag,ab,aa,br,bg,bb,ba,cr,cg,cb,ca;
   uint32_t c;

   
   aa = (a & 0xFF000000) >> 24;
   ar = (a & 0x00FF0000) >> 16;
   ag = (a & 0x0000FF00) >> 8;
   ab = (a & 0x000000FF);

   ba = (b & 0xFF000000) >> 24;
   br = (b & 0x00FF0000) >> 16;
   bg = (b & 0x0000FF00) >> 8;
   bb = (b & 0x000000FF);

   ca = fmin(aa+ba*strength, 255);
   cr = fmin(ar+br*strength, 255);
   cg = fmin(ag+bg*strength, 255);
   cb = fmin(ab+bb*strength, 255);

   c = ca <<24 | cr << 16 | cg << 8 | cb;
   return c;
}
static uint32_t
pixel_subtract(uint32_t a, uint32_t b, float strength)
{
   uint16_t ar,ag,ab,aa,br,bg,bb,ba,cr,cg,cb,ca;
   uint32_t c;

   
   aa = (a & 0xFF000000) >> 24;
   ar = (a & 0x00FF0000) >> 16;
   ag = (a & 0x0000FF00) >> 8;
   ab = (a & 0x000000FF);

   ba = (b & 0xFF000000) >> 24;
   br = (b & 0x00FF0000) >> 16;
   bg = (b & 0x0000FF00) >> 8;
   bb = (b & 0x000000FF);

   ca = fmax(aa-ba, 0);
   cr = fmax(ar-br, 0);
   cg = fmax(ag-bg, 0);
   cb = fmax(ab-bb, 0);

   c = ca <<24 | cr << 16 | cg << 8 | cb;
   return c;
}

static uint32_t
pixel_mix(uint32_t a, uint32_t b, float strength)
{
   uint16_t ar,ag,ab,aa,br,bg,bb,ba,cr,cg,cb,ca;
   uint32_t c;
   
   aa = (a & 0xFF000000) >> 24;
   ar = (a & 0x00FF0000) >> 16;
   ag = (a & 0x0000FF00) >> 8;
   ab = (a & 0x000000FF);

   ba = (b & 0xFF000000) >> 24;
   br = (b & 0x00FF0000) >> 16;
   bg = (b & 0x0000FF00) >> 8;
   bb = (b & 0x000000FF);

   ca = aa * strength + ba * (strength * -1 + 1);
   cr = ar * strength + br * (strength * -1 + 1);
   cg = ag * strength + bg * (strength * -1 + 1);
   cb = ab * strength + bb * (strength * -1 + 1);

   c = ca <<24 | cr << 16 | cg << 8 | cb;
   fprintf(stderr, "%8X * %0.1f + %8X * %0.1f = %8X\n", a, strength, b, strength * -1 + 1,  c);
   return c;
}

static uint32_t
pixel_multiply_float(uint32_t a, float b)
{
   uint16_t ar,ag,ab,aa,cr,cg,cb,ca;
   uint32_t c;
 
   aa = (a & 0xFF000000) >> 24;
   ar = (a & 0x00FF0000) >> 16;
   ag = (a & 0x0000FF00) >> 8;
   ab = (a & 0x000000FF);

   ca = fmin(aa * b, 255);
   cr = fmin(ar * b, 255);
   cg = fmin(ag * b, 255);
   cb = fmin(ab * b, 255);

   c = ca <<24 | cr << 16 | cg << 8 | cb;
   return c;
}

static uint32_t
pixel_desaturate(uint32_t a, uint32_t b __UNUSED__, float strength)
{
   uint16_t ar,ag,ab,aa;
   uint32_t c;
 
   aa = (a & 0xFF000000) >> 24;
   ar = (a & 0x00FF0000) >> 16;
   ag = (a & 0x0000FF00) >> 8;
   ab = (a & 0x000000FF);


   c = fmin((aa + ar + ag + ab) / 4, 255);

   c = c <<24 | c << 16 | c << 8 | c;
   return c;
}

#endif /* PIXEL_H */

