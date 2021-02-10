#include "handlers.h"

GdkPixbuf* pixbuf;
GtkAdjustment** adjustments;

void update_sliders(GtkBuilder* builder, int n_sliders, Slider* sliders)
{
    static int adj_count = 0;

    GtkBox* sliders_box = GTK_BOX(GETOBJ("sliders_box"));
    gtk_container_foreach(GTK_CONTAINER(sliders_box), (GtkCallback)gtk_widget_destroy, NULL);
    for (int i = 0; i < adj_count; ++i)
        g_object_unref(adjustments[i]);
    adjustments = realloc(adjustments, sizeof(GtkAdjustment*)*n_sliders);
    adj_count = n_sliders;

    for (int i = 0; i < n_sliders; ++i)
    {
        GtkBox* slider_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        GtkLabel* label = gtk_label_new(sliders[i].label);
        GtkAdjustment* adjustment = gtk_adjustment_new(sliders[i].initval,
                                                      sliders[i].min,
                                                      sliders[i].max,
                                                      sliders[i].step,
                                                      10.0, 0.0);
        GtkScale* slider = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
        gtk_scale_set_digits(slider, sliders[i].digits);
        gtk_range_set_round_digits(GTK_RANGE(slider), sliders[i].digits);
        g_signal_connect(slider, "scroll-event", ignore_scroll, builder);
        g_signal_connect(adjustment, "value-changed", slider_changed, builder);

//        gint event_mask = gtk_widget_get_events(widget);
//        event_mask = event_mask & ~GDK_SCROLL;
//        gtk_widget_set_events(widget, event_mask);

        adjustments[i] = adjustment;
        g_object_ref_sink(adjustment);
        gtk_box_pack_start(slider_box, GTK_WIDGET(label), FALSE, FALSE, 0);
        gtk_box_pack_start(slider_box, GTK_WIDGET(slider), FALSE, FALSE, 0);
        gtk_box_pack_start(sliders_box, slider_box, FALSE, FALSE, 0);
    }
    gtk_widget_show_all(sliders_box);
}

int read_seed(GtkEditable* editable)
{
    gchar* seedstr = gtk_editable_get_chars(editable, 0, -1);
    int seed = atoi(seedstr);
    g_free(seedstr);
    return seed;
}

TextureFunc* get_texture_func(GtkBuilder* builder)
{
    GtkComboBox* func_combobox = GTK_COMBO_BOX(GETOBJ("function_combobox"));
    gint id = gtk_combo_box_get_active(func_combobox);
    assert(id >= 0 && id <= tex_funcs_n);
    return &tex_funcs[id];
}

double* get_slider_values(GtkBuilder* builder, int n_sliders)
{
    double* values = malloc(sizeof(double)*n_sliders);
    for (int i = 0; i < n_sliders; ++i)
        values[i] = gtk_adjustment_get_value(adjustments[i]);
    return values;
}

void recalc_texture(GtkBuilder* builder)
{
    GtkImage* texture = GTK_IMAGE(GETOBJ("texture_image"));
    guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);

    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    Pixel* tex = malloc(sizeof(Pixel)*width*height);
    float max_pixel = 0;

    TextureFunc* tex_func = get_texture_func(builder);
    double* slider_values = get_slider_values(builder, tex_func->n_sliders);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Pixel p = tex_func->f_ptr(x, y, slider_values);
            tex[y*width + x] = p;
            if (max_pixel < p.r)
                max_pixel = p.r;
            if (max_pixel < p.g)
                max_pixel = p.g;
            if (max_pixel < p.b)
                max_pixel = p.b;
        }
    }

    GdkRGBA color;
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(GETOBJ("tint_button")), &color);
    gboolean normalize = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GETOBJ("normalize_checkbutton")));
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Pixel* ptex = tex + y*width + x;
            guchar* p = pixels + y*rowstride + x*n_channels;
            p[0] = (normalize ? fmax(0, ptex->r / max_pixel * 255.0f) : ptex->r * 255.0f) * color.red;
            p[1] = (normalize ? fmax(0, ptex->g / max_pixel * 255.0f) : ptex->g * 255.0f) * color.green;
            p[2] = (normalize ? fmax(0, ptex->b / max_pixel * 255.0f) : ptex->b * 255.0f) * color.blue;
        }
    }




    free(slider_values);
    free(tex);
    gtk_image_set_from_pixbuf(texture, pixbuf);
}

int seed_entry_update(GtkBuilder* builder, int seed)
{
    GtkEditable* seed_entry = GTK_EDITABLE(GETOBJ("seed_entry"));
    gint* pos;
    char seed_str[20];
    itoa(seed, seed_str, 10);
    gtk_editable_delete_text(seed_entry, 0, -1);
    gtk_editable_insert_text(seed_entry, seed_str, strlen(seed_str), &pos);
}

void seed_update_all(GtkBuilder* builder)
{
    int seed = rand();
    seed_entry_update(builder, seed);
    noise_init(seed);
    noise_new();
    recalc_texture(builder);
}

G_MODULE_EXPORT void slider_changed(GtkWidget* widget, GtkBuilder* builder)
{
    recalc_texture(builder);
}

G_MODULE_EXPORT void newnoise_click(GtkWidget* widget, GtkBuilder* builder)
{
    seed_update_all(builder);
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

G_MODULE_EXPORT void seed_change(GtkWidget* widget, GtkBuilder* builder)
{
    noise_init(read_seed(GTK_EDITABLE(widget)));
    noise_new();
    recalc_texture(builder);
}

G_MODULE_EXPORT void function_changed(GtkWidget* widget, GtkBuilder* builder)
{
    TextureFunc* tex_func = get_texture_func(builder);
    update_sliders(builder, tex_func->n_sliders, tex_func->sliders);
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

G_MODULE_EXPORT void window_show(GtkWidget* widget, GtkBuilder* builder)
{
    // init texture display
    GtkImage* texture = GTK_IMAGE(GETOBJ("texture_image"));
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, 256, 256);

    // init function combobox
    GtkComboBox* functions_combo = GTK_COMBO_BOX_TEXT(GETOBJ("function_combobox"));
    for (int i = 0; i < tex_funcs_n; ++i)
        gtk_combo_box_text_append_text(functions_combo, tex_funcs[i].f_name);
    gtk_combo_box_set_active(GTK_COMBO_BOX(functions_combo), 0);

    // init seed
    srand(time(NULL));
    seed_update_all(builder);

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

G_MODULE_EXPORT void ignore_scroll(GtkScale* scale, GtkBuilder* builder)
{
    g_signal_stop_emission_by_name(scale, "scroll-event");
}

G_MODULE_EXPORT void color_set(GtkColorButton* button, GtkBuilder* builder)
{
    GdkRGBA color;
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(GETOBJ("tint_button")), &color);
    printf("%f %f %f\n", color.red, color.green, color.blue);
    recalc_texture(builder);
}

G_MODULE_EXPORT void normalize_toggled(GtkCheckButton* button, GtkBuilder* builder)
{
    recalc_texture(builder);
}
