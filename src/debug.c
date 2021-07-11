#include <stdio.h>
#include <token.h>

/* 打印token */
void print_token(struct token tk)
{
	printf("Name: \"%s\"\n", tk.name);
	printf("Type: %d\n\n", tk.type);
}