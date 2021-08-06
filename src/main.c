#include <stdio.h>
#include <src/token.h>
#include <src/ast.h>
#include <src/debug.h>
#include <src/format.h>
#include <src/scanner.h>

int main()
{
	char str[100];
	struct token tk[10];
	format_code(str, "var i: Int = (9 + 3) * 2");
	puts(str);
	int num = scan_code(tk, str);
	struct ast_node nodes[10];
	nodes[0].node[0] = &nodes[1];
	nodes[0].node[1] = NULL;
	nodes[1].node[0] = &nodes[2];
	nodes[1].node[1] = NULL;
	print_ast_tree(nodes);
}