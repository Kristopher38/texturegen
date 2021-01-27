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

TextureFunc tex_funcs[] = {
    (TextureFunc){.f_ptr=texture_eval_empty,
                  .n_sliders=0,
                  .sliders=NULL},
    (TextureFunc){.f_ptr=texture_eval_frac,
                 .n_sliders=3,
                 .sliders=texture_eval_frac_sliders},
    (TextureFunc){.f_ptr=texture_eval_sin,
                  .n_sliders=3,
                  .sliders=texture_eval_sin_sliders},
    (TextureFunc){.f_ptr=texture_eval_gradient,
                  .n_sliders=1,
                  .sliders=texture_eval_gradient_sliders}
};
