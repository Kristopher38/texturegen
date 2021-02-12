#include "pixel.h"

void pixel_pwise_add(Pixel* a, const Pixel* b)
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

void pixel_pwise_mul(Pixel* a, const Pixel* b)
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

PixelHSV pixel_to_hsv(Pixel* p)
{
    PixelHSV pHsv;
    gdouble h, s, v;
    gtk_rgb_to_hsv(p->r, p->g, p->b, &h, &s, &v);
    pHsv.h = h;
    pHsv.s = s;
    pHsv.v = v;
    return pHsv;
}

Pixel pixel_to_rgb(PixelHSV* p)
{
    Pixel pRgb;
    gdouble r, g, b;
    gtk_hsv_to_rgb((gdouble)p->h, (gdouble)p->s, (gdouble)p->v, &r, &g, &b);
    pRgb.r = r;
    pRgb.g = g;
    pRgb.b = b;
    return pRgb;
}
