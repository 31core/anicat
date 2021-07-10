#include <stdio.h>
#include <Parser/token.h>

void print_koken(struct token tk)
{
	printf("Name: %s\n", tk.name);
	printf("Type: %d\n\n", tk.type);
}