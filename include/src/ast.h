#include <src/token.h>

#define AST_TYPE_UNDEFINED 0
#define AST_TYPE_PROGRAM 1
#define AST_TYPE_VARIABLE_DECLARING 2
#define AST_TYPE_VARIABLE_NAME 3
#define AST_TYPE_VARIABLE_VALUE 5
#define AST_TYPE_DEFFUNC 6
#define AST_TYPE_CODE_BLOCK 7
#define AST_TYPE_PARAMS 8


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