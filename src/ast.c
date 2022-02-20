#include <stdlib.h>
#include <string.h>
#include <src/ast.h>

struct ast_node *ast_nodes;
int ast_node_buf_size = 1024;

/* 初始化AST node */
void ast_node_init(AST_NODE *ast)
{
	ast->type = 0;

	for(int i = 0; i < 100; i++)
	{
		ast->nodes[i] = 0;
	}
}
/* 初始化AST管理 */
void ast_node_manage_init(void)
{
	ast_nodes = malloc(sizeof(AST_NODE) * ast_node_buf_size);
	for(int i = 0; i < ast_node_buf_size; i++)
	{
		ast_nodes[i].type = -1;
	}
}
/* 分配一个AST node */
AST_NODE* ast_node_manage_alloc(void)
{
	int i = 0;
	_ast_node_manage_alloc_start:
	for(; i < ast_node_buf_size; i++)
	{
		if(ast_nodes[i].type == -1)
		{
			ast_node_init(&ast_nodes[i]);
			return &(ast_nodes[i]);
		}
	}
	/* ast_nodes满了, 重新分配内存 */
	ast_node_buf_size += 1024;
	ast_nodes = realloc(ast_nodes, ast_node_buf_size);
	goto _ast_node_manage_alloc_start;
}
/* 释放AST node */
void ast_node_manage_free(AST_NODE *node)
{
	for(int i = 0; i < ast_node_buf_size; i++)
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
int ast_tree_build(AST_NODE *top_ast, TOKEN tk[])
{
	int token_index = 0;
	int ast_index = 0;
	AST_NODE *ast;
	ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
	ast = top_ast->nodes[ast_index];
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
		else if(tk[token_index].type == TOKEN_TYPE_NAME || tk[token_index].type == TOKEN_TYPE_NUMBER)
		{
			/* 函数调用 */
			if(tk[token_index + 1].type == TOKEN_TYPE_LS_BKT)
			{
				ast->type = AST_TYPE_FUNC_CALL;
				ast_node_append(ast, ast_node_manage_alloc(), 0); //name
				ast_node_append(ast, ast_node_manage_alloc(), 1); //params
				ast->nodes[0]->type = AST_TYPE_NAME;
				ast->nodes[1]->type = AST_TYPE_PARAMS;

				strcpy(ast->nodes[0]->data, tk[token_index].name);
			}
			/* 四则运算 */
			else if(tk[token_index + 1].type == TOKEN_TYPE_ADD ||
				tk[token_index + 1].type == TOKEN_TYPE_SUB ||
				tk[token_index + 1].type == TOKEN_TYPE_MUL ||
				tk[token_index + 1].type == TOKEN_TYPE_DIV)
			{
				ast->type = AST_TYPE_VAR_GET_VALUE;
				ast_node_append(ast, ast_node_manage_alloc(), 0);
				ast_node_append(ast, ast_node_manage_alloc(), 1);
				ast_node_append(ast, ast_node_manage_alloc(), 2);

				/* 前操作数 */
				if(tk[token_index].type == TOKEN_TYPE_NAME)
				{
					ast->nodes[0]->type = AST_TYPE_NAME;
				}
				else if(tk[token_index].type == TOKEN_TYPE_NUMBER)
				{
					ast->nodes[0]->type = AST_TYPE_NUMBER;
				}

				strcpy(ast->nodes[0]->data, tk[token_index].name);
				strcpy(ast->nodes[2]->data, tk[token_index + 2].name);
				/* 后操作数 */
				if(tk[token_index + 2].type == TOKEN_TYPE_NAME)
				{
					ast->nodes[2]->type = AST_TYPE_NAME;
				}
				else if(tk[token_index + 2].type == TOKEN_TYPE_NUMBER)
				{
					ast->nodes[2]->type = AST_TYPE_NUMBER;
				}

				/* 运算符 */
				if(tk[token_index + 1].type == TOKEN_TYPE_ADD)
				{
					ast->nodes[1]->type = AST_TYPE_ADD;
				}
				else if(tk[token_index + 1].type == TOKEN_TYPE_SUB)
				{
					ast->nodes[1]->type = AST_TYPE_SUB;
				}
				else if(tk[token_index + 1].type == TOKEN_TYPE_MUL)
				{
					ast->nodes[1]->type = AST_TYPE_MUL;
				}
				else
				{
					ast->nodes[1]->type = AST_TYPE_DIV;
				}
			}
			else if(tk[token_index + 1].type == TOKEN_TYPE_ISEQU)
			{
				ast->type = AST_TYPE_VAR_COMPARE;
				ast_node_append(ast, ast_node_manage_alloc(), 0);
				ast_node_append(ast, ast_node_manage_alloc(), 1);
				ast_node_append(ast, ast_node_manage_alloc(), 2);

				strcpy(ast->nodes[0]->data, tk[token_index].name);
				strcpy(ast->nodes[2]->data, tk[token_index + 2].name);
				/* 前操作数 */
				if(tk[token_index].type == TOKEN_TYPE_NAME)
				{
					ast->nodes[0]->type = AST_TYPE_NAME;
				}
				else if(tk[token_index].type == TOKEN_TYPE_NUMBER)
				{
					ast->nodes[0]->type = AST_TYPE_NUMBER;
				}

				/* 后操作数 */
				if(tk[token_index + 2].type == TOKEN_TYPE_NAME)
				{
					ast->nodes[2]->type = AST_TYPE_NAME;
				}
				else if(tk[token_index + 2].type == TOKEN_TYPE_NUMBER)
				{
					ast->nodes[2]->type = AST_TYPE_NUMBER;
				}

				/* 运算符 */
				if(tk[token_index + 1].type == TOKEN_TYPE_ISEQU)
				{
					ast->nodes[1]->type = AST_TYPE_EQU;
				}
			}
		}
		else if(tk[token_index].type == TOKEN_TYPE_SPLIT)
		{
			ast_index++;
			ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
			ast = top_ast->nodes[ast_index];
		}
		/* ( */
		else if(tk[token_index].type == TOKEN_TYPE_LS_BKT)
		{
			if(ast->type == AST_TYPE_FUNC_DEF || ast->type == AST_TYPE_FUNC_CALL)
			{
				/* ast->nodes[1] is params block */
				token_index += ast_tree_build(ast->nodes[1], &tk[token_index]);
				
			}
			else if(ast->type == AST_TYPE_IF)
			{
				/* ast->nodes[0] is expression block */
				token_index += ast_tree_build(ast->nodes[0], &tk[token_index]);
			}
		}
		/* { */
		else if(tk[token_index].type == TOKEN_TYPE_LL_BKT)
		{
			if(ast->type == AST_TYPE_FUNC_DEF)
			{
				/* ast->nodes[2] is code block */
				token_index += ast_tree_build(ast->nodes[2], &tk[token_index]);
				ast_index++;
				ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
				ast = top_ast->nodes[ast_index];
			}
			else if(ast->type == AST_TYPE_IF)
			{
				/* ast->nodes[1] is code block */
				token_index += ast_tree_build(ast->nodes[1], &tk[token_index]);
				ast_index++;
				ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
				ast = top_ast->nodes[ast_index];
			}
		}
		/* right brackets */
		else if(tk[token_index].type == TOKEN_TYPE_RS_BKT ||
		tk[token_index].type == TOKEN_TYPE_RM_BKT ||
		tk[token_index].type == TOKEN_TYPE_RL_BKT)
		{
			return token_index;
		}
		token_index += 1;
	}
}
