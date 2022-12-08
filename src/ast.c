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
	ast->nodes[p + 1] = NULL;
}

struct layer
{
	AST_NODE *node;
	int potition;
};

#define CURRENT_NODE top_ast->nodes[ast_index]
#define LAST_NODE top_ast->nodes[ast_index - 1]
#define IS_LAST_NODE ast_index > 0
/* 组建AST树 */
int ast_tree_build(AST_NODE *top_ast, TOKEN tk[])
{
	int token_index = 0;
	int ast_index = 0;
	ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
	while(tk[token_index].type != TOKEN_TYPE_UNKOWN)
	{
		/* keywords */
		if(tk[token_index].type == TOKEN_TYPE_KEYWORD)
		{
			/* 函数定义 */
			if(!strcmp(tk[token_index].name, "func"))
			{
				CURRENT_NODE->type = AST_TYPE_FUNC_DEF;
			}
			/* 变量声明 */
			else if(!strcmp(tk[token_index].name, "var"))
			{
				CURRENT_NODE->type = AST_TYPE_VAR_DECLEAR;
			}
			/* if语句 */
			else if(!strcmp(tk[token_index].name, "if"))
			{
				CURRENT_NODE->type = AST_TYPE_IF;
			}
		}
		else if(tk[token_index].type == TOKEN_TYPE_NAME ||
			tk[token_index].type == TOKEN_TYPE_NUMBER ||
			tk[token_index].type == TOKEN_TYPE_STRING)
		{
			CURRENT_NODE->type = AST_TYPE_IDENTIFIER;
			strcpy(CURRENT_NODE->data, tk[token_index].name);
			if(IS_LAST_NODE && LAST_NODE->type == AST_TYPE_FUNC_DEF)
			{
				LAST_NODE->nodes[0] = CURRENT_NODE;
				ast_index--;
			}
			else if(IS_LAST_NODE && LAST_NODE->type == AST_TYPE_VAR_DECLEAR)
			{
				LAST_NODE->nodes[0] = CURRENT_NODE;
				ast_index--;
			}
			else if(IS_LAST_NODE && LAST_NODE->type == AST_TYPE_VAR_TYPE)
			{
				LAST_NODE->nodes[0] = CURRENT_NODE;
				ast_index -= 2;
			}
			else if(IS_LAST_NODE && LAST_NODE->type == AST_TYPE_VAR_SET_VALUE)
			{
				LAST_NODE->nodes[1] = CURRENT_NODE;
				ast_index--;
			}
			else if(IS_LAST_NODE && (LAST_NODE->type == AST_TYPE_ADD ||
				LAST_NODE->type == AST_TYPE_SUB))
			{
				LAST_NODE->nodes[1] = CURRENT_NODE;
				ast_index--;
			}
		}
		else if(tk[token_index].type == TOKEN_TYPE_LS_BKT)
		{
			if(IS_LAST_NODE && (LAST_NODE->type == AST_TYPE_FUNC_DEF || LAST_NODE->type == AST_TYPE_IF))
			{
				CURRENT_NODE->type = AST_TYPE_PARAMS;
				if(LAST_NODE->type == AST_TYPE_FUNC_DEF)
				{
					LAST_NODE->nodes[1] = CURRENT_NODE;
				}
				else if(LAST_NODE->type == AST_TYPE_IF)
				{
					LAST_NODE->nodes[0] = CURRENT_NODE;
				}
				token_index += ast_tree_build(CURRENT_NODE, &tk[token_index + 1]);
				ast_index--;
			}
			else if(IS_LAST_NODE && LAST_NODE->type == AST_TYPE_IDENTIFIER)
			{
				CURRENT_NODE->type = AST_TYPE_FUNC_CALL;
				CURRENT_NODE->nodes[0] = LAST_NODE;
				LAST_NODE = CURRENT_NODE;

				ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
				LAST_NODE->nodes[1] = CURRENT_NODE;
				CURRENT_NODE->type = AST_TYPE_PARAMS;
				token_index += ast_tree_build(CURRENT_NODE, &tk[token_index + 1]);
			}
		}
		else if(tk[token_index].type == TOKEN_TYPE_LL_BKT)
		{
			if(IS_LAST_NODE && (LAST_NODE->type == AST_TYPE_FUNC_DEF || LAST_NODE->type == AST_TYPE_IF))
			{
				CURRENT_NODE->type = AST_TYPE_CODE_BLOCK;
				if(LAST_NODE->type == AST_TYPE_FUNC_DEF)
				{
					LAST_NODE->nodes[2] = CURRENT_NODE;
				}
				else if(LAST_NODE->type == AST_TYPE_IF)
				{
					LAST_NODE->nodes[1] = CURRENT_NODE;
				}
				token_index += ast_tree_build(CURRENT_NODE, &tk[token_index + 1]);
				ast_index--;
			}
		}
		else if(tk[token_index].type == TOKEN_TYPE_EXPLAIN)
		{
			if(LAST_NODE->type == AST_TYPE_VAR_DECLEAR)
			{
				CURRENT_NODE->type = AST_TYPE_VAR_TYPE;
				LAST_NODE->nodes[1] = CURRENT_NODE;
				LAST_NODE = CURRENT_NODE;
			}
			else if(LAST_NODE->type == AST_TYPE_IDENTIFIER)
			{
				CURRENT_NODE->type = AST_TYPE_VAR_DECLEAR;
				CURRENT_NODE->nodes[0] = LAST_NODE;
				LAST_NODE = CURRENT_NODE;

				ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
				CURRENT_NODE->type = AST_TYPE_VAR_TYPE;
				LAST_NODE->nodes[1] = CURRENT_NODE;
			}
		}
		/* 四则运算 */
		else if(tk[token_index].type == TOKEN_TYPE_ADD ||
			tk[token_index].type == TOKEN_TYPE_SUB ||
			tk[token_index].type == TOKEN_TYPE_MUL ||
			tk[token_index].type == TOKEN_TYPE_DIV)
		{
			/* 运算符 */
			if(tk[token_index].type == TOKEN_TYPE_ADD)
			{
				CURRENT_NODE->type = AST_TYPE_ADD;
			}
			else if(tk[token_index].type == TOKEN_TYPE_SUB)
			{
				CURRENT_NODE->type = AST_TYPE_SUB;
			}
			else if(tk[token_index].type == TOKEN_TYPE_MUL)
			{
				CURRENT_NODE->type = AST_TYPE_MUL;
			}
			else
			{
				CURRENT_NODE->type = AST_TYPE_DIV;
			}
			if(IS_LAST_NODE && (LAST_NODE->type == AST_TYPE_IDENTIFIER ||
				LAST_NODE->type == AST_TYPE_NUMBER))
			{
				CURRENT_NODE->nodes[0] = LAST_NODE;
			}
			else
			{
				CURRENT_NODE->nodes[0] = LAST_NODE->nodes[1];
			}
			//LAST_NODE = CURRENT_NODE;
			//ast_index--;
		}
		/* i = x */
		else if(tk[token_index].type == TOKEN_TYPE_EQU)
		{
			CURRENT_NODE->type = AST_TYPE_VAR_SET_VALUE;
			CURRENT_NODE->nodes[0] = LAST_NODE;
			LAST_NODE = CURRENT_NODE;
			ast_index--;
		}
		/* <, >, ==, <=, >= */
		else if(tk[token_index].type == TOKEN_TYPE_LESS ||
			tk[token_index].type == TOKEN_TYPE_GREATER ||
			tk[token_index].type == TOKEN_TYPE_ISEQU ||
			tk[token_index].type == TOKEN_TYPE_LEEQU ||
			tk[token_index].type == TOKEN_TYPE_GREQU)
		{
			/* 运算符 */
			if(tk[token_index].type == TOKEN_TYPE_LESS)
			{
				CURRENT_NODE->type = AST_TYPE_LE;
			}
			else if(tk[token_index].type == TOKEN_TYPE_GREATER)
			{
				CURRENT_NODE->type = AST_TYPE_GR;
			}
			else if(tk[token_index].type == TOKEN_TYPE_ISEQU)
			{
				CURRENT_NODE->type = AST_TYPE_EQU;
			}
			else if(tk[token_index].type == TOKEN_TYPE_LEEQU)
			{
				CURRENT_NODE->type = AST_TYPE_LEEQU;
			}
			else
			{
				CURRENT_NODE->type = AST_TYPE_GREQU;
			}
		}
		/* right brackets */
		else if(tk[token_index].type == TOKEN_TYPE_RS_BKT ||
		tk[token_index].type == TOKEN_TYPE_RM_BKT ||
		tk[token_index].type == TOKEN_TYPE_RL_BKT)
		{
			top_ast->nodes[ast_index] = NULL;
			return token_index + 1;
		}
		token_index++;
		ast_index++;
		ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
	}
}
