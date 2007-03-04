/* vim: set sts=4 sw=4 noet : */

/*
 * Copyright (c) 2007, Fernando J. Pereda <ferdy@gentoo.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the library nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "lsys.h"
#include "draw.h"

char *axiom = "F-F-F-F-F-F";
unsigned int depth = 4;
double max_x, max_y, min_x, min_y;

int calcule_limits(int rule)
{
	static double x = 0, y = 0;
	static int degree = 0;

	position_after_rule(rule, &degree, &x, &y);

	max_x = MAX(max_x, x);
	max_y = MAX(max_y, y);
	min_x = MIN(min_x, x);
	min_y = MIN(min_y, y);

	return rule;
}

static gboolean handle_expose(GtkWidget *widget,
		GdkEventExpose *event,
		gpointer data)
{
	const int width = event->area.width;
	const int height = event->area.height;

	if(!cr) cr = gdk_cairo_create(widget->window);

	cairo_rectangle(cr,event->area.x, event->area.y,
			width, height);
	cairo_scale(cr, 10, 10);
	cairo_translate(cr, 7.5, 7.5);
	cairo_set_line_width(cr, 0.04);
	cairo_clip(cr);

	/* Background */
	cairo_save(cr);
	cairo_set_source_rgba(cr, 0.337, 0.612, 0.117, 0.9);   // green
	cairo_paint(cr);
	cairo_stroke(cr);
	cairo_restore(cr);

	/* Paint! */
	cairo_save(cr);
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_move_to(cr, 0, 0);
	compute_figure(axiom, depth, draw_rule);
	cairo_stroke(cr);
	cairo_restore(cr);

	return FALSE;
}

int main(int argc, char *argv[])
{
	degree_step = 60;
	rules['F'] = "F-F++F-F";
	cr = NULL;

	compute_figure(axiom, depth, calcule_limits);
	printf("limits: MX:%f MY:%f mX:%f mY:%f\n", max_x, max_y, min_x, min_y);

	GtkWidget *window;
	GtkWidget *drawing_area, *menu_bar;
	GtkWidget *lyout_top;

	gtk_init(&argc, &argv);

	/* Set up window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window), "lsys");
	g_signal_connect(window, "destroy",
			G_CALLBACK (gtk_main_quit), NULL);

	/* Set drawing area */
	drawing_area = gtk_drawing_area_new();
	g_signal_connect (drawing_area, "expose-event",
			G_CALLBACK (handle_expose), NULL);

	/* Set UI */
	/* ...    */

	/* Layout */
	lyout_top = gtk_vbox_new(FALSE,5);
	/*menu_bar = gtk_menu_bar_new();
	  gtk_container_add(GTK_CONTAINER (lyout_top), menu_bar);*/
	gtk_container_add(GTK_CONTAINER (lyout_top), drawing_area);
	gtk_container_add(GTK_CONTAINER (window), lyout_top);

	gtk_widget_show_all(window);
	gtk_main();

	return EXIT_SUCCESS;
}
