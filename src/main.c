#include <stdio.h>
#include <token.h>

int main()
{
	char str[100];
	struct token tk[10];
	format_code(str, "var i: Int = (9 + 3) * 2");
	puts(str);
	int num = scan_code(&tk, str);
	int i = 0;
	for(; i < num; i++)
	print_koken(tk[i]);
}