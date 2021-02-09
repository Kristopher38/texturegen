#include "pixel.h"

void pixel_pwise_add(Pixel* a, Pixel* b)
{
    a->r += b->r;
    a->g += b->g;
    a->b += b->b;
}

void pixel_scalar_add(Pixel* p, double scalar)
{
    p->r += scalar;
    p->g += scalar;
    p->b += scalar;
}

void pixel_pwise_mul(Pixel* a, Pixel* b)
{
    a->r *= b->r;
    a->g *= b->g;
    a->b *= b->b;
}

void pixel_scalar_mul(Pixel* p, double scalar)
{
    p->r *= scalar;
    p->g *= scalar;
    p->b *= scalar;
}

void pixel_scalar_div(Pixel* p, double scalar)
{
    p->r /= scalar;
    p->g /= scalar;
    p->b /= scalar;
}

void pixel_norm(Pixel* p)
{
    float maxp = fmax(p->r, fmax(p->g, p->b));
    pixel_scalar_div(p, maxp);
}
