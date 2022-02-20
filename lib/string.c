#include <ctype.h>

/* 检测是否为数字 */
int is_number(char *str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		if(!isdigit(str[i]))
		{
			return 0;
		}
		i += 1;
	}
	return 1;
}
