#include <stdio.h>
#include <string.h>
#include <src/ast.h>
#include <src/exception.h>
#include <src/function.h>

void execute_code(AST_NODE *top_node)
{
	int ast_index = 0;
	AST_NODE *ast = top_node->nodes[2];
	while(ast->nodes[ast_index] != 0)
	{
		ast_index++;
	}
}

void execute_func(struct function *func)
{
	var_pool_init(&func->pool);
	int ast_index = 0;
	AST_NODE *ast = func->code_block;
	while(ast->nodes[ast_index] != 0)
	{
		if(ast->nodes[ast_index]->type == AST_TYPE_VAR_DECLEAR)
		{
			var_pool_append(&func->pool, ast->nodes[ast_index]->nodes[0]->data,
				detect_type(ast->nodes[ast_index]->nodes[1]->data));
		}
		else if(ast->nodes[ast_index]->type == AST_TYPE_FUNC_CALL)
		{
			struct function *func_ptr = get_func_ptr(ast->nodes[ast_index]->nodes[0]->data);
			if(func_ptr == NULL)
			{
				char excp[50];
				sprintf(excp, "'%s' no such function.", ast->nodes[ast_index]->nodes[0]->data);
				exception(EXCEPTION_UNDEFINED_FUNCTION, excp);
			}
			execute_func(func_ptr);
		}
		ast_index++;
	}
	var_pool_free(&func->pool);
}
