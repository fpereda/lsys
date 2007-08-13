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
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include <lsys/lsys.h>
#include <lsys/examples.h>
#include <lsysutil/xfuncs.h>

#include <copme/copme.h>

#include "gui.h"
#include "examples.h"
#include "about.h"

static void usage_pre(void)
{
	printf("Usage: %s [options]\n\n", LSYS_PACKAGE);
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

int main(int argc, char *argv[])
{
	struct lsys_opts *opts = get_lsys_opts();

	struct copme_long acts[] = {
		{"depth", 'd', "Generation of the l-system", COPME_HASARG},
		{"axiom", 'a', "Starting point of the l-system", COPME_HASARG},
		{"degree-step", 's', "Delta. Degrees to turn in + and -", COPME_HASARG},
		{"initial-degree", 'i', "Initial degree of the turtle", COPME_HASARG},
		{"delta-depth", 'D', "Delta for each depth. Command '|'", COPME_HASARG},
		{"rule", 'r', "Add a production rule to the l-system", COPME_HASARG},
		{"list-examples", 'l', "List available examples", COPME_NOARG},
		{"example", 'e', "Use settings from an example", COPME_HASARG},
		{0, 0, 0, 0}
	};

	struct copme_long otheracts[] = {
		{"raw", 0, "Don't paint anything. Just print the l-system", COPME_NOARG},
		{"help", 'h', "Display this information message", COPME_NOARG},
		{"version", 'V', "Display version information", COPME_NOARG},
		{0, 0, 0, 0}
	};

	struct copme_group groups[] = {
		{"Actions", "Actions for lsys", acts},
		{"Other actions", "Less-common actions for lsys", otheracts},
		{0, 0, 0}
	};

	struct copme_long *o_depth = copme_option_named(groups, "depth");
	struct copme_long *o_axiom = copme_option_named(groups, "degree-step");
	struct copme_long *o_degree_step = copme_option_named(groups, "degree-step");
	struct copme_long *o_initial_degree = copme_option_named(groups, "initial-degree");
	struct copme_long *o_delta_depth = copme_option_named(groups, "delta-depth");
	struct copme_long *o_rule = copme_option_named(groups, "rule");
	struct copme_long *o_list_examples = copme_option_named(groups, "list-examples");
	struct copme_long *o_example = copme_option_named(groups, "example");
	struct copme_long *o_raw = copme_option_named(groups, "raw");
	struct copme_long *o_help = copme_option_named(groups, "help");
	struct copme_long *o_version = copme_option_named(groups, "version");

	struct copme_state *cst = copme_init(groups, argc, argv);
	while (! copme_finished(cst)) {
		copme_next(cst);

		if (o_rule->arg->specified) {
			o_rule->arg->specified = 0;
			if (add_rule(opts, o_rule->arg->data) > 0)
				goto err;
		}
		if (copme_error(cst))
			goto err;
		if (o_help->specified) {
			copme_usage(cst, usage_pre, 0);
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

	if (o_example->arg->specified)
		if (lsys_set_example(o_example->arg->data, opts) > 0) {
			fprintf(stderr, "Example with key '%s' not found.\n",
					o_example->arg->data);
			goto err;
		}

	if (o_axiom->arg->specified)
		opts->axiom = o_axiom->arg->data;
	if (o_depth->arg->specified)
		opts->depth = atoi(o_depth->arg->data);
	if (o_degree_step->arg->specified)
		opts->degree_step = atof(o_degree_step->arg->data) * M_PI / 180;
	if (o_initial_degree->arg->specified)
		opts->initial_degree = -(atof(o_initial_degree->arg->data) * M_PI / 180);
	if (o_delta_depth->arg->specified) {
		double dd = atof(o_delta_depth->arg->data);
		if (dd > 0 && dd <= 1)
			opts->delta_depth = dd;
		else {
			fprintf(stderr, "delta-depth has to be between 0 and 1.\n");
			goto err;
		}
	}

	if (o_raw->specified) {
		compute_figure(opts->axiom, opts->depth, putchar_wrapper);
		putchar('\n');
		goto suc;
	}

	launch_gui(&argc, &argv);

suc:
	free(cst);
	return EXIT_SUCCESS;
err:
	free(cst);
	return EXIT_FAILURE;
}
