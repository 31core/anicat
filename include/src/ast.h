#include <src/token.h>

#define AST_TYPE_UNDEFINED		0
#define AST_TYPE_PROGRAM		1
#define AST_TYPE_VAR			2
#define AST_TYPE_VAR_NAME		3
#define AST_TYPE_VAR_TYPE		4
#define AST_TYPE_VAR_SET_VALUE	5
#define AST_TYPE_VAR_COMPARE	6
#define AST_TYPE_VAR_VALUE		7
#define AST_TYPE_FUNC_DEF		8
#define AST_TYPE_FUNC_NAME		9
#define AST_TYPE_FUNC_CALL		10
#define AST_TYPE_CODE_BLOCK		11
#define AST_TYPE_PARAMS			12

/* AST节点 */
struct ast_node
{
	int type;
	char data[20];
	struct ast_node *nodes[100];
};

void ast_node_init(struct ast_node*);
void ast_node_manage_init();
void ast_tree_build(struct ast_node*, struct token*);