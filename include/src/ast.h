#pragma once
#include <src/token.h>

#define AST_TYPE_UNDEFINED		0
#define AST_TYPE_PROGRAM		1
#define AST_TYPE_IDENTIFIER		2
#define AST_TYPE_VAR_DECLEAR	3
#define AST_TYPE_VAR_TYPE		4
#define AST_TYPE_VAR_SET_VALUE	5
#define AST_TYPE_VAR_GET_VALUE	6
#define AST_TYPE_FUNC_DEF		7
#define AST_TYPE_FUNC_CALL		8
#define AST_TYPE_CODE_BLOCK		9
#define AST_TYPE_PARAMS			10
#define AST_TYPE_IF				11
#define AST_TYPE_EXPRESS		12
#define AST_TYPE_ADD			13 // +
#define AST_TYPE_SUB			14 // -
#define AST_TYPE_MUL			15 // *
#define AST_TYPE_DIV			16 // /
#define AST_TYPE_GR				17 // >
#define AST_TYPE_LE				18 // <
#define AST_TYPE_GREQU			19 // >=
#define AST_TYPE_LEEQU			20 // >=
#define AST_TYPE_EQU			21 // ==
#define AST_TYPE_NUMBER			22
#define AST_TYPE_STRING			23
#define AST_TYPE_VARIABLE		24
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
int ast_tree_build(AST_NODE*, TOKEN*);
