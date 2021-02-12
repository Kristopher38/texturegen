#ifndef H_TEXGEN
#define H_TEXGEN
#include <math.h>
#include "noise.h"
#include "pixel.h"
#include "common.h"

// Texture generating functions

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
    const char* f_name;
    Pixel pref_tint;
} TextureFunc;

Pixel texture_eval_frac(int x, int y, double* slidervals);

Pixel texture_eval_sin(int _x, int _y, double* slidervals);

Pixel texture_eval_empty(int x, int y, double* slidervals);

Pixel texture_eval_gradient(int x, int y, double* slidervals);

Pixel texture_eval_grass(int x, int y, double* slidervals);

double wood(double x, double l);

double dist(double x, double y);

Pixel texture_eval_wood(int xi, int yi, double* slidervals);

extern TextureFunc tex_funcs[];
extern gint tex_funcs_n;

#endif
