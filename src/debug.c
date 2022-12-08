#include <stdio.h>
#include <src/function.h>
#include <src/variable.h>

/* 打印token */
void print_token(TOKEN tk)
{
	printf("Name: \"%s\"\n", tk.name);
	char *types[] = {"TOKEN_TYPE_UNKOWN",
		"TOKEN_TYPE_NAME",
		"TOKEN_TYPE_KEYWORD",
		"TOKEN_TYPE_EQU",
		"TOKEN_TYPE_EXPLAIN",
		"TOKEN_TYPE_LS_BKT",
		"TOKEN_TYPE_LM_BKT",
		"TOKEN_TYPE_LL_BKT",
		"TOKEN_TYPE_RS_BKT",
		"TOKEN_TYPE_RM_BKT",
		"TOKEN_TYPE_RL_BKT",
		"TOKEN_TYPE_ADD",
		"TOKEN_TYPE_SUB",
		"TOKEN_TYPE_MUL",
		"TOKEN_TYPE_DIV",
		"TOKEN_TYPE_GREATER",
		"TOKEN_TYPE_LESS",
		"TOKEN_TYPE_NUMBER",
		"TOKEN_TYPE_SPLIT",
		"TOKEN_TYPE_ISEQU",
		"TOKEN_TYPE_GREQU",
		"TOKEN_TYPE_LEEQU",
		"TOKEN_TYPE_STRING"
	};
	printf("Type: %s\n", types[tk.type]);
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
	char *types[] = {"AST_TYPE_UNDEFINED",
		"AST_TYPE_PROGRAM",
		"AST_TYPE_IDENTIFIER",
		"AST_TYPE_VAR_DECLEAR",
		"AST_TYPE_VAR_TYPE",
		"AST_TYPE_VAR_SET_VALUE",
		"AST_TYPE_VAR_GET_VALUE",
		"AST_TYPE_FUNC_DEF",
		"AST_TYPE_FUNC_CALL",
		"AST_TYPE_CODE_BLOCK",
		"AST_TYPE_PARAMS",
		"AST_TYPE_IF",
		"AST_TYPE_EXPRESS",
		"AST_TYPE_ADD",
		"AST_TYPE_SUB",
		"AST_TYPE_MUL",
		"AST_TYPE_DIV",
		"AST_TYPE_GR",
		"AST_TYPE_LE",
		"AST_TYPE_GREQU",
		"AST_TYPE_LEEQU",
		"AST_TYPE_EQU",
		"AST_TYPE_NUMBER",
		"AST_TYPE_STRING",
		"AST_TYPE_VARIABLE"
	};
	printf("Type: %s\n", types[node->type]);

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
	while(node->nodes[i] != NULL)
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

void show_functions(void)
{
	printf("--- Functions ---\n");
	for(int i = 0; i < func_count; i++)
	{
		printf("%d: %s\n", i, functions[i].name);
	}
}

void show_variables(void)
{
	printf("--- Global variables ---\n");
	for(int i = 0; i < var_count; i++)
	{
		printf("%d: %s\n", i, variables[i].name);
	}
}
