#include <string.h>
#include <src/variable.h>

struct variable variables[1024];
int var_count = 0;

void register_variable(AST_NODE *top_ast)
{
	int i = 0;
	while(top_ast->nodes[i] != 0)
	{
		if(top_ast->nodes[i]->type == AST_TYPE_VAR_DECLEAR)
		{
			strcpy(variables[var_count].name, top_ast->nodes[i]->nodes[0]->data);
			if(!strcmp(top_ast->nodes[i]->nodes[1]->data, "Int"))
			{
				variables[var_count].type = VAR_TYPE_INT;
			}
			var_count++;
		}
		i++;
	}
}
