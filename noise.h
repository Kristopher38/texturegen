#ifndef H_NOISE
#define H_NOISE

#include <stdlib.h>
#include <time.h>
#include "vec2d.h"

double smoothstep(double t);

double lerp(double lo, double hi, double t);

void swap(int* a, int* b);

void noise_init(int seed);

double noise_eval_2d(vec2d p);

double noise_eval_xy(double x, double y);

#endif
