#define TOKEN_TYPE_UNKOWN 0
#define TOKEN_TYPE_NAME 1
#define TOKEN_TYPE_KEYWORD 2
#define TOKEN_TYPE_EQU 3
#define TOKEN_TYPE_EXPLAIN 4
#define TOKEN_TYPE_LS_BKT 5 // (
#define TOKEN_TYPE_LM_BKT 6 // [
#define TOKEN_TYPE_LL_BKT 7 // {
#define TOKEN_TYPE_RS_BKT 8 // )
#define TOKEN_TYPE_RM_BKT 9 // ]
#define TOKEN_TYPE_RL_BKT 10 // }
#define TOKEN_TYPE_ADD 11
#define TOKEN_TYPE_SUB 12
#define TOKEN_TYPE_MUL 13
#define TOKEN_TYPE_DIV 14
#define TOKEN_TYPE_NUMBER 15
#define TOKEN_TYPE_SPLIT 16

struct token
{
	int type;
	char name[20];
};

typedef struct token TOKEN;