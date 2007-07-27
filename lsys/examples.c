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
#include <string.h>
#include <math.h>

#include "lsys.h"
#include "examples.h"

void example_dragon_curve(struct lsys_opts *o);
void example_koch_curve(struct lsys_opts *o);
void example_peano_curve(struct lsys_opts *o);
void example_hilbert_curve(struct lsys_opts *o);
void example_hilbert2_curve(struct lsys_opts *o);
void example_antikoch_island(struct lsys_opts *o);
void example_koch_island(struct lsys_opts *o);
void example_sierpinski_arrowhead(struct lsys_opts *o);
void example_sierpinski_sieve(struct lsys_opts *o);
void example_sierpinski_carpet(struct lsys_opts *o);
void example_plant1(struct lsys_opts *o);
void example_plant2(struct lsys_opts *o);
void example_plant3(struct lsys_opts *o);
void example_plant4(struct lsys_opts *o);
void example_plant5(struct lsys_opts *o);
void example_tree1(struct lsys_opts *o);

static const struct lsysexample lsysexamples[] = {
	{"Koch Curve", "koch-curve", "Interesting mathematical monster", example_koch_curve},
	{"Dragon Curve", "dragon", "A filling curve fractal", example_dragon_curve},
	{"Peano Curve", "peano", "Another filling curve fractal", example_peano_curve},
	{"Hilbert Curve", "hilbert", "Yet another filling curve", example_hilbert_curve},
	{"Hilbert Curve II ", "hilbert2", "Variant of the Hilbert Curve", example_hilbert2_curve},
	{"anti-Koch Island", "akoch-island", "Koch Island with oposite angles", example_antikoch_island},
	{"Koch Island", "koch-island", "Three Koch curves forming a triangle", example_koch_island},
	{"Sierpinski Arrowhead", "sier-arrow", "The Sierpinski Arrowhead", example_sierpinski_arrowhead},
	{"Sierpinski Sieve", "sier-sieve", "The Sierpinski Sieve", example_sierpinski_sieve},
	{"Sierpinski Carpet", "sier-carpet", "The Sierpinski Carpet", example_sierpinski_carpet},
	{"Plant 1", "plant1", "A plant-like lsystem", example_plant1},
	{"Plant 2", "plant2", "A plant-like lsystem", example_plant2},
	{"Plant 3", "plant3", "A plant-like lsystem", example_plant3},
	{"Plant 4", "plant4", "A plant-like lsystem", example_plant4},
	{"Plant 5", "plant5", "A plant-like lsystem", example_plant5},
	{"Tree 1", "tree1", "A tree/bush-like lsystem", example_tree1},
	{0, 0, 0, 0}
};

const struct lsysexample *lsys_get_examples(void)
{
	return lsysexamples;
}

int lsys_set_example(char *key, struct lsys_opts *o)
{
	const struct lsysexample *e;
	for (e = lsysexamples; e->lname; e++) {
		if (strcmp(key, e->key) != 0)
			continue;
		e->do_example(o);
		return 0;
	}
	return 1;
}

void example_dragon_curve(struct lsys_opts *o)
{
	o->axiom = "FX";
	o->depth = 14;
	o->degree_step = M_PI / 4;

	o->rules['F'] = "";
	o->rules['Y'] = "+FX--FY+";
	o->rules['X'] = "-FX++FY-";
}

void example_koch_curve(struct lsys_opts *o)
{
	o->axiom = "F";
	o->depth = 5;
	o->degree_step = M_PI / 4;
	o->rules['F'] = "F+F--F+F";

}

void example_peano_curve(struct lsys_opts *o)
{
	o->axiom = "F";
	o->depth = 2;
	o->degree_step = M_PI_2;
	o->rules['F'] = "F+F-F-F-F+F+F+F-F";
}

void example_hilbert_curve(struct lsys_opts *o)
{
	o->axiom = "L";
	o->depth = 2;
	o->degree_step = M_PI_2;
	o->rules['L'] = "+RF-LFL-FR+";
	o->rules['R'] = "-LF+RFR+FL-";
}

void example_hilbert2_curve(struct lsys_opts *o)
{
	o->axiom = "X";
	o->depth = 2;
	o->degree_step = M_PI_2;
	o->rules['X'] = "XFYFX+F+YFXFY-F-XFYFX";
	o->rules['Y'] = "YFXFY-F-XFYFX+F+YFXFY";
}

void example_antikoch_island(struct lsys_opts *o)
{
	o->axiom = "F++F++F";
	o->depth = 3;
	o->degree_step = M_PI / 3;
	o->rules['F'] = "F+F--F+F";
}

void example_koch_island(struct lsys_opts *o)
{
	o->axiom = "F++F++F";
	o->depth = 8;
	o->degree_step = M_PI / 3;
	o->rules['F'] = "F-F++F-F";
}

void example_sierpinski_arrowhead(struct lsys_opts *o)
{
	o->axiom = "YF";
	o->depth = 8;
	o->degree_step = M_PI / 3;
	o->rules['X'] = "YF+XF+Y";
	o->rules['Y'] = "XF-YF-X";
}

void example_sierpinski_sieve(struct lsys_opts *o)
{
	o->axiom = "FXF--FF--FF";
	o->depth = 7;
	o->initial_degree = M_PI;
	o->degree_step = M_PI / 3;
	o->rules['F'] = "FF";
	o->rules['X'] = "--FXF++FXF++FXF--";
}

void example_sierpinski_carpet(struct lsys_opts *o)
{
	o->axiom = "F";
	o->depth = 5;
	o->degree_step = M_PI / 2;
	o->rules['F'] = "F+F-F-F-G+F+F+F-F";
	o->rules['G'] = "GGG";
}

void example_plant1(struct lsys_opts *o)
{
	o->axiom = "X";
	o->depth = 7;
	o->initial_degree = -M_PI_2;
	o->degree_step = 25 * M_PI / 180;
	o->rules['F'] = "FF";
	o->rules['X'] = "F+[[X]-X]-F[-FX]+X";
}

void example_plant2(struct lsys_opts *o)
{
	o->axiom = "X";
	o->depth = 10;
	o->initial_degree = -M_PI_2;
	o->degree_step = M_PI / 8;
	o->rules['F'] = "FF";
	o->rules['X'] = "F[+X]F[+X]-X";
}

void example_plant3(struct lsys_opts *o)
{
	o->axiom = "X";
	o->depth = 10;
	o->initial_degree = -M_PI_2;
	o->degree_step = 20 * M_PI / 180;
	o->rules['F'] = "FF";
	o->rules['X'] = "F[+X]F[--X]F[+X]";
}

void example_plant4(struct lsys_opts *o)
{
	o->axiom = "Z";
	o->depth = 8;
	o->initial_degree = -M_PI_2;
	o->degree_step = M_PI / 7;
	o->rules['Z'] = "ZFX[+Z][-Z]";
	o->rules['X'] = "X[-FFF][+FFF]FX";
}

void example_plant5(struct lsys_opts *o)
{
	o->axiom = "X";
	o->depth = 8;
	o->degree_step = M_PI / 6;
	o->initial_degree = -M_PI_2;
	o->rules['F'] = "FF";
	o->rules['X'] = "F[+X][-X]FX";
}

void example_tree1(struct lsys_opts *o)
{
	o->axiom = "F";
	o->initial_degree = -M_PI_2;
	o->depth = 5;
	o->degree_step = M_PI / 8;
	o->rules['F'] = "FF-[-F+F+F]+[+F-F-F]";
}
