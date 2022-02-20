#include <src/ast.h>

#define VAR_TYPE_INT 	1
#define VAR_TYPE_BOOL	2
#define VAR_TYPE_STRING	3

struct variable
{
	char name[20];
	int type;
};

extern struct variable variables[1024];
extern int var_count;

void register_variable(AST_NODE*);
