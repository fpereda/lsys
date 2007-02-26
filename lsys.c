#include <stdio.h>
#include <stdlib.h>

char *rules['Z' - 'A'];

void compute_figure(const char *current, unsigned depth, int (*process)(int))
{
	const char *p;

	if (depth <= 0) {
		for (p = current; *p ; p++)
			process(*p);
		return;
	}

	for (p = current; *p ; p++) {
		char c = toupper(*p);
		if (isalpha(c) && rules[c]) {
			compute_figure(rules[c], depth - 1, process);
		} else {
			process(c);
		}
	}
}

int main(int argc, char *argv[])
{
	char *axiom = "[F]4-F";
	unsigned depth = 10;

	rules['F'] = "|[+F][-F]";

	compute_figure(axiom, depth, putchar);

	putchar('\n');

	return EXIT_SUCCESS;
}
