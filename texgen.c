#include "texgen.h"

Slider texture_eval_frac_sliders[3] = {
    (Slider){.initval=0.05f, .min=0.0f, .max=1.0f, .step=0.002f, .digits=4, .label="frequency"},
    (Slider){.initval=0.5f, .min=0.0f, .max=1.0f, .step=0.01f, .digits=2, .label="gain"},
    (Slider){.initval=2.0f, .min=1.0f, .max=4.0f, .step=0.05f, .digits=2, .label="lacunarity"}
};

Pixel texture_eval_frac(int x, int y, double* slidervals) //double freq, double gain, double lacunarity)
{
    double freq = slidervals[0];
    double gain = slidervals[1];
    double lacunarity = slidervals[2];
    Pixel p;
    //int v = asin((noise_eval_xy(x*freq, y*freq)*2.0f)-1.0f)/M_PI_2 * 255.0f;
    double v = 0;
    double amp = 1;
    for (int i = 0; i < 5; ++i)
    {
        //v += noise_eval_xy(x*rate, y*freq) * amp;
        v += noise_eval_xy(x*freq, y*freq) * amp;
        //v += fabs(2*noise_eval_xy(x*freq, y*freq)-1) * amp;
        amp *= gain;
        freq *= lacunarity;
    }
    //v = (sin((x + v * 100) * 2 * M_PI / 200.f) + 1) / 2.f;
    //v *= 10;
    //v = v - (int)v;

    v = fabs(2*v-1);

    p.r = v;
    p.g = v;
    p.b = v;
    return p;
}

Slider texture_eval_sin_sliders[3] = {
    (Slider){.initval=0.05f, .min=0.0f, .max=1.0f, .step=0.002f, .digits=4, .label="frequency 1"},
    (Slider){.initval=0.5f, .min=0.0f, .max=1.0f, .step=0.01f, .digits=2, .label="gain"},
    (Slider){.initval=2.0f, .min=1.0f, .max=4.0f, .step=0.05f, .digits=2, .label="frequency 2"}
};

Pixel texture_eval_sin(int _x, int _y, double* slidervals) //double freq, double amp, double freq2)
{
    double freq = slidervals[0];
    double amp = slidervals[1];
    double freq2 = slidervals[2];
    Pixel p;
    double x = (double)_x / 255.0f;
    double y = (double)_y / 255.0f;
    //fmax(sin(10*x)*y, 0);
                      //(1+sin((x+noise_eval_xy(x * 5, y * 5)/2)*50))/2
    p.r = p.g = p.b = (1+sin((x+noise_eval_xy(x * 10 * freq, y * 10 * freq)*amp)*freq2*50))/2;//((1+sin((double)x/25.0f))/2);
    return p;
}

Pixel texture_eval_empty(int x, int y, double* slidervals) //double a, double b, double c)
{
    return (Pixel){.r=0, .g=0, .b=0};
}

Slider texture_eval_gradient_sliders[1] = {
    (Slider){.initval=0.0f, .min=0.0f, .max=1.0f, .step=0.01f, .digits=2, .label="offset"}
};

Pixel texture_eval_gradient(int x, int y, double* slidervals)
{
    double v = slidervals[0] + ((double)x)/255.0f;
    return (Pixel){.r=v, .g=v, .b=v};
}

Slider texture_eval_grass_sliders[3] = {
    (Slider){.initval=0.1f, .min=0.05f, .max=1.0f, .step=0.002f, .digits=4, .label="zoom"},
    (Slider){.initval=1.5f, .min=0.5f, .max=1.5f, .step=0.01f, .digits=2, .label="dirt amount"},
    (Slider){.initval=2.0f, .min=1.5f, .max=3.0f, .step=0.05f, .digits=2, .label="grain"}
};

Pixel texture_eval_grass(int x, int y, double* slidervals)
{
    static const Pixel green = (Pixel){.r=0.23f, .g=0.60f, .b=0.18f};
    static const Pixel brown = (Pixel){.r=0.35f, .g=0.17f, .b=0.23f};
    Pixel p1 = texture_eval_frac(x, y, slidervals);
    slidervals[0] -= 0.05f;
    Pixel p2 = texture_eval_frac(x, y, slidervals);
    slidervals[0] += 0.05f;

    pixel_scalar_mul(&p1, 0.5f);
    pixel_scalar_mul(&p2, 0.5f);
    pixel_pwise_mul(&p2, &green);
    pixel_pwise_mul(&p1, &brown);
    pixel_pwise_add(&p1, &p2);

    return p1;
}

Slider texture_eval_wood_sliders[3] = {
    (Slider){.initval=0.75f, .min=-0.5f, .max=1.5f, .step=0.01f, .digits=2, .label="center X"},
    (Slider){.initval=0.75f, .min=-0.5f, .max=1.5f, .step=0.01f, .digits=2, .label="center Y"},
    (Slider){.initval=0.0f, .min=0.0f, .max=M_2_PI, .step=0.01f, .digits=2, .label="phase shift"}
};

Pixel texture_eval_wood(int xi, int yi, double* slidervals)
{
    double x = ((double)xi) / 255.0f;
    double y = ((double)yi) / 255.0f;
    double dx = slidervals[0] - x;
    double dy = slidervals[1] - y;
    double phase = slidervals[2];
    double v = cos((sqrt(dx*dx + dy*dy)/25.0f) + phase);
    v = (v+1)/2;
    return (Pixel){.r=v, .g=v, .b=v};
}

TextureFunc tex_funcs[] = {
    (TextureFunc){.f_ptr=texture_eval_frac,
                  .n_sliders=3,
                  .sliders=texture_eval_frac_sliders,
                  .f_name="Fractal sum (value noise)",
                  .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}},
    (TextureFunc){.f_ptr=texture_eval_sin,
                  .n_sliders=3,
                  .sliders=texture_eval_sin_sliders,
                  .f_name="Sine wave",
                  .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}},
    (TextureFunc){.f_ptr=texture_eval_gradient,
                  .n_sliders=1,
                  .sliders=texture_eval_gradient_sliders,
                  .f_name="Gradient",
                  .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}},
    (TextureFunc){.f_ptr=texture_eval_grass,
                  .n_sliders=3,
                  .sliders=texture_eval_grass_sliders,
                  .f_name="Grass",
                  .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}},
    (TextureFunc){.f_ptr=texture_eval_wood,
                  .n_sliders=3,
                  .sliders=texture_eval_wood_sliders,
                  .f_name="Wood",
                  .pref_tint={.r=1.0f, .g=1.0f, .b=1.0f}},
};

size_t tex_funcs_n = sizeof(tex_funcs)/sizeof(tex_funcs[0]);
