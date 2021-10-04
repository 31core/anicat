#include <stdio.h>
//#include <src/token.h>
#include <src/ast.h>
#include <src/debug.h>
#include <src/format.h>
#include <src/scanner.h>

int main()
{
    ast_node_manage_init();

    char code[100];
    struct token tk[100];
    format_code(code, "func a()");
    scan_code(tk, code);
    
    struct ast_node ast;
    struct ast_node *ast1, *ast2;

    ast_node_init(&ast);
    ast_tree_build(&ast, tk, 2);
    print_ast_tree(&ast);
}