#include <ctype.h>
#include <stdbool.h>

/* 检测是否为数字 */
bool is_number(const char *str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		if(!isdigit(str[i]))
		{
			return false;
		}
		i++;
	}
	return true;
}
