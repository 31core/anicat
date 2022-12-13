#include <stdlib.h>
#include <string.h>
#include <src/ast.h>

struct ast_node *ast_nodes;
int ast_node_buf_size = 1024;

/* initialize AST node */
void ast_node_init(AST_NODE *ast)
{
	ast->type = AST_TYPE_UNDEFINED;
	ast->data[0] = '\0';

	for(int i = 0; i < 100; i++)
	{
		ast->nodes[i] = NULL;
	}
}
/* initialize AST nodes management */
void ast_node_manage_init(void)
{
	ast_nodes = malloc(sizeof(AST_NODE) * ast_node_buf_size);
	for(int i = 0; i < ast_node_buf_size; i++)
	{
		ast_nodes[i].type = -1;
	}
}
/* alloc AST node */
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
/* release AST node */
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
/* append a node */
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
/* build an AST from tokens */
int ast_build(AST_NODE *top_ast, const TOKEN *tk)
{
	int token_index = 0;
	int ast_index = 0;
	ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
	while(tk[token_index].type != TOKEN_TYPE_UNKOWN)
	{
		/* keywords */
		if(tk[token_index].type == TOKEN_TYPE_KEYWORD)
		{
			/* function definition */
			if(!strcmp(tk[token_index].name, "func"))
			{
				CURRENT_NODE->type = AST_TYPE_FUNC_DEF;
			}
			/* variable declaration */
			else if(!strcmp(tk[token_index].name, "var"))
			{
				CURRENT_NODE->type = AST_TYPE_VAR_DECLARE;
			}
			/* if expression */
			else if(!strcmp(tk[token_index].name, "if"))
			{
				CURRENT_NODE->type = AST_TYPE_IF;
			}
			/* while expression */
			else if(!strcmp(tk[token_index].name, "while"))
			{
				CURRENT_NODE->type = AST_TYPE_WHILE;
			}
			/* for expression */
			else if(!strcmp(tk[token_index].name, "for"))
			{
				CURRENT_NODE->type = AST_TYPE_FOR;
			}
		}
		/* indentifiers, numbers or strings will be added as child nodes of the top node */
		else if(tk[token_index].type == TOKEN_TYPE_NAME ||
			tk[token_index].type == TOKEN_TYPE_NUMBER ||
			tk[token_index].type == TOKEN_TYPE_STRING)
		{
			if(tk[token_index].type == TOKEN_TYPE_NAME)
			{
				CURRENT_NODE->type = AST_TYPE_IDENTIFIER;
			}
			else if(tk[token_index].type == TOKEN_TYPE_NUMBER)
			{
				CURRENT_NODE->type = AST_TYPE_NUMBER;
			}
			else if(tk[token_index].type == TOKEN_TYPE_STRING)
			{
				CURRENT_NODE->type = AST_TYPE_STRING;
			}
			
			strcpy(CURRENT_NODE->data, tk[token_index].name);
			if(IS_LAST_NODE && LAST_NODE->type == AST_TYPE_FUNC_DEF)
			{
				LAST_NODE->nodes[0] = CURRENT_NODE;
				ast_index--;
			}
			else if(IS_LAST_NODE && LAST_NODE->type == AST_TYPE_VAR_DECLARE)
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
				if(LAST_NODE->nodes[1] != NULL && LAST_NODE->nodes[1]->nodes[1] != NULL &&  (LAST_NODE->nodes[1]->nodes[1]->type == AST_TYPE_MUL ||
					LAST_NODE->nodes[1]->nodes[1]->type == AST_TYPE_DIV))
				{
					LAST_NODE->nodes[1]->nodes[1]->nodes[1] = CURRENT_NODE;
				}
				else if(LAST_NODE->nodes[1] != NULL && (LAST_NODE->nodes[1]->type == AST_TYPE_ADD ||
					LAST_NODE->nodes[1]->type == AST_TYPE_SUB))
				{
					LAST_NODE->nodes[1]->nodes[1] = CURRENT_NODE;
				}
				else
				{
					LAST_NODE->nodes[1] = CURRENT_NODE;
				}
				ast_index--;
			}
			else if(IS_LAST_NODE && LAST_NODE->nodes[1] != NULL && (LAST_NODE->nodes[1]->type == AST_TYPE_MUL ||
				LAST_NODE->nodes[1]->type == AST_TYPE_DIV))
			{
				LAST_NODE->nodes[1]->nodes[1] = CURRENT_NODE;
			}
			else if(IS_LAST_NODE && (LAST_NODE->type == AST_TYPE_ADD ||
				LAST_NODE->type == AST_TYPE_SUB ||
				LAST_NODE->type == AST_TYPE_EQU ||
				LAST_NODE->type == AST_TYPE_LE ||
				LAST_NODE->type == AST_TYPE_GR ||
				LAST_NODE->type == AST_TYPE_LEEQU ||
				LAST_NODE->type == AST_TYPE_GREQU))
			{
				LAST_NODE->nodes[1] = CURRENT_NODE;
				ast_index--;
			}
		}
		else if(tk[token_index].type == TOKEN_TYPE_LS_BKT)
		{
			/* * function definition
			   * if expression */
			if(IS_LAST_NODE && (LAST_NODE->type == AST_TYPE_FUNC_DEF || LAST_NODE->type == AST_TYPE_IF ||
				LAST_NODE->type == AST_TYPE_WHILE || LAST_NODE->type == AST_TYPE_FOR))
			{
				CURRENT_NODE->type = AST_TYPE_PARAMS;
				if(LAST_NODE->type == AST_TYPE_FUNC_DEF)
				{
					LAST_NODE->nodes[1] = CURRENT_NODE;
				}
				else if(LAST_NODE->type == AST_TYPE_IF || LAST_NODE->type == AST_TYPE_WHILE || LAST_NODE->type == AST_TYPE_FOR)
				{
					LAST_NODE->nodes[0] = CURRENT_NODE;
				}
				token_index += ast_build(CURRENT_NODE, &tk[token_index + 1]);
				ast_index--;
			}
			/* function call */
			else if(IS_LAST_NODE && LAST_NODE->type == AST_TYPE_IDENTIFIER)
			{
				CURRENT_NODE->type = AST_TYPE_FUNC_CALL;
				CURRENT_NODE->nodes[0] = LAST_NODE;
				LAST_NODE = CURRENT_NODE;

				ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
				LAST_NODE->nodes[1] = CURRENT_NODE;
				CURRENT_NODE->type = AST_TYPE_PARAMS;
				token_index += ast_build(CURRENT_NODE, &tk[token_index + 1]);
				ast_index--;
			}
		}
		/* code block such as 'if(){}' and func (){} */
		else if(tk[token_index].type == TOKEN_TYPE_LL_BKT)
		{
			if(IS_LAST_NODE && (LAST_NODE->type == AST_TYPE_FUNC_DEF || LAST_NODE->type == AST_TYPE_IF ||
				LAST_NODE->type == AST_TYPE_WHILE || LAST_NODE->type == AST_TYPE_FOR))
			{
				CURRENT_NODE->type = AST_TYPE_CODE_BLOCK;
				if(LAST_NODE->type == AST_TYPE_FUNC_DEF)
				{
					LAST_NODE->nodes[2] = CURRENT_NODE;
				}
				else if(LAST_NODE->type == AST_TYPE_IF || LAST_NODE->type == AST_TYPE_WHILE || LAST_NODE->type == AST_TYPE_FOR)
				{
					LAST_NODE->nodes[1] = CURRENT_NODE;
				}
				token_index += ast_build(CURRENT_NODE, &tk[token_index + 1]);
				ast_index--;
			}
		}
		else if(tk[token_index].type == TOKEN_TYPE_EXPLAIN)
		{
			if(LAST_NODE->type == AST_TYPE_VAR_DECLARE)
			{
				CURRENT_NODE->type = AST_TYPE_VAR_TYPE;
				LAST_NODE->nodes[1] = CURRENT_NODE;
			}
			else if(LAST_NODE->type == AST_TYPE_IDENTIFIER)
			{
				CURRENT_NODE->type = AST_TYPE_VAR_DECLARE;
				CURRENT_NODE->nodes[0] = LAST_NODE;
				LAST_NODE = CURRENT_NODE;

				ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
				CURRENT_NODE->type = AST_TYPE_VAR_TYPE;
				LAST_NODE->nodes[1] = CURRENT_NODE;
			}
		}
		/* operations */
		else if(tk[token_index].type == TOKEN_TYPE_ADD ||
			tk[token_index].type == TOKEN_TYPE_SUB ||
			tk[token_index].type == TOKEN_TYPE_MUL ||
			tk[token_index].type == TOKEN_TYPE_DIV ||
			tk[token_index].type == TOKEN_TYPE_LESS ||
			tk[token_index].type == TOKEN_TYPE_GREATER ||
			tk[token_index].type == TOKEN_TYPE_ISEQU ||
			tk[token_index].type == TOKEN_TYPE_LEEQU ||
			tk[token_index].type == TOKEN_TYPE_GREQU)
		{
			/* detect symbol */
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
			else if(tk[token_index].type == TOKEN_TYPE_DIV)
			{
				CURRENT_NODE->type = AST_TYPE_DIV;
			}
			else if(tk[token_index].type == TOKEN_TYPE_LESS)
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

			AST_NODE **last_node;
			if(LAST_NODE->type == AST_TYPE_VAR_SET_VALUE)
			{
				last_node = &LAST_NODE->nodes[1];
			}
			else
			{
				last_node = &LAST_NODE;
			}
			if(CURRENT_NODE->type == AST_TYPE_MUL && (*last_node)->type == AST_TYPE_ADD)
			{
				CURRENT_NODE->nodes[0] = (*last_node)->nodes[1];
				(*last_node)->nodes[1] = CURRENT_NODE;
			}
			else
			{
				CURRENT_NODE->nodes[0] = *last_node;
				*last_node = CURRENT_NODE;
			}

			ast_index--;
		}
		/* i = x */
		else if(tk[token_index].type == TOKEN_TYPE_EQU)
		{
			CURRENT_NODE->type = AST_TYPE_VAR_SET_VALUE;
			CURRENT_NODE->nodes[0] = LAST_NODE;
			LAST_NODE = CURRENT_NODE;
			ast_index--;
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
