#include <string.h>
#include <src/variable.h>

struct variable variables[1024];
int var_count = 0;

/* 根据字符串获取类型 */
int detect_type(char *str_type)
{
	if(!strcmp(str_type, "Int"))
	{
		return VAR_TYPE_INT;
	}
}

void register_variable(AST_NODE *top_ast)
{
	int i = 0;
	while(top_ast->nodes[i] != 0)
	{
		if(top_ast->nodes[i]->type == AST_TYPE_VAR_DECLEAR)
		{
			strcpy(variables[var_count].name, top_ast->nodes[i]->nodes[0]->data);
			variables[var_count].type = detect_type(top_ast->nodes[i]->nodes[1]->data);
			var_count++;
		}
		i++;
	}
}
