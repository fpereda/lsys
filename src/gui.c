/* vim: set sts=4 sw=4 noet : */

/*
 * Copyright (c) 2007, Fernando J. Pereda <ferdy@gentoo.org>
 * Copyright (c) 2007, Francesc Gordillo <frangor@gmail.com>
 * Copyright (c) 2007, Santiago M. Mola Velasco <cooldwind@gmail.com>
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

#include <cairo.h>

#include "gui.h"
#include "lsys.h"
#include "draw.h"

gboolean handle_expose(GtkWidget *widget,
		GdkEventExpose *event, gpointer data)
{
	if (!sur) {
		struct lsys_opts *opts = get_lsys_opts();
		const struct lsys_limits *lims = get_lsys_limits();

		double max_x = lims->max_x;
		double max_y = lims->max_y;
		double min_x = lims->min_x;
		double min_y = lims->min_y;

		gint width;
		gint height;
		gdk_drawable_get_size(widget->window, &width, &height);


		sur = cairo_image_surface_create(
					CAIRO_FORMAT_A8, width, height);

		cr = cairo_create(sur);

		cairo_scale(cr,
				width / (max_x - min_x + (MARGIN * 2)),
				height / (max_y - min_y + (MARGIN * 2)));
		cairo_translate(cr,
				ABS(min_x - MARGIN),
				ABS(min_y - MARGIN));

		cairo_set_line_width(cr, (max_x - min_x) * 0.001);

		/* Paint! */
		cairo_save(cr);
		cairo_set_source_rgb(cr, 0, 0, 0);
		draw_rule('#');
		compute_figure(opts->axiom, opts->depth, draw_rule);
		cairo_stroke(cr);
		cairo_restore(cr);

		cairo_destroy(cr);
	}

	cairo_t *rcr = gdk_cairo_create(widget->window);

	/* Background */
	cairo_save(rcr);
	cairo_set_source_rgb(rcr, 0.337, 0.612, 0.117);   // green
	cairo_paint(rcr);
	cairo_restore(rcr);

	cairo_set_source_surface(rcr, sur, 0, 0);

	cairo_paint(rcr);
	cairo_destroy(rcr);

	return FALSE;
}
