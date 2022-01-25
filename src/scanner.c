#include <string.h>
#include <ctype.h>
#include <src/token.h>
#include <src/keywords.h>

/* 获取标识符位置,以识别语句类型 */
static int get_flag_pos(int ret[], char *str)
{
	int i = 0;
	int size = 1;
	char *symbol = " =()[]{},:;+-*/\t\n";
	while(str[i] != '\0')
	{
		for(int j = 0; j < strlen(symbol); j++)
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
/* 检测是否为关键字 */
static int is_keyword(char *str)
{
	char *kyewords[] = {KEYWORDS}; //关键字数组
	for(int i = 0; i < KEYWORDS_COUNT; i++)
	{
		if(!strcmp(str, kyewords[i]))
		{
			return 1;
		}
	}
	return 0;
}
/* 检测是否为数字 */
static int is_number(char *str)
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
/* 词法分析 */
void scan_code(TOKEN tk[], char *code)
{
	int symbol_list[100];
	int iter = 0;
	int size = get_flag_pos(symbol_list, code); //获取每个分隔符位置
	char tmp_code[1024];
	for(int i = 1; i < size; i++)
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
	/* 删除空格token */
	for(int i = 0; i < iter - 1; i++)
	{
		if(tk[i].name[0] == ' ' || tk[i].name[0] == '\t' || tk[i].name[0] == '\n')
		{
			for(int j = i; j < iter - 1; j++)
			{
				tk[j] = tk[j + 1];
			}
			iter -= 1;
			i -= 1;
		}
	}
	/* 确定每个token的type */
	for(int i = 0; i < iter - 1; i++)
	{
		tk[i].type = TOKEN_TYPE_NAME;
		if(is_keyword(tk[i].name))
		{
			tk[i].type = TOKEN_TYPE_KEYWORD;
		}
		else if(is_number(tk[i].name))
		{
			tk[i].type = TOKEN_TYPE_NUMBER;
		}
		else if(strlen(tk[i].name) == 1)
		{
			if(tk[i].name[0] == '=')
			{
				tk[i].type = TOKEN_TYPE_EQU;
			}
			else if(tk[i].name[0] == ':')
			{
				tk[i].type = TOKEN_TYPE_EXPLAIN;
			}
			else if(tk[i].name[0] == '+')
			{
				tk[i].type = TOKEN_TYPE_ADD;
			}
			else if(tk[i].name[0] == '-')
			{
				tk[i].type = TOKEN_TYPE_SUB;
			}
			else if(tk[i].name[0] == '*')
			{
				tk[i].type = TOKEN_TYPE_MUL;
			}
			else if(tk[i].name[0] == '/')
			{
				tk[i].type = TOKEN_TYPE_DIV;
			}
			else if(tk[i].name[0] == '(')
			{
				tk[i].type = TOKEN_TYPE_LS_BKT;
			}
			else if(tk[i].name[0] == '[')
			{
				tk[i].type = TOKEN_TYPE_LM_BKT;
			}
			else if(tk[i].name[0] == '{')
			{
				tk[i].type = TOKEN_TYPE_LL_BKT;
			}
			else if(tk[i].name[0] == ')')
			{
				tk[i].type = TOKEN_TYPE_RS_BKT;
			}
			else if(tk[i].name[0] == ']')
			{
				tk[i].type = TOKEN_TYPE_RM_BKT;
			}
			else if(tk[i].name[0] == '}')
			{
				tk[i].type = TOKEN_TYPE_RL_BKT;
			}
			else if(tk[i].name[0] == ',' || tk[i].name[0] == ';')
			{
				tk[i].type = TOKEN_TYPE_SPLIT;
			}
		}
	}
	tk[iter - 1].type = TOKEN_TYPE_UNKOWN;
}