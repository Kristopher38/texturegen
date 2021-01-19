#include "noise.h"

#define SAMPLES 256
#define SAMPLES_MASK (SAMPLES-1)

double noise[SAMPLES];
int perm_table[SAMPLES*2];

inline double smoothstep(double t)
{
    return t * t * (3 - 2 * t);
}

inline double lerp(double lo, double hi, double t)
{
    return lo * (1-t) + hi * t;
}

inline void swap(int* a, int* b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void noise_new()
{
    for (int i = 0; i < SAMPLES; ++i)
    {
        noise[i] = (double)(rand() % 1024) / 1023.0f;
        perm_table[i] = i;
    }
    for (int i = 0; i < SAMPLES; ++i)
    {
        int r = rand() & SAMPLES_MASK;
        swap(&perm_table[i], &perm_table[r]);
        perm_table[SAMPLES+i] = perm_table[i];
    }
}

void noise_init()
{
    srand(time(NULL));
}

double noise_eval_vec2d(vec2d p)
{
    vec2i i = {.x = p.x, .y = p.y};
    vec2d t = {.x = p.x-i.x, .y = p.y-i.y};

    vec2i r0, r1;
    r0.x = i.x & SAMPLES_MASK;
    r0.y = i.y & SAMPLES_MASK;
    r1.x = (r0.x+1) & SAMPLES_MASK;
    r1.y = (r0.y+1) & SAMPLES_MASK;

    double c00, c01, c10, c11;
    c00 = noise[perm_table[perm_table[r0.x] + r0.y]];
    c10 = noise[perm_table[perm_table[r1.x] + r0.y]];
    c01 = noise[perm_table[perm_table[r0.x] + r1.y]];
    c11 = noise[perm_table[perm_table[r1.x] + r1.y]];

    vec2d s;
    s.x = smoothstep(t.x);
    s.y = smoothstep(t.y);

    double nx0 = lerp(c00, c10, s.x);
    double nx1 = lerp(c01, c11, s.x);
    return lerp(nx0, nx1, s.y);
}

double noise_eval_xy(double x, double y)
{
    return noise_eval_vec2d((vec2d){.x = x, .y = y});
}
