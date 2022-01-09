#include <stdio.h>
//#include <src/token.h>
#include <src/ast.h>

/* 打印token */
void print_token(TOKEN tk)
{
	printf("Name: \"%s\"\n", tk.name);
	printf("Type: %d\n", tk.type);
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
/* 打印AST节点 */
void print_ast_node(AST_NODE *node)
{
	printf("Type: %d\n", node->type);
	printf("Data: %s\n", node->data);
}
/* 打印AST */
static void print_ast(AST_NODE *node, int tab)
{
	for(int j = 0; j < tab; j++)
	{
		printf("`--");
	}
	printf("Type: %d\n", node->type);
	for(int j = 0; j < tab; j++)
	{
		printf(" --");
	}
	printf("Data: %s\n", node->data);
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