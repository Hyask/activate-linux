#include <gtk-layer-shell/gtk-layer-shell.h>
#include <gtk/gtk.h>
#include <stdio.h>

struct config {
	char *title;
	char *subtitle;
};

void draw_window(GdkMonitor *monitor, char *text, GtkApplication *app) {
	GtkWindow *gtk_window = GTK_WINDOW(gtk_application_window_new(app));

	gtk_layer_init_for_window(gtk_window);

	gtk_layer_set_layer(gtk_window, GTK_LAYER_SHELL_LAYER_TOP);
	//
	// gtk layer shell options
	gtk_layer_set_margin(gtk_window, GTK_LAYER_SHELL_EDGE_RIGHT, 40);
	gtk_layer_set_margin(gtk_window, GTK_LAYER_SHELL_EDGE_BOTTOM, 40);
	static const gboolean anchors[] = {FALSE, TRUE, FALSE, TRUE};
	for (int i = 0; i < GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER; i++) {
		gtk_layer_set_anchor (gtk_window, i, anchors[i]);
	}

	gtk_layer_set_monitor(gtk_window, monitor);

	// set window background to transparent
	GtkCssProvider *css_provider = gtk_css_provider_new();
	char *css = "window {background-color: transparent;}"
		"#title {color: red;}";
	gtk_css_provider_load_from_data(css_provider, css, strlen(css), NULL);
	GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(gtk_window));
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


	// render label
	GtkWidget *label = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(label), text);
	gtk_container_add(GTK_CONTAINER(gtk_window), label);
	gtk_container_set_border_width(GTK_CONTAINER(gtk_window), 12);
	gtk_widget_show_all(GTK_WIDGET(gtk_window));
}

static void activate(GtkApplication *app, void *data)
{
	struct config *conf = (struct config*) data;

	int title_len = strlen(conf->title);
	int subtitle_len = strlen(conf->subtitle);
	int len = 100+title_len+subtitle_len;
	char text[len];

	snprintf(text, len, "<span font_desc=\"24.0\">%s</span>\n<span font_desc=\"16\">%s</span>", conf->title, conf->subtitle);

	//GtkWidget *monitor_selection =  monitor_selection_new(gtk_window);
	GdkDisplay *display = gdk_display_get_default ();
	for (int i = 0; i < gdk_display_get_n_monitors(display); i++) {
		GdkMonitor *monitor = gdk_display_get_monitor(display, i);
		draw_window(monitor, text, app);
	}
}

int main(int argc, char **argv)
{
	struct config conf;
	conf.title = "Activate Linux";
	conf.subtitle = "Go to Settings to activate Linux.";
	GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), (void *) &conf);
	int status = g_application_run(G_APPLICATION(app), 0, NULL);
	g_object_unref(app);
	return status;
}
