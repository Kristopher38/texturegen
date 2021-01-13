#ifndef H_NOISE
#define H_NOISE

#include <stdlib.h>
#include <time.h>
#include "vec2d.h"

double smoothstep(double t);

double lerp(double lo, double hi, double t);

void noise_init();

float noise_eval(vec2d p);


#endif
