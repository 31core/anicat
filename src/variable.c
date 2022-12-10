#include <string.h>
#include <src/variable.h>
#include <lib/string.h>

struct variable variables[1024];
int var_count = 0;

/* 根据字符串获取类型 */
int get_type_from_str(char *str_type)
{
	if(!strcmp(str_type, "Int"))
	{
		return VAR_TYPE_INT;
	}
}

int detect_type(char *str)
{
	if(is_number(str))
	{
		return VAR_TYPE_INT;
	}
	else if(str[0] == '"' && str[strlen(str) - 1] == '=')
	{
		return VAR_TYPE_STRING;
	}
	else if(strcmp(str, "true") && strcmp(str, "false"))
	{
		return VAR_TYPE_BOOL;
	}
}

void register_variable(AST_NODE *top_ast)
{
	int i = 0;
	while(top_ast->nodes[i] != 0)
	{
		if(top_ast->nodes[i]->type == AST_TYPE_VAR_DECLARE)
		{
			strcpy(variables[var_count].name, top_ast->nodes[i]->nodes[0]->data);
			variables[var_count].type = get_type_from_str(top_ast->nodes[i]->nodes[1]->data);
			var_count++;
		}
		i++;
	}
}
