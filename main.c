#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include "noise.h"

#define GETOBJ(a) gtk_builder_get_object(builder, a)
#define GET_SLIDER_VAL(a) gtk_adjustment_get_value(GTK_ADJUSTMENT(GETOBJ(a)))

GdkPixbuf* pixbuf;

typedef struct Pixel
{
    float r;
    float g;
    float b;
} Pixel;

Pixel texture_eval_frac(int x, int y, double freq, double gain, double lacunarity)
{
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

Pixel texture_eval_sin(int _x, int _y, double freq, double amp, double freq2)
{
    Pixel p;
    double x = (double)_x / 255.0f;
    double y = (double)_y / 255.0f;
    //fmax(sin(10*x)*y, 0);
                      //(1+sin((x+noise_eval_xy(x * 5, y * 5)/2)*50))/2
    p.r = p.g = p.b = (1+sin((x+noise_eval_xy(x * 10 * freq, y * 10 * freq)*amp)*freq2*50))/2;//((1+sin((double)x/25.0f))/2);
    return p;
}

Pixel texture_eval_empty(int x, int y, double a, double b, double c)
{
    return (Pixel){.r=0, .g=0, .b=0};
}

Pixel (*choose_func(GtkBuilder* builder))(int, int, double, double, double)
{
    void* funcs[] = {texture_eval_empty, texture_eval_frac, texture_eval_sin};
    GtkComboBox* func_combobox = GTK_COMBO_BOX(GETOBJ("function_combobox"));
    gint id = gtk_combo_box_get_active(func_combobox);
    assert(id >= -1 && id <= 1);
    return funcs[id+1];
}

void recalc_texture(GtkBuilder* builder)
{
    GtkImage* texture = GTK_IMAGE(GETOBJ("texture_image"));
    gdouble freq = GET_SLIDER_VAL("frequency_adjustment");
    gdouble gain = GET_SLIDER_VAL("gain_adjustment");
    gdouble lacunarity = GET_SLIDER_VAL("lacunarity_adjustment");
    guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);

    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    Pixel* tex = malloc(sizeof(Pixel)*width*height);
    float max_pixel = 0;
    Pixel (*texture_eval)(int, int, double, double, double) = choose_func(builder);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Pixel p = texture_eval(x, y, freq, gain, lacunarity);
            //Pixel p = texture_eval_sin(x, y, freq, gain, lacunarity);
            //Pixel p = texture_eval_frac2(x, y, freq, gain, lacunarity, 5);
            tex[y*width + x] = p;
            if (max_pixel < p.r)
                max_pixel = p.r;
            if (max_pixel < p.g)
                max_pixel = p.g;
            if (max_pixel < p.b)
                max_pixel = p.b;
        }
    }
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Pixel* ptex = tex + y*width + x;
            guchar* p = pixels + y*rowstride + x*n_channels;
            p[0] = fmax(0, ptex->r / max_pixel * 255.0f);
            p[1] = fmax(0, ptex->g / max_pixel * 255.0f);
            p[2] = fmax(0, ptex->b / max_pixel * 255.0f);
        }
    }

    free(tex);
    gtk_image_set_from_pixbuf(texture, pixbuf);
}



G_MODULE_EXPORT void seed_insert_text(GtkEditable* editable, char* new_text, int new_text_length, gpointer position, GtkBuilder* builder)
{
    for (int i = 0; i < new_text_length; ++i)
    {
        if (!isdigit(new_text[i]))
        {
            g_signal_stop_emission_by_name(editable, "insert-text");
            return;
        }
    }
}

int read_seed(GtkEditable* editable)
{
    gchar* seedstr = gtk_editable_get_chars(editable, 0, -1);
    int seed = atoi(seedstr);
    g_free(seedstr);
    return seed;
}

G_MODULE_EXPORT void seed_change(GtkWidget* widget, GtkBuilder* builder)
{
    noise_init(read_seed(GTK_EDITABLE(widget)));
    noise_new();
    recalc_texture(builder);
}

G_MODULE_EXPORT void function_changed(GtkWidget* widget, GtkBuilder* builder)
{
    recalc_texture(builder);
}

G_MODULE_EXPORT void lacunarity_change(GtkWidget* widget, GtkBuilder* builder)
{
    recalc_texture(builder);
}

G_MODULE_EXPORT void gain_change(GtkWidget* widget, GtkBuilder* builder)
{
    recalc_texture(builder);
}

G_MODULE_EXPORT void newnoise_click(GtkWidget* widget, GtkBuilder* builder)
{
    noise_new();
    recalc_texture(builder);
}

G_MODULE_EXPORT void save_click(GtkWidget* widget, GtkBuilder* builder)
{
    GtkFileChooser* chooser = GTK_FILE_CHOOSER(GETOBJ("save_dialog"));
    GtkComboBox* format_combo = GTK_COMBO_BOX(GETOBJ("save_format_combo"));
    gtk_file_chooser_set_current_name (chooser, "Untitled document");

    gint res = gtk_dialog_run(GTK_DIALOG(chooser));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char* filename = gtk_file_chooser_get_filename(chooser);
        char* format = gtk_combo_box_get_active_id(format_combo);
        GError* save_error = NULL;
        gboolean save_ok = gdk_pixbuf_save(pixbuf, filename, format, &save_error, NULL);

        if (save_ok == FALSE)
        {
            g_printerr("%s", save_error->message);
            GtkMessageDialog* dialog = GTK_MESSAGE_DIALOG(GETOBJ("save_error_dialog"));
            gtk_message_dialog_set_markup(dialog, save_error->message);
            gtk_dialog_run(dialog);
        }
        g_free(filename);
    }
    gtk_widget_hide(GTK_WIDGET(chooser));
}

G_MODULE_EXPORT void frequency_change(GtkWidget* widget, GtkBuilder* builder)
{
    recalc_texture(builder);
}

G_MODULE_EXPORT void window_show(GtkWidget* widget, GtkBuilder* builder)
{
    // init texture display
    GtkImage* texture = GTK_IMAGE(GETOBJ("texture_image"));
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, 256, 256);
    noise_init(read_seed(GETOBJ("seed_entry")));
    noise_new();
    recalc_texture(builder);

    // workaround for setting save dialog action
    GtkFileChooser* chooser = GTK_FILE_CHOOSER(GETOBJ("save_dialog"));
    gtk_file_chooser_set_action(chooser, GTK_FILE_CHOOSER_ACTION_SAVE);

    // workaround for setting image of save error dialog
    GtkImage* save_error_img = GTK_IMAGE(GETOBJ("save_error_image"));
    GtkMessageDialog* save_error_dialog = GTK_MESSAGE_DIALOG(GETOBJ("save_error_dialog"));
    gtk_message_dialog_set_image(save_error_dialog, save_error_img);
}

G_MODULE_EXPORT void window_destroy(GtkWidget* widget, GtkBuilder* builder)
{
    g_object_unref(G_OBJECT(pixbuf));
}

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);
    GtkBuilder* builder = gtk_builder_new_from_file("test.ui");
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_builder_connect_signals(builder, builder);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}
