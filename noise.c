#include "noise.h"

double noise[SAMPLES];
int perm_table[SAMPLES*2];

inline double smootherstep(double t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
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

void noise_init(long long int seed)
{
    srand(seed != 0 ? seed : time(NULL));
}

void noise_new()
{
    // generate noise table and permutation table
    for (int i = 0; i < SAMPLES; ++i)
    {
        noise[i] = (double)(rand()) / (double)(RAND_MAX);
        perm_table[i] = i;
    }
    // permute the permutation table
    for (int i = 0; i < SAMPLES; ++i)
    {
        int r = rand() & SAMPLES_MASK;
        swap(&perm_table[i], &perm_table[r]);
        perm_table[SAMPLES+i] = perm_table[i];
    }
}

double value_noise(double x, double y)
{
    vec2i i = {.x = x, .y = y};
    vec2d t = {.x = x-i.x, .y = y-i.y};

    vec2i r0, r1;
    r0.x = i.x & SAMPLES_MASK;
    r0.y = i.y & SAMPLES_MASK;
    r1.x = (r0.x+1) & SAMPLES_MASK;
    r1.y = (r0.y+1) & SAMPLES_MASK;

    vec2d s;
    s.x = smootherstep(t.x);
    s.y = smootherstep(t.y);

    double c00, c01, c10, c11;
    c00 = noise[perm_table[perm_table[r0.x] + r0.y]];
    c10 = noise[perm_table[perm_table[r1.x] + r0.y]];
    c01 = noise[perm_table[perm_table[r0.x] + r1.y]];
    c11 = noise[perm_table[perm_table[r1.x] + r1.y]];

    double nx0 = lerp(c00, c10, s.x);
    double nx1 = lerp(c01, c11, s.x);
    return lerp(nx0, nx1, s.y);
}

double gradient_vec(int hash, double x, double y)
{
    switch (hash & 0x3)
    {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        default: return 0;
    }
}

double perlin_noise(double x, double y)
{
    vec2i i = {.x = x, .y = y};
    vec2d t = {.x = x-i.x, .y = y-i.y};

    vec2i r0, r1;
    r0.x = i.x & SAMPLES_MASK;
    r0.y = i.y & SAMPLES_MASK;
    r1.x = (r0.x+1) & SAMPLES_MASK;
    r1.y = (r0.y+1) & SAMPLES_MASK;

    vec2d s;
    s.x = smootherstep(t.x);
    s.y = smootherstep(t.y);

    int c00, c01, c10, c11;
    c00 = perm_table[perm_table[r0.x] + r0.y];
    c10 = perm_table[perm_table[r1.x] + r0.y];
    c01 = perm_table[perm_table[r0.x] + r1.y];
    c11 = perm_table[perm_table[r1.x] + r1.y];

    double nx0 = lerp(gradient_vec(c00, t.x, t.y), gradient_vec(c10, t.x-1, t.y), s.x);
    double nx1 = lerp(gradient_vec(c01, t.x, t.y-1), gradient_vec(c11, t.x-1, t.y-1), s.x);
    double res = (lerp(nx0, nx1, s.y)+1.0f)/2.0f;
    return res;
}
