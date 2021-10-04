#include <string.h>
#include <src/ast.h>
//#include <src/token.h>

struct ast_node ast_nodes[1024];
struct ast_node *node_layer[100];

/* 初始化AST node */
void ast_node_init(struct ast_node *ast)
{
	ast->type = 0;
    int i = 0;
    for(; i < 100; i++)
    {
        ast->nodes[i] = 0;
    }
}

void ast_node_manage_init()
{
    int i = 0;
    for(; i < 1024; i++)
    {
        ast_nodes[i].type = -1;
    }
}

struct ast_node* ast_node_manage_alloc()
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

void ast_node_manage_free(struct ast_node *node)
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
void ast_node_append(struct ast_node *ast, struct ast_node *target, int p)
{
    ast->nodes[p] = target;
    ast->nodes[p + 1] = 0;
}
/* 组建AST树 */
void ast_tree_build(struct ast_node *ast, struct token tk[], int tk_count)
{
	int i = 0;
	for(; i < tk_count; i++)
	{
        /* 函数定义 */
		if(tk[i].type == TOKEN_TYPE_KEYWORD && strcmp(tk[i].name, "func") == 0)
		{
			ast->type = AST_TYPE_DEFFUNC;
			strcpy(ast->name, tk[i + 1].name);
            /* 分配一个PARAMS */
            ast_node_append(ast, ast_node_manage_alloc(), 0);
            ast->nodes[0]->type = AST_TYPE_PARAMS;
            /* 分配一个CODE_BLOCK */
            ast_node_append(ast, ast_node_manage_alloc(), 1);
            ast->nodes[1]->type = AST_TYPE_CODE_BLOCK;
			i += 1;
		}
	}
}