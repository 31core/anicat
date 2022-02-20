#include <stdio.h>
#include <stdlib.h>

void exception(int code, char *exc)
{
	printf("%s\n", exc);
	exit(code);
}