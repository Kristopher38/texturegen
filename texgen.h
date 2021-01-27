#ifndef H_TEXGEN
#define H_TEXGEN
#include <math.h>
#include "noise.h"

// Texture generating functions

typedef struct Pixel
{
    float r;
    float g;
    float b;
} Pixel;

typedef struct Slider
{
    double initval;
    double min;
    double max;
    double step;
    int digits;
    const char* label;
} Slider;

typedef struct TextureFunc
{
    Pixel (*f_ptr)(int, int, double*);
    int n_sliders;
    Slider* sliders;
} TextureFunc;

Pixel texture_eval_frac(int x, int y, double* slidervals);
Pixel texture_eval_sin(int _x, int _y, double* slidervals);
Pixel texture_eval_empty(int x, int y, double* slidervals);
Pixel texture_eval_gradient(int x, int y, double* slidervals);
extern TextureFunc tex_funcs[];

#endif
