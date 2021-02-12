#ifndef H_PIXEL
#define H_PIXEL
#include <gtk/gtk.h>
#include <math.h>

typedef struct Pixel
{
    float r;
    float g;
    float b;
} Pixel;

typedef struct PixelHSV
{
    float h;
    float s;
    float v;
} PixelHSV;

void pixel_pwise_add(Pixel* a, Pixel* b);

void pixel_scalar_add(Pixel* p, double scalar);

void pixel_pwise_mul(Pixel* a, Pixel* b);

void pixel_scalar_mul(Pixel* p, double scalar);

void pixel_scalar_div(Pixel* p, double scalar);

void pixel_norm(Pixel* p);

PixelHSV pixel_to_hsv(Pixel* p);

Pixel pixel_to_rgb(PixelHSV* p);

#endif
