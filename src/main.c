#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <src/ast.h>
#include <src/debug.h>
#include <src/scanner.h>
#include <src/function.h>
#include <src/variable.h>
#include <src/execute.h>

int main(int argc, char *argv[])
{
	ast_node_manage_init();

	FILE *f = fopen(argv[1], "r");
	if(f == NULL)
	{
		perror(argv[0]);
		return -1;
	}
	char *code = malloc(1024);
	int i = 0;
	while(!feof(f))
	{
		code[i] = fgetc(f);
		i++;
	}
	code[i] = '\0';

	fclose(f);
	struct token tk[1024];
	struct ast_node ast;

	scan_code(tk, code);

	ast_node_init(&ast);
	ast_tree_build(&ast, tk);
	register_function(&ast);
	register_variable(&ast);
	execute_func(get_func_ptr("main"));
}
