/* vim: set sts=4 sw=4 noet : */

/*
 * Copyright (c) 2007, Fernando J. Pereda <ferdy@gentoo.org>
 * Copyright (c) 2007, Francesc Gordillo <frangor@gmail.com>
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

#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>

#include "lsys.h"
#include "draw.h"
#include "gui.h"

int main(int argc, char *argv[])
{
	struct lsys_opts *opts = get_lsys_opts();

#if 0
	opts->axiom = "FX";
	opts->depth = 16;
	opts->degree_step = M_PI / 4;

	opts->rules['F'] = "";
	opts->rules['Y'] = "+FX--FY+";
	opts->rules['X'] = "-FX++FY-";
#endif

#if 0
	/* Koch Curve */
	opts->axiom = "F";
	opts->depth = 5;
	opts->degree_step = M_PI / 4;
	opts->rules['F'] = "F+F--F+F";
#endif

#if 0
	/* Peano Curve */
	opts->axiom = "F";
	opts->depth = 2;
	opts->degree_step = M_PI_2;
	opts->rules['F'] = "F+F-F-F-F+F+F+F-F";
#endif

#if 1
	/* Silly bush */
	opts->axiom = "F";
	opts->depth = 3;
	opts->initial_degree = -M_PI_2;
	opts->degree_step = M_PI / 7;
	opts->rules['F'] = "F[-F][+F]";
#endif

#if 0
	/* NFC but cool */
	opts->axiom = "F++F++F";
	opts->depth = 3;
	opts->degree_step = M_PI / 3;
	opts->rules['F'] = "F+F--F+F";
#endif

#if 0
	/* Koch island */
	opts->axiom = "F++F++F";
	opts->depth = 8;
	opts->degree_step = M_PI / 3;
	opts->rules['F'] = "F-F++F-F";
#endif

	GtkWidget *window;
	GtkWidget *drawing_area;

	gtk_init(&argc, &argv);

	/* Set up window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "lsys");

	double width;
	double height;
	draw_size(&width, &height);
	gtk_widget_set_size_request(window, width, height);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	g_signal_connect(window, "destroy",
			G_CALLBACK(gtk_main_quit), NULL);

	/* Set drawing area */
	drawing_area = gtk_drawing_area_new();
	g_signal_connect(drawing_area, "expose-event",
			G_CALLBACK(handle_expose), NULL);

	gtk_container_add(GTK_CONTAINER(window), drawing_area);

	gtk_widget_show_all(window);
	gtk_main();

	return EXIT_SUCCESS;
}
