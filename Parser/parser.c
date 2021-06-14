#include <string.h>
#include <Parser/token.h>

/* 获取标识符位置,以识别语句类型 */
static int get_next_flag(char *str)
{
	int i = 0;
	char *symbol = " =(";
	while(str[i] != 0)
	{
		for(int j = 0; j < strlen(symbol); j++)
		{
			if(str[i] == symbol[j])
			{
				return i;
			}
		}
	}
	return -1;
}

void parse_code(struct token *tk, char *code)
{
	char tmp_code[100];
	strcpy(tmp_code, code);
	int i = get_next_flag(tmp_code);
	tmp_code[i] = '\0';
}