#include <src/token.h>

#define AST_TYPE_UNDEFINED		0
#define AST_TYPE_PROGRAM		1
#define AST_TYPE_NAME			2
#define AST_TYPE_VAR_DECLEAR	3
#define AST_TYPE_VAR_TYPE		4
#define AST_TYPE_VAR_SET_VALUE	5
#define AST_TYPE_VAR_COMPARE	6
#define AST_TYPE_VAR_GET_VALUE	7
#define AST_TYPE_FUNC_DEF		8
#define AST_TYPE_FUNC_CALL		9
#define AST_TYPE_CODE_BLOCK		10
#define AST_TYPE_PARAMS			11

/* AST节点 */
struct ast_node
{
	int type;
	char data[20];
	struct ast_node *nodes[100];
};

typedef struct ast_node AST_NODE;

void ast_node_init(AST_NODE*);
void ast_node_manage_init(void);
void ast_tree_build(AST_NODE*, TOKEN*);