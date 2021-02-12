#include "texgen.h"

Slider frac_sum_sliders[] = {
    {.initval=0.05f, .min=0.0f, .max=1.0f, .step=0.002f, .digits=4, .label="frequency"},
    {.initval=0.5f, .min=0.0f, .max=1.0f, .step=0.01f, .digits=2, .label="gain"},
    {.initval=2.0f, .min=1.0f, .max=4.0f, .step=0.05f, .digits=2, .label="lacunarity"}
};

Pixel frac_sum(int x, int y, double freq, double gain, double lacunarity, double (*noise_f)(double, double))
{
    double v = 0;
    double amp = 1;
    for (int i = 0; i < 5; ++i)
    {
        v += noise_f(x*freq, y*freq) * amp;
        amp *= gain;
        freq *= lacunarity;
    }

    double maxvalue = gain == 1 ? 5 : (pow(gain, 5) - 1)/(gain - 1);
    v = v/maxvalue;

    return (Pixel){.r=v, .g=v, .b=v};
}

Pixel texture_eval_perlin_noise(int x, int y, double* slidervals)
{
    return frac_sum(x, y, slidervals[0], slidervals[1], slidervals[2], perlin_noise);
}

Pixel texture_eval_value_noise(int x, int y, double* slidervals)
{
    return frac_sum(x, y, slidervals[0], slidervals[1], slidervals[2], value_noise);
}

double smoke(double x, double y)
{
    return fabs(2*perlin_noise(x, y)-1);
}

Pixel texture_eval_smoke(int x, int y, double* slidervals)
{
    return frac_sum(x, y, slidervals[0], slidervals[1], slidervals[2], smoke);
}

Pixel texture_eval_clouds(int x, int y, double* slidervals)
{
    PixelHSV p;
    p.h = 0.66f; // blue
    Pixel noise = texture_eval_perlin_noise(x, y, slidervals);
    pixel_scalar_div(&noise, 3.0f);
    p.s = noise.r;
    p.v = 1.0f;
    return pixel_to_rgb(&p);
}

Slider texture_eval_sin_sliders[] = {
    {.initval=0.05f, .min=0.0f, .max=1.0f, .step=0.002f, .digits=4, .label="frequency 1"},
    {.initval=0.5f, .min=0.0f, .max=1.0f, .step=0.01f, .digits=2, .label="gain"},
    {.initval=2.0f, .min=1.0f, .max=4.0f, .step=0.05f, .digits=2, .label="frequency 2"}
};

Pixel texture_eval_sin(int _x, int _y, double* slidervals) //double freq, double amp, double freq2)
{
    double freq = slidervals[0];
    double amp = slidervals[1];
    double freq2 = slidervals[2];
    Pixel p;
    double x = (double)_x / 255.0f;
    double y = (double)_y / 255.0f;
    p.r = p.g = p.b = (1+sin((x+perlin_noise(x * 10 * freq, y * 10 * freq)*amp)*freq2*50))/2;
    return p;
}

Slider texture_eval_gradient_sliders[] = {
    {.initval=0.0f, .min=0.0f, .max=1.0f, .step=0.01f, .digits=2, .label="offset"}
};

Pixel texture_eval_gradient(int x, int y, double* slidervals)
{
    double v = slidervals[0] + ((double)x)/255.0f;
    return (Pixel){.r=v, .g=v, .b=v};
}

Slider texture_eval_grass_sliders[] = {
    {.initval=0.1f, .min=0.05f, .max=1.0f, .step=0.002f, .digits=4, .label="zoom"},
    {.initval=1.5f, .min=0.5f, .max=1.5f, .step=0.01f, .digits=2, .label="dirt amount"},
    {.initval=2.0f, .min=1.5f, .max=3.0f, .step=0.05f, .digits=2, .label="grain"}
};

Pixel texture_eval_grass(int x, int y, double* slidervals)
{
    static const Pixel green = {.r=0.23f, .g=0.60f, .b=0.18f};
    static const Pixel brown = {.r=0.35f, .g=0.17f, .b=0.23f};
    Pixel p1 = texture_eval_perlin_noise(x, y, slidervals);
    slidervals[0] -= 0.05f;
    Pixel p2 = texture_eval_perlin_noise(x, y, slidervals);
    slidervals[0] += 0.05f;

    pixel_scalar_mul(&p1, 0.5f);
    pixel_scalar_mul(&p2, 0.5f);
    pixel_pwise_mul(&p2, &green);
    pixel_pwise_mul(&p1, &brown);
    pixel_pwise_add(&p1, &p2);

    return p1;
}

Slider texture_eval_wood_sliders[] = {
    {.initval=0.5f, .min=-0.5f, .max=1.5f, .step=0.01f, .digits=2, .label="center X"},
    {.initval=0.5f, .min=-0.5f, .max=1.5f, .step=0.01f, .digits=2, .label="center Y"},
    {.initval=0.0f, .min=0.0f, .max=2*M_PI, .step=0.01f, .digits=2, .label="phase shift"},
    {.initval=10.0f, .min=10.0f, .max=100.0f, .step=0.05f, .digits=2, .label="scale"},
    {.initval=0.4f, .min=0.1f, .max=10.0f, .step=0.05f, .digits=2, .label="noise perturbation"},
    {.initval=1.0f, .min=1.0f, .max=10.0f, .step=0.05f, .digits=2, .label="line thickness"},
    {.initval=1.0f, .min=0.5f, .max=10.0f, .step=0.05f, .digits=2, .label="line amplitude"},
    {.initval=0.001f, .min=0.0f, .max=0.01f, .step=0.0001f, .digits=4, .label="noise frequency"},
    {.initval=0.5f, .min=0.0f, .max=1.0f, .step=0.01f, .digits=2, .label="noise gain"},
    {.initval=2.0f, .min=1.0f, .max=4.0f, .step=0.05f, .digits=2, .label="noise lacunarity"},
};

double wood(double x, double l)
{
    return exp(l*sin(x))/exp(l);
}

double dist(double x, double y)
{
    return sqrt(x*x + y*y);
}

Pixel texture_eval_wood(int xi, int yi, double* slidervals)
{
    double x = ((double)xi) / 255.0f;
    double y = ((double)yi) / 255.0f;
    double dx = slidervals[0] - x;
    double dy = slidervals[1] - y;
    double phase = slidervals[2];
    double scale = slidervals[3];
    double perturbation = slidervals[4];
    double thickness = slidervals[5];
    double line_amp = slidervals[6];
    double frac_sliders[] = {slidervals[7], slidervals[8], slidervals[9]};

    Pixel p = texture_eval_perlin_noise(xi, yi, frac_sliders);
    double v = line_amp * wood(2*((dist(dx, dy)*scale) + phase + p.r*perturbation), thickness);
    v += 0.4f;
    return (Pixel){.r=v, .g=v, .b=v};
}

Slider texture_eval_marble_sliders[] = {
    {.initval=0.00001f, .min=0.0f, .max=0.01f, .step=0.00001f, .digits=6, .label="noise frequency"},
    {.initval=0.5f, .min=0.0f, .max=1.0f, .step=0.01f, .digits=2, .label="noise gain"},
    {.initval=2.0f, .min=1.0f, .max=4.0f, .step=0.05f, .digits=2, .label="noise lacunarity"},
    {.initval=8.0f, .min=1.0f, .max=16.0f, .step=0.1f, .digits=2, .label="zoom"},
    {.initval=5.0f, .min=1.0f, .max=20.0f, .step=0.1f, .digits=2, .label="x period"},
    {.initval=10.0f, .min=1.0f, .max=20.0f, .step=0.1f, .digits=2, .label="y period"},
    {.initval=8.0f, .min=1.0f, .max=32.0f, .step=0.1f, .digits=2, .label="turbulence power"},
};

Pixel texture_eval_marble(int x, int y, double* slidervals)
{
    Pixel noise = texture_eval_perlin_noise(x, y, slidervals);
    double freq = slidervals[3];
    double xPeriod = slidervals[4];
    double yPeriod = slidervals[5];
    double turbPower = slidervals[6];
    double xyVal = x * xPeriod * freq / 255.0f + y * yPeriod * freq / 255.0f + turbPower * noise.r;
    double v = (sin(xyVal * M_PI * 2.0f)+1.0f)/2.0f;
    return (Pixel){.r=v, .g=v, .b=v};
}

TextureFunc tex_funcs[] = {
    {
        .f_ptr=texture_eval_perlin_noise,
        .n_sliders=ARRAYLEN(frac_sum_sliders),
        .sliders=frac_sum_sliders,
        .f_name="Perlin noise (fractal sum)",
        .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}
    },
    {
        .f_ptr=texture_eval_value_noise,
        .n_sliders=ARRAYLEN(frac_sum_sliders),
        .sliders=frac_sum_sliders,
        .f_name="Value noise (fractal sum)",
        .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}
    },
    {
        .f_ptr=texture_eval_smoke,
        .n_sliders=ARRAYLEN(frac_sum_sliders),
        .sliders=frac_sum_sliders,
        .f_name="Smoke",
        .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}
    },
    {
        .f_ptr=texture_eval_sin,
        .n_sliders=ARRAYLEN(texture_eval_sin_sliders),
        .sliders=texture_eval_sin_sliders,
        .f_name="Sine wave",
        .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}
    },
    {
        .f_ptr=texture_eval_clouds,
        .n_sliders=ARRAYLEN(frac_sum_sliders),
        .sliders=frac_sum_sliders,
        .f_name="Clouds",
        .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}
    },
    {
        .f_ptr=texture_eval_gradient,
        .n_sliders=ARRAYLEN(texture_eval_gradient_sliders),
        .sliders=texture_eval_gradient_sliders,
        .f_name="Gradient",
        .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}},
    {
        .f_ptr=texture_eval_grass,
        .n_sliders=ARRAYLEN(texture_eval_grass_sliders),
        .sliders=texture_eval_grass_sliders,
        .f_name="Grass",
        .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}
    },
    {
        .f_ptr=texture_eval_wood,
        .n_sliders=ARRAYLEN(texture_eval_wood_sliders),
        .sliders=texture_eval_wood_sliders,
        .f_name="Wood",
        .pref_tint={.r=0.256667f, .g=0.154740f, .b=0.058178f}
    },
    {
        .f_ptr=texture_eval_marble,
        .n_sliders=ARRAYLEN(texture_eval_marble_sliders),
        .sliders=texture_eval_marble_sliders,
        .f_name="Marble",
        .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}
    }
};

gint tex_funcs_n = ARRAYLEN(tex_funcs);
