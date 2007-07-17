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
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include <lsys/lsys.h>
#include <lsys/examples.h>
#include <lsysutil/xfuncs.h>

#include <copme/copme.h>

#include "draw.h"
#include "gui.h"
#include "examples.h"
#include "about.h"

static void usage_pre(void)
{
	printf("Usage: %s [options]\n\n", PACKAGE_NAME);
}

static void usage_post(void)
{
	printf("\n");
}

static int putchar_wrapper(int c, unsigned short d)
{
	putchar(c);
	return 0;
}

static int add_rule(struct lsys_opts *o, char *r)
{
	unsigned len = strlen(r);
	if (len < 2) {
		fprintf(stderr, "Rule '%s' too short.\n", r);
		return 1;
	} else if (! isalpha(r[0])) {
		fprintf(stderr, "Invalid left hand side: '%c'.\n", r[0]);
		return 1;
	} else if (r[1] != '=') {
		fprintf(stderr, "Malformed rule '%s'.\n", r);
		return 1;
	}

	short lhs = r[0];
	char *rhs = xstrdup(r + 2);
	o->rules[lhs] = rhs;
	return 0;
}

static void parse_options(int argc, char *argv[])
{
	struct lsys_opts *opts = get_lsys_opts();

	struct copme_arg a_depth;
	struct copme_arg a_axiom;
	struct copme_arg a_degree_step;
	struct copme_arg a_initial_degree;
	struct copme_arg a_rule;
	struct copme_arg a_example;

	struct copme_long copts[] = {
		{"depth", 'd', "Generation of the l-system", COPME_HASARG, &a_depth},
		{"axiom", 'a', "Starting point of the l-system", COPME_HASARG, &a_axiom},
		{"degree-step", 's', "Delta. Degrees to turn in + and -", COPME_HASARG, &a_degree_step},
		{"initial-degree", 'i', "Initial degree of the turtle", COPME_HASARG, &a_initial_degree},
		{"rule", 'r', "Add a production rule to the l-system", COPME_HASARG, &a_rule},
		{"list-examples", 'l', "List available examples", COPME_NOARG, 0},
		{"example", 'e', "Use settings from an example", COPME_HASARG, &a_example},
		{"raw", 0, "Don't paint anything. Just print the l-system", COPME_NOARG, 0},
		{"help", 'h', "Display this information message", COPME_NOARG, 0},
		{"version", 'V', "Display version information", COPME_NOARG, 0},
		{0, 0, 0, 0, 0}
	};

	struct copme_long *o_list_examples = copme_option_named(copts, "list-examples");
	struct copme_long *o_raw = copme_option_named(copts, "raw");
	struct copme_long *o_help = copme_option_named(copts, "help");
	struct copme_long *o_version = copme_option_named(copts, "version");

	struct copme_state *cst = copme_init(copts, argc, argv);
	while (! copme_finished(cst)) {
		copme_next(cst);

		if (a_rule.specified) {
			a_rule.specified = 0;
			if (add_rule(opts, a_rule.data) > 0)
				goto err;
		}
		if (copme_error(cst))
			goto err;
		if (o_help->specified) {
			copme_usage(cst, usage_pre, usage_post);
			goto suc;
		}
	}

	if (o_version->specified) {
		printf("%s", version());
		goto suc;
	}

	if (o_list_examples->specified) {
		example_list();
		goto suc;
	}

	if (a_example.specified)
		if (lsys_set_example(a_example.data, opts) > 0) {
			fprintf(stderr, "Example with key '%s' not found.\n",
					a_example.data);
			goto err;
		}

	if (a_axiom.specified)
		opts->axiom = a_axiom.data;
	if (a_depth.specified)
		opts->depth = atoi(a_depth.data);
	if (a_degree_step.specified)
		opts->degree_step = atoi(a_degree_step.data) * M_PI / 180;
	if (a_initial_degree.specified)
		opts->initial_degree = -(atoi(a_initial_degree.data) * M_PI / 180);

	if (o_raw->specified) {
		compute_figure(opts->axiom, opts->depth, putchar_wrapper);
		putchar('\n');
		goto suc;
	}

	return;

suc:
	free(cst);
	exit(EXIT_SUCCESS);
err:
	free(cst);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	parse_options(argc, argv);

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
