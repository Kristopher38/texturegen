#ifndef H_HANDLERS
#define H_HANDLERS
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "noise.h"
#include "texgen.h"

extern GdkPixbuf* pixbuf;
extern GtkAdjustment** adjustments;

void update_sliders(GtkBuilder* builder, int n_sliders, Slider* sliders);
int read_seed(GtkEditable* editable);
TextureFunc* get_texture_func(GtkBuilder* builder);
double* get_slider_values(GtkBuilder* builder, int n_sliders);
void recalc_texture(GtkBuilder* builder);
int seed_entry_update(GtkBuilder* builder, int seed);
void seed_update_all(GtkBuilder* builder);

G_MODULE_EXPORT void slider_changed(GtkWidget* widget, GtkBuilder* builder);
G_MODULE_EXPORT void newnoise_click(GtkWidget* widget, GtkBuilder* builder);
G_MODULE_EXPORT void seed_insert_text(GtkEditable* editable, char* new_text, int new_text_length, gpointer position, GtkBuilder* builder);
G_MODULE_EXPORT void seed_change(GtkWidget* widget, GtkBuilder* builder);
G_MODULE_EXPORT void function_changed(GtkWidget* widget, GtkBuilder* builder);
G_MODULE_EXPORT void save_click(GtkWidget* widget, GtkBuilder* builder);
G_MODULE_EXPORT void window_show(GtkWidget* widget, GtkBuilder* builder);
G_MODULE_EXPORT void window_destroy(GtkWidget* widget, GtkBuilder* builder);
G_MODULE_EXPORT void ignore_scroll(GtkScale* scale, GtkBuilder* builder);

#endif
