#pragma once
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
#define AST_TYPE_IF				12
#define AST_TYPE_EXPRESS		13
#define AST_TYPE_ADD			14 // +
#define AST_TYPE_SUB			15 // -
#define AST_TYPE_MUL			16 // *
#define AST_TYPE_DIV			17 // /
#define AST_TYPE_GR				18 // >
#define AST_TYPE_LE				19 // >
#define AST_TYPE_GREQU			20 // >
#define AST_TYPE_LEEQU			21 // >
#define AST_TYPE_EQU			22 // =
#define AST_TYPE_NUMBER			23

#define PRINT_TYPE(x, type) if(x == type) {printf("Type: "#type"\n");} 
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
