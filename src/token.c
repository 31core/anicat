#include <string.h>
#include <src/token.h>
#include <src/keywords.h>
#include <lib/string.h>

/* detect the positions of symbols */
static int get_flag_pos(int ret[], const char *str)
{
	int i = 0;
	int size = 1;
	const char *symbol = " =()[]{},:;+-*/\t\n";
	while(str[i] != '\0')
	{
		for(int j = 0; j < strlen(symbol); j++)
		{
			if(str[i] == symbol[j])
			{
				ret[size] = i;
				size++;
			}
		}
		i++;
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

/* 词法分析 */
void generate_token(TOKEN *tk, const char *code)
{
	int symbol_list[1024];
	int iter = 0;
	int size = get_flag_pos(symbol_list, code); //获取每个分隔符位置
	char tmp_code[1024];
	for(int i = 1; i < size; i++)
	{
		/* single byte sybmol */
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
			iter++;
		}
		iter++;
	}
	int token_size = iter - 2;

	/* 删除空格token */
	for(int i = 0; i < token_size; i++)
	{
		if(tk[i].name[0] == ' ' || tk[i].name[0] == '\t' || tk[i].name[0] == '\n' || tk[i].name[0] == '\r')
		{
			for(int j = i; j < token_size - 1; j++)
			{
				tk[j] = tk[j + 1];
			}
			token_size--;
			i--;
		}
	}
	/* detect types */
	for(int i = 0; i < token_size; i++)
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
		else if(tk[i].name[0] == '"' && tk[i].name[strlen(tk[i].name) - 1] == '"')
		{
			tk[i].type = TOKEN_TYPE_STRING;
		}
		else if(tk[i].name[0] == '=')
		{
			/* == */
			if(tk[i + 1].name[0] == '=')
			{
				tk[i].type = TOKEN_TYPE_ISEQU;
				strcpy(tk[i].name, "==");
				for(int j = i + 1; j < token_size - 1; j++)
				{
					tk[j] = tk[j + 1];
				}
				token_size--;
			}
			/* = */
			else
			{
				tk[i].type = TOKEN_TYPE_EQU;
			}
		}
		/* <, >, <=, >= */
		else if(tk[i].name[0] == '>' || tk[i].name[0] == '<')
		{
			/* >= or <= */
			if(tk[i + 1].name[0] == '=')
			{
				if(tk[i].name[0] == '>')
				{
					strcpy(tk[i].name, ">=");
					tk[i].type = TOKEN_TYPE_GREQU;
				}
				else
				{
					strcpy(tk[i].name, "<=");
					tk[i].type = TOKEN_TYPE_LEEQU;
				}

				for(int j = i + 1; j < token_size - 1; j++)
				{
					tk[j] = tk[j + 1];
				}
				token_size--;
			}
			else if(tk[i].name[0] == '>')
			{
				tk[i].type = TOKEN_TYPE_GREATER;
			}
			else
			{
				tk[i].type = TOKEN_TYPE_LESS;
			}
		}
		else if(strlen(tk[i].name) == 1)
		{
			if(tk[i].name[0] == ':')
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
	tk[token_size].type = TOKEN_TYPE_UNKOWN;
}
