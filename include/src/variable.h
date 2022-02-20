#pragma once
#include <src/ast.h>

#define VAR_TYPE_NULL 	0
#define VAR_TYPE_INT 	1
#define VAR_TYPE_BOOL	2
#define VAR_TYPE_STRING	3

struct variable
{
	char name[20];
	int type;
	void *data;
};

#define POOL_SIZE 1024

struct variable_pool
{
	struct variable *vars[POOL_SIZE];
};

extern struct variable variables[1024];
extern int var_count;

int detect_type(char*);
void register_variable(AST_NODE*);
void var_pool_init(struct variable_pool*);
void var_pool_append(struct variable_pool*, char*, int);
void var_pool_free(struct variable_pool*);
