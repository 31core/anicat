#include <string.h>
#include <src/function.h>

struct function functions[1024];
int func_count = 0;

void register_function(AST_NODE *top_ast)
{
	int i = 0;
	while(top_ast->nodes[i] != 0)
	{
		if(top_ast->nodes[i]->type == AST_TYPE_FUNC_DEF)
		{
			strcpy(functions[func_count].name, top_ast->nodes[i]->nodes[0]->data);
			functions[func_count].code_block = top_ast->nodes[i]->nodes[1];
			func_count++;
		}
		i++;
	}
}
