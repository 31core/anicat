#include <stdio.h>
#include <src/token.h>
#include <src/ast.h>

/* 打印token */
void print_token(struct token tk)
{
	printf("Name: \"%s\"\n", tk.name);
	printf("Type: %d\n", tk.type);
}
/* 打印AST节点 */
void print_ast_node(struct ast_node *node)
{
	printf("Name: %s\n", node->name);
	printf("Type: %d\n", node->type);
}
/* 打印AST */
static void print_ast(struct ast_node *node, int tab)
{
	for(int j = 0; j < tab; j++)
	{
		printf("--");
	}
	printf("Name: %s\n", node->name);
	for(int j = 0; j < tab; j++)
	{
		printf("--");
	}
	printf("Type: %d\n", node->type);
}
/* 打印AST树 */
static void print_ast_tree_sub(struct ast_node *node, int re)
{
	print_ast(node, re);
	int i = 0;
	while(node->node[i] != NULL)
	{
		print_ast_tree_sub(node->node[i], re + 1);
		i += 1;
	}
}
/* 打印AST树 */
void print_ast_tree(struct ast_node *node)
{
	 print_ast_tree_sub(node, 0);
}