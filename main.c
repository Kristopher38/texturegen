#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "noise.h"

#define GETOBJ(a) gtk_builder_get_object(builder, a)

GdkPixbuf* pixbuf;

void recalc_texture(GtkBuilder* builder)
{
    GtkImage* texture = GTK_IMAGE(GETOBJ("texture_image"));
    GtkAdjustment* frequency = GTK_ADJUSTMENT(GETOBJ("frequency_adjustment"));
    gdouble freq = gtk_adjustment_get_value(frequency);
    guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);

    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int v = noise_eval((vec2d){.x = x*freq, .y = y*freq}) * 255.0f;
            guchar* p = pixels + y*rowstride + x*n_channels;
            p[0] = v;
            p[1] = v;
            p[2] = v;
        }
    }
    gtk_image_set_from_pixbuf(texture, pixbuf);
}

G_MODULE_EXPORT void newnoise_click(GtkWidget* widget, GtkBuilder* builder)
{
    noise_new();
    recalc_texture(builder);
}

G_MODULE_EXPORT void frequency_change(GtkWidget* widget, GtkBuilder* builder)
{
    recalc_texture(builder);
}

G_MODULE_EXPORT void window_show(GtkWidget* widget, GtkBuilder* builder)
{
    GtkImage* texture = GTK_IMAGE(GETOBJ("texture_image"));
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, 256, 256);
    noise_init();
    noise_new();
    recalc_texture(builder);
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
