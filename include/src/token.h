#pragma once

#include <stdint.h>

#define TOKEN_TYPE_UNKOWN	0
#define TOKEN_TYPE_NAME		1
#define TOKEN_TYPE_KEYWORD	2
#define TOKEN_TYPE_EQU		3 // =
#define TOKEN_TYPE_EXPLAIN	4 // :
#define TOKEN_TYPE_LS_BKT	5 // (
#define TOKEN_TYPE_LM_BKT	6 // [
#define TOKEN_TYPE_LL_BKT	7 // {
#define TOKEN_TYPE_RS_BKT	8 // )
#define TOKEN_TYPE_RM_BKT	9 // ]
#define TOKEN_TYPE_RL_BKT	10 // }
#define TOKEN_TYPE_ADD		11 // +
#define TOKEN_TYPE_SUB		12 // -
#define TOKEN_TYPE_MUL		13 // *
#define TOKEN_TYPE_DIV		14 // /
#define TOKEN_TYPE_GREATER	15 // /
#define TOKEN_TYPE_LESS		16 // /
#define TOKEN_TYPE_NUMBER	17
#define TOKEN_TYPE_SPLIT	18
#define TOKEN_TYPE_ISEQU	19 // ==
#define TOKEN_TYPE_GREQU	20 // ==
#define TOKEN_TYPE_LEEQU	21 // ==
#define TOKEN_TYPE_STRING	22

typedef struct token
{
	uint8_t type;
	char name[20];
} TOKEN;

void generate_token(TOKEN *, const char *);
