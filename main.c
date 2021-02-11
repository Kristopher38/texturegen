#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

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
