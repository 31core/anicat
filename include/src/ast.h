#include <src/ast_type.h>
#include <src/token.h>

/* AST节点 */
struct ast_node
{
	int type;
	char name[20];
	struct ast_node *nodes[100];
};

void ast_node_init(struct ast_node*);
void ast_node_manage_init();
void ast_tree_build(struct ast_node*, struct token*, int);