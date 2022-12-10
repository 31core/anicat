#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <src/ast.h>
#include <src/debug.h>
#include <src/token.h>

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
	TOKEN tk[1024];
	AST_NODE ast;

	generate_token(tk, code);

	ast_node_init(&ast);
	ast.type = AST_TYPE_PROGRAM;
	ast_build(&ast, tk);

	//print_tokens(tk);

	print_ast(&ast);

	//register_function(&ast);
	//register_variable(&ast);
	//execute_func(get_func_ptr("main"));
}
