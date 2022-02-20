#include <stdio.h>
#include <stdlib.h>
#include <src/ast.h>
#include <src/debug.h>
#include <src/scanner.h>
#include <src/function.h>
#include <src/variable.h>

int main(void)
{
	ast_node_manage_init();

	FILE *f = fopen("test.ac", "r");
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
	print_ast_tree(&ast);
	register_function(&ast);
	register_variable(&ast);
	show_functions();
	show_variables();
}
