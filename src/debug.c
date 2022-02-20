#include <stdio.h>
#include <src/function.h>
#include <src/variable.h>

/* 打印token */
void print_token(TOKEN tk)
{
	printf("Name: \"%s\"\n", tk.name);
	PRINT_TYPE(tk.type, TOKEN_TYPE_UNKOWN);
	PRINT_TYPE(tk.type, TOKEN_TYPE_NAME);
	PRINT_TYPE(tk.type, TOKEN_TYPE_KEYWORD);
	PRINT_TYPE(tk.type, TOKEN_TYPE_EQU);
	PRINT_TYPE(tk.type, TOKEN_TYPE_EXPLAIN);
	PRINT_TYPE(tk.type, TOKEN_TYPE_LS_BKT);
	PRINT_TYPE(tk.type, TOKEN_TYPE_LM_BKT);
	PRINT_TYPE(tk.type, TOKEN_TYPE_LL_BKT);
	PRINT_TYPE(tk.type, TOKEN_TYPE_RS_BKT);
	PRINT_TYPE(tk.type, TOKEN_TYPE_RM_BKT);
	PRINT_TYPE(tk.type, TOKEN_TYPE_RL_BKT);
	PRINT_TYPE(tk.type, TOKEN_TYPE_ADD);
	PRINT_TYPE(tk.type, TOKEN_TYPE_SUB);
	PRINT_TYPE(tk.type, TOKEN_TYPE_MUL);
	PRINT_TYPE(tk.type, TOKEN_TYPE_DIV);
	PRINT_TYPE(tk.type, TOKEN_TYPE_NUMBER);
	PRINT_TYPE(tk.type, TOKEN_TYPE_SPLIT);
	PRINT_TYPE(tk.type, TOKEN_TYPE_ISEQU);
}
/* 打印tokens */
void print_tokens(TOKEN *tk)
{
	int i = 0;
	while(tk[i].type != 0)
	{
		print_token(tk[i]);
		i += 1;
	}
}
/* 打印AST */
static void print_ast(AST_NODE *node, int tab)
{
	for(int j = 0; j < tab; j++)
	{
		printf("`--");
	}
	PRINT_TYPE(node->type, AST_TYPE_UNDEFINED);
	PRINT_TYPE(node->type, AST_TYPE_PROGRAM);
	PRINT_TYPE(node->type, AST_TYPE_NAME);
	PRINT_TYPE(node->type, AST_TYPE_VAR_DECLEAR);
	PRINT_TYPE(node->type, AST_TYPE_VAR_TYPE);
	PRINT_TYPE(node->type, AST_TYPE_VAR_SET_VALUE);
	PRINT_TYPE(node->type, AST_TYPE_VAR_COMPARE);
	PRINT_TYPE(node->type, AST_TYPE_VAR_GET_VALUE);
	PRINT_TYPE(node->type, AST_TYPE_FUNC_DEF);
	PRINT_TYPE(node->type, AST_TYPE_FUNC_CALL);
	PRINT_TYPE(node->type, AST_TYPE_CODE_BLOCK);
	PRINT_TYPE(node->type, AST_TYPE_PARAMS);
	PRINT_TYPE(node->type, AST_TYPE_IF);
	PRINT_TYPE(node->type, AST_TYPE_EXPRESS);
	PRINT_TYPE(node->type, AST_TYPE_ADD);
	PRINT_TYPE(node->type, AST_TYPE_SUB);
	PRINT_TYPE(node->type, AST_TYPE_MUL);
	PRINT_TYPE(node->type, AST_TYPE_DIV);
	PRINT_TYPE(node->type, AST_TYPE_NUMBER);
	PRINT_TYPE(node->type, AST_TYPE_EQU);
	for(int j = 0; j < tab; j++)
	{
		printf(" --");
	}
	printf("Data: \"%s\"\n", node->data);
}
/* 打印AST树 */
static void print_ast_tree_sub(AST_NODE *node, int re)
{
	print_ast(node, re);
	int i = 0;
	while(node->nodes[i] != 0)
	{
		print_ast_tree_sub(node->nodes[i], re + 1);
		i += 1;
	}
}
/* 打印AST树 */
void print_ast_tree(AST_NODE *node)
{
	print_ast_tree_sub(node, 0);
}

void show_functions()
{
	for(int i = 0; i < func_count; i++)
	{
		printf("%d: %s\n", i, functions[i].name);
	}
}

void show_variables()
{
	for(int i = 0; i < var_count; i++)
	{
		printf("%d: %s\n", i, variables[i].name);
	}
}
