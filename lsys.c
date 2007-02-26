#include <stdio.h>
#include <stdlib.h>

char *rules['Z' - 'A'];

void compute_figure(const char *current, unsigned depth)
{
	if (depth <= 0) {
		printf("%s", current);
		return;
	}

	const char *p;
	for (p = current; *p ; p++) {
		char c = toupper(*p);
		if (isalpha(c) && rules[c]) {
			compute_figure(rules[c], depth - 1);
		} else {
			putchar(c);
		}
	}
}

int main(int argc, char *argv[])
{
	char *axiom = "[F]4-F";
	unsigned depth = 1;

	rules['F'] = "|[+F][-F]";

	compute_figure(axiom, depth);

	putchar('\n');

	return EXIT_SUCCESS;
}
