#include <stdio.h>
#include <Parser/token.h>

int main()
{
	char str[100];
	struct token tk[10];
	format_code(str, "var i: Int = (9 + 3) * 2");
	scan_code(&tk, str);
	print_koken(tk);
}