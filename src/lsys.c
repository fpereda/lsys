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

#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "lsys.h"
#include "stack.h"

static struct lsys_limits lims;
const static struct lsys_limits *limsptr;

static stack *get_saved_pos(void)
{
	static stack st;
	static stack *s;

	if (s == NULL) {
		s = &st;
		stack_init(s, NULL);
	}

	return s;
}

static struct position limits_pos = {0, 0, 0};

static int calculate_limits(int rule, unsigned short curdepth)
{
	position_after_rule(rule, &limits_pos);

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	lims.max_x = MAX(lims.max_x, limits_pos.x);
	lims.max_y = MAX(lims.max_y, limits_pos.y);
	lims.min_x = MIN(lims.min_x, limits_pos.x);
	lims.min_y = MIN(lims.min_y, limits_pos.y);

	return rule;
}

static void do_compute_figure(
		const char *current,
		unsigned depth,
		unsigned curdepth,
		int (*process)(int, unsigned short))
{
	const char *p;
	struct lsys_opts *opts = get_lsys_opts();

	if (depth <= 0) {
		for (p = current; *p ; p++)
			process(*p, curdepth);
		return;
	}

	for (p = current; *p ; p++) {
		int c = toupper(*p);
		if (isalpha(c) && opts->rules[c])
			do_compute_figure(opts->rules[c],
					depth - 1, curdepth + 1, process);
		else
			process(c, curdepth);
	}
}

void compute_figure(
		const char *current,
		unsigned depth,
		int (*process)(int, unsigned short))
{
	do_compute_figure(current, depth, 0, process);
}

void position_after_rule(int rule, struct position *pos)
{
	struct lsys_opts *opts = get_lsys_opts();
	stack *saved_pos = get_saved_pos();
	struct position *npos;
	switch (rule) {
		case '#':
			pos->x = 0;
			pos->y = 0;
			pos->degree = opts->initial_degree;
			break;
		case 'G':
		case 'F':
			pos->y += sin(pos->degree);
			pos->x += cos(pos->degree);
			break;
		case '+':
			pos->degree -= opts->degree_step;
			break;
		case '-':
			pos->degree += opts->degree_step;
			break;
		case '[':
			npos = malloc(sizeof(*npos));
			memcpy(npos, pos, sizeof(*npos));
			stack_push(saved_pos, npos);
			break;
		case ']':
			npos = stack_pop(saved_pos);
			memcpy(pos, npos, sizeof(*npos));
			free(npos);
			break;
	}
}

void invalidate_lsys_limits(void)
{
	limsptr = NULL;
}

const struct lsys_limits *get_lsys_limits(void)
{
	if (limsptr == NULL) {
		struct lsys_opts *o = get_lsys_opts();
		limits_pos.degree = o->initial_degree;
		compute_figure(o->axiom, o->depth, calculate_limits);
		limsptr = &lims;
	}

	return limsptr;
}

struct lsys_opts *get_lsys_opts(void)
{
	static struct lsys_opts opts;
	static struct lsys_opts *p;

	if (p == NULL) {
		opts.axiom = "F";

#define SIZEOF_ARRAY(a) (sizeof(a)/sizeof(a[0]))
		unsigned i;
		for (i = 0; i < SIZEOF_ARRAY(opts.rules); i++)
			opts.rules[i] = 0;

		opts.depth = 0;
		opts.degree_step = M_PI / 2;
		opts.initial_degree = 0;
		opts.xmax = 800;
		opts.ymax = 600;

		p = &opts;
	}

	return p;
}
