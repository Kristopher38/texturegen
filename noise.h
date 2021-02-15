#ifndef H_NOISE
#define H_NOISE
#include <stdlib.h>
#include <time.h>
#include "vec2d.h"
#include "kisstoys.h"

#define SAMPLES 256
#define SAMPLES_MASK (SAMPLES-1)

double smootherstep(double t);

double lerp(double lo, double hi, double t);

void swap(int* a, int* b);

void noise_init(long long int seed);

void noise_new();

double value_noise(double x, double y);

double gradient_vec(int hash, double x, double y);

double perlin_noise(double x, double y);

#endif
