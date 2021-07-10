#include <string.h>
#include <token.h>
#include <keywords.h>

/* 获取标识符位置,以识别语句类型 */
static int get_flag_pos(int ret[], char *str)
{
	int i = 0;
	int size = 1;
	char *symbol = " =(),:+-*/";
	while(str[i] != '\0')
	{
		for(int j = 0; j < strlen(symbol) + 1; j++)
		{
			if(str[i] == symbol[j])
			{
				ret[size] = i;
				size += 1;
			}
		}
		i += 1;
	}
	ret[0] = -1;
	ret[size] = strlen(str) + 1;
	return size + 1;
}
/* 词法分析 */
int scan_code(struct token *tk, char *code)
{
	int i = 0;
	int symbol_list[10];
	int iter = 0;
	int size = get_flag_pos(symbol_list, code);
	char tmp_code[20];
	for(i = 1; i < size; i++)
	{
		/* 单符号 */
		if(symbol_list[i] - symbol_list[i - 1] == 1)
		{
			tk[iter].name[0] = code[symbol_list[i]];
			tk[iter].name[1] = '\0';
		}
		else
		{
			strcpy(tmp_code, code);
			tmp_code[symbol_list[i]] = '\0';
			strcpy(tk[iter].name, tmp_code + symbol_list[i - 1] + 1);

			tk[iter + 1].name[0] = code[symbol_list[i]];
			tk[iter + 1].name[1] = '\0';
			iter += 1;
		}
		iter += 1;
	}
	return iter - 1;

}