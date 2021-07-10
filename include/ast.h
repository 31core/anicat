/* 运算表达式 */
struct expression
{
	struct ast_node *left, *right;
	int option;
};
/* AST节点 */
struct ast_node
{
	int type;
	char name[20];
	struct expression expr; //表达式
	void *data;
	struct ast_node *statement[100];
};
