#include <src/ast.h>
#include <src/variable.h>
struct function
{
	char name[20];
	struct variable params[10];
	AST_NODE *code_block;
};

extern struct function functions[1024];
extern int func_count;

void register_function(AST_NODE*);
