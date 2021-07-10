#define TOKEN_TYPE_KEYWORD 1
#define TOKEN_TYPE_EQU 2
#define TOKEN_TYPE_EXPLAIN 3
#define TOKEN_TYPE_LS_BKT 4 // (
#define TOKEN_TYPE_LM_BKT 5 // [
#define TOKEN_TYPE_LL_BKT 6 //{
#define TOKEN_TYPE_RS_BKT 7 // )
#define TOKEN_TYPE_RM_BKT 8 // ]
#define TOKEN_TYPE_RL_BKT 9 // }

struct token
{
	int type;
	char name[20];
};