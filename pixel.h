#ifndef H_PIXEL
#define H_PIXEL
#include <math.h>

typedef struct Pixel
{
    float r;
    float g;
    float b;
} Pixel;

void pixel_pwise_add(Pixel* a, Pixel* b);

void pixel_scalar_add(Pixel* p, double scalar);

void pixel_pwise_mul(Pixel* a, Pixel* b);

void pixel_scalar_mul(Pixel* p, double scalar);

void pixel_scalar_div(Pixel* p, double scalar);

void pixel_norm(Pixel* p);

#endif
