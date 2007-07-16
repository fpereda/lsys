#include "ctme.h"

char *name(void)
{
	static char n[] = "test ctme: pass";
	return n;
}

int repeat(void)
{
	return 1;
}

void run_test(void)
{
	CTME_CHECK(1);

	int t = 0;
	CTME_CHECK_EQUAL(t, 0);

	CTME_CHECK_EQUAL_STRING("same string", "same string");

	CTME_CHECK_NOT_EQUAL(1, 2);
	CTME_CHECK_NULL(0);
	CTME_CHECK_NOT_NULL(&t);
}
