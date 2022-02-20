#include <src/ast.h>
#include <src/variable.h>
struct function
{
	char name[20];
	struct variable params[10];
	struct variable_pool pool;
	AST_NODE *code_block;
};

extern struct function functions[1024];
extern int func_count;

struct function* get_func_ptr(char*);
void register_function(AST_NODE*);
