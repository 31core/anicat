#include <stdio.h>
#include <src/ast.h>
#include <src/token.h>

/* print token */
void print_token(TOKEN tk)
{
	const char *types[] = {"TOKEN_TYPE_UNKOWN",
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
	printf("%s %s\n", tk.name, types[tk.type]);
}
/* print token list */
void print_tokens(TOKEN *tk)
{
	int i = 0;
	while(tk[i].type != 0)
	{
		print_token(tk[i]);
		i++;
	}
}
/* print AST */
static void _print_ast_node(AST_NODE *node, int tab)
{
	for(int j = 0; j < tab; j++)
	{
		printf("\t");
	}
	const char *types[] = {"AST_TYPE_UNDEFINED",
		"AST_TYPE_PROGRAM",
		"AST_TYPE_IDENTIFIER",
		"AST_TYPE_VAR_DECLARE",
		"AST_TYPE_VAR_TYPE",
		"AST_TYPE_VAR_SET_VALUE",
		"AST_TYPE_VAR_GET_VALUE",
		"AST_TYPE_FUNC_DEF",
		"AST_TYPE_FUNC_CALL",
		"AST_TYPE_CODE_BLOCK",
		"AST_TYPE_PARAMS",
		"AST_TYPE_IF",
		"AST_TYPE_IFELSE",
		"AST_TYPE_ELSE",
		"AST_TYPE_FOR",
		"AST_TYPE_WHILE",
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
		"AST_TYPE_STRING"
	};
	printf("%s %s\n", types[node->type], node->data);
}

static void _print_ast(AST_NODE *node, int re)
{
	_print_ast_node(node, re);
	int i = 0;
	while(node->nodes[i] != NULL)
	{
		_print_ast(node->nodes[i], re + 1);
		i++;
	}
}
/* print AST */
void print_ast(AST_NODE *node)
{
	_print_ast(node, 0);
}
