#include <stdlib.h>
#include <string.h>
#include <src/ast.h>

AST_NODE *ast_nodes;
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

/* merge '+' '-' '*' '/' nodes */
static void merge_op(AST_NODE *top_ast, int *ast_index)
{
	int i = 1;
	int deleted = 0;
	while(i <= *ast_index && top_ast->nodes[*ast_index - i]->type != AST_SPLIT_REMOVABLE)
	{
		if(top_ast->nodes[*ast_index - i]->type == AST_TYPE_ADD ||
			top_ast->nodes[*ast_index - i]->type == AST_TYPE_SUB ||
			top_ast->nodes[*ast_index - i]->type == AST_TYPE_MUL ||
			top_ast->nodes[*ast_index - i]->type == AST_TYPE_DIV)
		{
			if(top_ast->nodes[*ast_index - i + 1]->type == AST_TYPE_ADD || top_ast->nodes[*ast_index - i + 1]->type == AST_TYPE_SUB)
			{
				top_ast->nodes[*ast_index - i]->nodes[1] = top_ast->nodes[*ast_index - i + 1]->nodes[0];
				top_ast->nodes[*ast_index - i + 1]->nodes[0] = top_ast->nodes[*ast_index - i];
				top_ast->nodes[*ast_index - i] = top_ast->nodes[*ast_index - i + 1];
				for(int j = i; j > 1; j--)
				{
					top_ast->nodes[*ast_index - j + 1] = top_ast->nodes[*ast_index - j + 2];
				}
				deleted++;
			}
			else if(top_ast->nodes[*ast_index - i + 1]->type == AST_TYPE_MUL || top_ast->nodes[*ast_index - i + 1]->type == AST_TYPE_DIV)
			{
				if(top_ast->nodes[*ast_index - i]->type == AST_TYPE_ADD || top_ast->nodes[*ast_index - i]->type == AST_TYPE_SUB)
				{
					top_ast->nodes[*ast_index - i]->nodes[1] = top_ast->nodes[*ast_index - i + 1];
				}
				else
				{
					top_ast->nodes[*ast_index - i]->nodes[1] = top_ast->nodes[*ast_index - i + 1]->nodes[0];
					top_ast->nodes[*ast_index - i + 1]->nodes[0] = top_ast->nodes[*ast_index - i];
					top_ast->nodes[*ast_index - i] = top_ast->nodes[*ast_index - i + 1];
				}
				for(int j = i; j > 1; j--)
				{
					top_ast->nodes[*ast_index - j + 1] = top_ast->nodes[*ast_index - j + 2];
				}
				deleted++;
			}
			else
			{
				top_ast->nodes[*ast_index - i]->nodes[1] = top_ast->nodes[*ast_index - i + 1];
				/* delete top_ast->nodes[ast_index - i + 1] */
				for(int j = i; j > 1; j--)
				{
					top_ast->nodes[*ast_index - j + 1] = top_ast->nodes[*ast_index - j + 2];
				}
				deleted++;
			}
		}
		i++;
	}
	*ast_index -= deleted;
}

/* merge '==' '>' '<' '>=' '<=' nodes */
static void merge_cmp(AST_NODE *top_ast, int *ast_index)
{
	int i = 1;
	int deleted = 0;
	while(i <= *ast_index && top_ast->nodes[*ast_index - i]->type != AST_SPLIT_REMOVABLE)
	{
		if(top_ast->nodes[*ast_index - i]->type == AST_TYPE_EQU ||
			top_ast->nodes[*ast_index - i]->type == AST_TYPE_GR ||
			top_ast->nodes[*ast_index - i]->type == AST_TYPE_GREQU ||
			top_ast->nodes[*ast_index - i]->type == AST_TYPE_LE ||
			top_ast->nodes[*ast_index - i]->type == AST_TYPE_LEEQU)
		{
			top_ast->nodes[*ast_index - i]->nodes[0] = top_ast->nodes[*ast_index - i - 1];
			top_ast->nodes[*ast_index - i]->nodes[1] = top_ast->nodes[*ast_index - i + 1];
			top_ast->nodes[*ast_index - i - 1] = top_ast->nodes[*ast_index - i];
			/* delete top_ast->nodes[ast_index - i] and [ast_index - i + 1] */
			for(int j = i; j > 1; j--)
			{
				top_ast->nodes[*ast_index - j] = top_ast->nodes[*ast_index - j + 2];
			}
			deleted += 2;
			i++;
		}
		i++;
	}
	*ast_index -= deleted;
}

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
			else if(!strcmp(tk[token_index].name, "break"))
			{
				CURRENT_NODE->type = AST_TYPE_BREAK;
			}
			else if(!strcmp(tk[token_index].name, "return"))
			{
				CURRENT_NODE->type = AST_TYPE_RETURN;
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
			else if(LAST_NODE->type == AST_TYPE_FUNC_DEF)
			{
				CURRENT_NODE->type = AST_TYPE_VAR_TYPE;
				LAST_NODE->nodes[3] = CURRENT_NODE;
			}
		}
		/* operations */
		else if(tk[token_index].type == TOKEN_TYPE_ADD ||
			tk[token_index].type == TOKEN_TYPE_SUB ||
			tk[token_index].type == TOKEN_TYPE_MUL ||
			tk[token_index].type == TOKEN_TYPE_DIV)
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

			CURRENT_NODE->nodes[0] = LAST_NODE;
			LAST_NODE = CURRENT_NODE;

			ast_index--;
		}
		else if(tk[token_index].type == TOKEN_TYPE_LESS ||
			tk[token_index].type == TOKEN_TYPE_GREATER ||
			tk[token_index].type == TOKEN_TYPE_ISEQU ||
			tk[token_index].type == TOKEN_TYPE_LEEQU ||
			tk[token_index].type == TOKEN_TYPE_GREQU)
		{
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
			merge_op(top_ast, &ast_index);
			merge_cmp(top_ast, &ast_index);
			/* remove AST_SPLIT_REMOVABLE nodes */
			int i = 1;
			while(top_ast->nodes[2 * i + 1] != NULL)
			{
				top_ast->nodes[i] = top_ast->nodes[2 * i];
				i++;
			}
			top_ast->nodes[i] = NULL;
			return token_index + 1;
		}
		/* ',' or ';' */
		else if(tk[token_index].type == TOKEN_TYPE_SPLIT)
		{
			CURRENT_NODE->type = AST_SPLIT_REMOVABLE;
			merge_op(top_ast, &ast_index);
			merge_cmp(top_ast, &ast_index);
			if(ast_index >= 2 && (top_ast->nodes[ast_index - 2]->type == AST_TYPE_VAR_SET_VALUE))
			{
				top_ast->nodes[ast_index - 2]->nodes[1] = LAST_NODE;
				LAST_NODE = CURRENT_NODE;
				ast_index--;
			}
			else if(ast_index >= 2 && (top_ast->nodes[ast_index - 2]->type == AST_TYPE_RETURN))
			{
				top_ast->nodes[ast_index - 2]->nodes[0] = LAST_NODE;
				LAST_NODE = CURRENT_NODE;
				ast_index--;
			}
		}
		token_index++;
		ast_index++;
		ast_node_append(top_ast, ast_node_manage_alloc(), ast_index);
	}
	return 0;
}
