#include <string.h>
#include <Parser/token.h>
#include <Program/keywords.h>

/* 获取标识符位置,以识别语句类型 */
static int get_next_flag(char *str)
{
	int i = 0;
	char *symbol = " =(),:+-*/";
	while(str[i] != '\0')
	{
		for(int j = 0; j < strlen(symbol) + 1; j++)
		{
			if(str[i] == symbol[j])
			{
				return i;
			}
		}
		i += 1;
	}
	return -1;
}
/* 词法分析 */
int scan_code(struct token *tk, char *code)
{
	int p = 0;
	int iter = 0;
	while(code[p] != '\0')
	{
		p = get_next_flag(code);
		/* 分析符号 */
		if(code[p] == '=')
		{
			tk[iter + 1].type = TOKEN_TYPE_EQU;
			iter += 1;
		}
		else if(code[p] == ':')
		{
			tk[iter + 1].type = TOKEN_TYPE_EXPLAIN;
			iter += 1;
		}
		else if(code[p] == '(')
		{
			tk[iter + 1].type = TOKEN_TYPE_LS_BKT;
			iter += 1;
		}
		else if(code[p] == '[')
		{
			tk[iter + 1].type = TOKEN_TYPE_LM_BKT;
			iter += 1;
		}
		else if(code[p] == '{')
		{
			tk[iter + 1].type = TOKEN_TYPE_LL_BKT;
			iter += 1;
		}
		else if(code[p] == ')')
		{
			tk[iter + 1].type = TOKEN_TYPE_RS_BKT;
			iter += 1;
		}
		else if(code[p] == ']')
		{
			tk[iter + 1].type = TOKEN_TYPE_RM_BKT;
			iter += 1;
		}
		else if(code[p] == '}')
		{
			tk[iter + 1].type = TOKEN_TYPE_RL_BKT;
			iter += 1;
		}

		code[p] = '\0';
		strcpy(tk[iter].name, code);
		iter += 1;
		code += p + 1;
	}
	return iter + 1;
}