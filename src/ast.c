#include <stdlib.h>
#include <string.h>
#include <src/ast.h>
//#include <src/token.h>

struct ast_node *ast_nodes;
struct ast_node *node_layer[100];

/* 初始化AST node */
void ast_node_init(AST_NODE *ast)
{
	ast->type = 0;

	for(int i = 0; i < 100; i++)
	{
		ast->nodes[i] = 0;
	}
}

void ast_node_manage_init(void)
{
	ast_nodes = malloc(sizeof(AST_NODE) * 1024);
	for(int i = 0; i < 1024; i++)
	{
		ast_nodes[i].type = -1;
	}
}

AST_NODE* ast_node_manage_alloc(void)
{
	int i = 0;
	for(; i < 1024; i++)
	{
		if(ast_nodes[i].type == -1)
		{
			ast_node_init(&ast_nodes[i]);
			return &(ast_nodes[i]);
		}
	}
}

void ast_node_manage_free(AST_NODE *node)
{
	int i = 0;
	for(; i < 1024; i++)
	{
		if(&ast_nodes[i] == node)
		{
			ast_nodes[i].type = -1;
			return;
		}
	}
}
/* 添加一个node */
void ast_node_append(AST_NODE *ast, AST_NODE *target, int p)
{
	ast->nodes[p] = target;
	ast->nodes[p + 1] = 0;
}
/* 组建AST树 */
void ast_tree_build(AST_NODE *ast, struct token tk[])
{
	int i = 0;
	while(tk[i].type != TOKEN_TYPE_UNKOWN)
	{
		/* 函数定义 */
		if(tk[i].type == TOKEN_TYPE_KEYWORD)
		{
			/* 函数定义 */
			if(!strcmp(tk[i].name, "func"))
			{
				ast->type = AST_TYPE_FUNC_DEF;

				ast_node_append(ast, ast_node_manage_alloc(), 0);
				ast_node_append(ast, ast_node_manage_alloc(), 1);
				ast_node_append(ast, ast_node_manage_alloc(), 2);

				ast->nodes[0]->type = AST_TYPE_NAME;
				ast->nodes[1]->type = AST_TYPE_PARAMS;
				ast->nodes[2]->type = AST_TYPE_CODE_BLOCK;

				strcpy(ast->nodes[0]->data, tk[i + 1].name);
				i += 1;
			}
			/* 变量声明 */
			else if(!strcmp(tk[i].name, "var"))
			{
				ast->type = AST_TYPE_VAR_DECLEAR;
				ast_node_append(ast, ast_node_manage_alloc(), 0); //NAME
				ast_node_append(ast, ast_node_manage_alloc(), 1); //TYPE

				ast->nodes[0]->type = AST_TYPE_NAME;
				ast->nodes[1]->type = AST_TYPE_VAR_TYPE;

				strcpy(ast->nodes[0]->data, tk[i + 1].name);
				strcpy(ast->nodes[1]->data, tk[i + 3].name);
			}
		}
		i += 1;
	}
}