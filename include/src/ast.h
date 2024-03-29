#pragma once

#include <stdint.h>
#include <src/token.h>

#define AST_TYPE_UNDEFINED		0
#define AST_TYPE_PROGRAM		1
#define AST_TYPE_IDENTIFIER		2
#define AST_TYPE_VAR_DECLARE	3
#define AST_TYPE_VAR_TYPE		4
#define AST_TYPE_VAR_SET_VALUE	5
#define AST_TYPE_VAR_GET_VALUE	6
#define AST_TYPE_FUNC_DEF		7
#define AST_TYPE_FUNC_CALL		8
#define AST_TYPE_CODE_BLOCK		9
#define AST_TYPE_PARAMS			10
#define AST_TYPE_IF				11
#define AST_TYPE_IFELSE			12
#define AST_TYPE_ELSE			13
#define AST_TYPE_FOR			14
#define AST_TYPE_WHILE			15
#define AST_TYPE_ADD			16 // +
#define AST_TYPE_SUB			17 // -
#define AST_TYPE_MUL			18 // *
#define AST_TYPE_DIV			19 // /
#define AST_TYPE_GR				20 // >
#define AST_TYPE_LE				21 // <
#define AST_TYPE_GREQU			22 // >=
#define AST_TYPE_LEEQU			23 // >=
#define AST_TYPE_EQU			24 // ==
#define AST_TYPE_NUMBER			25
#define AST_TYPE_STRING			26
#define AST_TYPE_BREAK			27
#define AST_TYPE_RETURN			28
#define AST_SPLIT_REMOVABLE		100 //will be removed on returning

/* AST node */
typedef struct ast_node
{
	int8_t type;
	char data[20];
	struct ast_node *nodes[100];
} AST_NODE;

void ast_node_init(AST_NODE *);
void ast_node_manage_init(void);
int ast_build(AST_NODE *, const TOKEN *);
