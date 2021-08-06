/* AST节点 */
struct ast_node
{
	int type;
	char name[20];
	struct ast_node *node[100];
};
