#include <stdio.h>

int main()
{
	char str[100];
	format_code(str, "var i: Int = (9 + 3) * 2");
	printf("%s\n", str);
}