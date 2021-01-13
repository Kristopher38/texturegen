#include "noise.h"

#define SAMPLES 256
#define SAMPLES_MASK (SAMPLES-1)

double noise[SAMPLES*SAMPLES];

inline double smoothstep(double t)
{
    return t * t * (3 - 2 * t);
}

inline double lerp(double lo, double hi, double t)
{
    return lo * (1-t) + hi * t;
}

void noise_new()
{
    for (int i = 0; i < SAMPLES*SAMPLES; ++i)
        noise[i] = (double)(rand() % 1024) / 1023.0f;
}

void noise_init()
{
    srand(time(NULL));
}

float noise_eval(vec2d p)
{
    vec2i i = {.x = p.x, .y = p.y};
    vec2d t = {.x = p.x-i.x, .y = p.y-i.y};

    vec2i r0, r1;
    r0.x = i.x & SAMPLES_MASK;
    r0.y = i.y & SAMPLES_MASK;
    r1.x = (r0.x+1) & SAMPLES_MASK;
    r1.y = (r0.y+1) & SAMPLES_MASK;

    float c00, c01, c10, c11;
    c00 = noise[r0.y * SAMPLES + r0.x];
    c10 = noise[r0.y * SAMPLES + r1.x];
    c01 = noise[r1.y * SAMPLES + r0.x];
    c11 = noise[r1.y * SAMPLES + r1.x];

    vec2d s;
    s.x = smoothstep(t.x);
    s.y = smoothstep(t.y);

    float nx0 = lerp(c00, c10, s.x);
    float nx1 = lerp(c01, c11, s.x);
    return lerp(nx0, nx1, s.y);
}
