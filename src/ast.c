#include <stdlib.h>
#include <string.h>
#include <src/ast.h>

struct ast_node *ast_nodes;
//struct ast_node *node_layer[100];

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
	for(int i = 0; i < 1024; i++)
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

struct layer
{
	AST_NODE *node;
	int potition;
};

/* 组建AST树 */
void ast_tree_build(AST_NODE *ast, TOKEN tk[])
{
	AST_NODE *node_layer[100];
	struct layer layers[100];
	int p = -1;
	int node_layer_position = 0;
	int token_index = 0;
	while(tk[token_index].type != TOKEN_TYPE_UNKOWN)
	{
		/* 函数定义 */
		if(tk[token_index].type == TOKEN_TYPE_KEYWORD)
		{
			/* 函数定义 */
			if(!strcmp(tk[token_index].name, "func"))
			{
				ast->type = AST_TYPE_FUNC_DEF;

				ast_node_append(ast, ast_node_manage_alloc(), 0);
				ast_node_append(ast, ast_node_manage_alloc(), 1);
				ast_node_append(ast, ast_node_manage_alloc(), 2);

				ast->nodes[0]->type = AST_TYPE_NAME;
				ast->nodes[1]->type = AST_TYPE_PARAMS;
				ast->nodes[2]->type = AST_TYPE_CODE_BLOCK;

				strcpy(ast->nodes[0]->data, tk[token_index + 1].name);
				token_index += 1;
			}
			/* 变量声明 */
			else if(!strcmp(tk[token_index].name, "var"))
			{
				ast->type = AST_TYPE_VAR_DECLEAR;
				ast_node_append(ast, ast_node_manage_alloc(), 0); //NAME
				ast_node_append(ast, ast_node_manage_alloc(), 1); //TYPE

				ast->nodes[0]->type = AST_TYPE_NAME;
				ast->nodes[1]->type = AST_TYPE_VAR_TYPE;

				strcpy(ast->nodes[0]->data, tk[token_index + 1].name);
				strcpy(ast->nodes[1]->data, tk[token_index + 3].name);
			}
			/* if语句 */
			else if(!strcmp(tk[token_index].name, "if"))
			{
				ast->type = AST_TYPE_IF;
				ast_node_append(ast, ast_node_manage_alloc(), 0); //Express
				ast_node_append(ast, ast_node_manage_alloc(), 1); //CODE BLOCK
				ast->nodes[0]->type = AST_TYPE_EXPRESS;
				ast->nodes[1]->type = AST_TYPE_CODE_BLOCK;
			}
		}
		/* 函数调用 */
		else if(tk[token_index].type == TOKEN_TYPE_NAME && tk[token_index + 1].type == TOKEN_TYPE_LS_BKT)
		{
			ast->type = AST_TYPE_FUNC_CALL;
			ast_node_append(ast, ast_node_manage_alloc(), 0); //name
			ast_node_append(ast, ast_node_manage_alloc(), 1); //params
			ast->nodes[0]->type = AST_TYPE_NAME;
			ast->nodes[1]->type = AST_TYPE_PARAMS;

			strcpy(ast->nodes[0]->data, tk[token_index].name);
		}
		else if(tk[token_index].type == TOKEN_TYPE_SPLIT)
		{
			ast_node_append(layers[p].node, ast_node_manage_alloc(), layers[p].potition);
			ast = layers[p].node->nodes[layers[p].potition];
			layers[p].potition += 1;
		}
		/* ( */
		else if(tk[token_index].type == TOKEN_TYPE_LS_BKT)
		{
			p += 1;
			node_layer[node_layer_position] = ast;
			node_layer_position += 1;
			if(ast->type == AST_TYPE_FUNC_DEF)
			{
				/* ast->nodes[1] is params block */
				layers[p].node = ast->nodes[1];
				layers[p].potition = 0;
				ast_node_append(ast->nodes[1], ast_node_manage_alloc(), 0);
				ast = ast->nodes[1]->nodes[0];
				layers[p].potition += 1;
			}
			else if(ast->type == AST_TYPE_IF)
			{
				/* ast->nodes[0] is params block */
				layers[p].node = ast->nodes[0];
				layers[p].potition = 0;
				ast_node_append(ast->nodes[0], ast_node_manage_alloc(), 0);
				ast = ast->nodes[0]->nodes[0];
				layers[p].potition += 1;
			}
		}
		/* { */
		else if(tk[token_index].type == TOKEN_TYPE_LL_BKT)
		{
			p += 1;
			node_layer[node_layer_position] = ast;
			node_layer_position += 1;
			if(ast->type == AST_TYPE_FUNC_DEF)
			{
				/* ast->nodes[2] is code block */
				layers[p].node = ast->nodes[2];
				layers[p].potition = 0;
				ast_node_append(ast->nodes[2], ast_node_manage_alloc(), 0);
				ast = ast->nodes[2]->nodes[0];
				layers[p].potition += 1;
			}
			else if(ast->type == AST_TYPE_IF)
			{
				/* ast->nodes[1] is code block */
				layers[p].node = ast->nodes[1];
				layers[p].potition = 0;
				ast_node_append(ast->nodes[1], ast_node_manage_alloc(), 0);
				ast = ast->nodes[1]->nodes[0];
				layers[p].potition += 1;
			}
		}
		/* right brackets */
		else if(tk[token_index].type == TOKEN_TYPE_RS_BKT ||
		tk[token_index].type == TOKEN_TYPE_RM_BKT ||
		tk[token_index].type == TOKEN_TYPE_RL_BKT)
		{
			p -= 1;
			node_layer_position -= 1;
			ast = node_layer[node_layer_position];
		}
		token_index += 1;
	}
}