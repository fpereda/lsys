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

#include <cairo.h>

#include <lsys/lsys.h>

#include "draw.h"

static cairo_t *cr;

int draw_rule(int rule, unsigned short curdepth)
{
	static struct position pos = {0, 0, 0};

	position_after_rule(rule, &pos, curdepth);

	if (rule == 'F' || rule == '|')
		cairo_line_to(cr, pos.x, pos.y);
	else if (rule == 'G' || rule == '#' || rule == ']')
		cairo_move_to(cr, pos.x, pos.y);

	return rule;
}

void draw_size(double *width, double *height)
{
	struct lsys_opts *o = get_lsys_opts();
	const struct lsys_limits *l = get_lsys_limits();

	double area_width = l->max_x - l->min_x + (2 * MARGIN);
	double area_height = l->max_y - l->min_y + (2 * MARGIN);

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	double factor = MIN((double) o->xmax / area_width,
			(double) o->ymax / area_height);
	if (width)
		*width = area_width * factor;
	if (height)
		*height = area_height * factor;
}

cairo_surface_t *draw_fractal(void)
{
	static cairo_surface_t *surface = 0;

	if (surface)
		return surface;

	struct lsys_opts *o = get_lsys_opts();
	const struct lsys_limits *l = get_lsys_limits();

	double width;
	double height;
	
	draw_size(&width, &height);

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8, width, height);

	cr = cairo_create(surface);

	double area_width  = l->max_x - l->min_x + (2 * MARGIN);
	double area_height = l->max_y - l->min_y + (2 * MARGIN);

	cairo_scale(cr, width / area_width, height / area_height);

#define ABS(a) (((a) < 0) ? -(a) : (a))

	cairo_translate(cr,
			ABS(l->min_x - MARGIN),
			ABS(l->min_y - MARGIN));

	cairo_set_line_width(cr, (l->max_x - l->min_x) * 0.001);

	/* Paint! */
	cairo_save(cr);
	cairo_set_source_rgb(cr, 0, 0, 0);
	draw_rule('#', 0);
	compute_figure(o->axiom, o->depth, draw_rule);
	cairo_stroke(cr);
	cairo_restore(cr);

	cairo_destroy(cr);

	return surface;
}
