#include <stdlib.h>
#include <string.h>
#include <src/variable.h>

void var_pool_init(struct variable_pool *pool)
{
	for(int i = 0; i < POOL_SIZE; i++)
	{
		pool->vars[i] = NULL;
	}
}

void var_pool_append(struct variable_pool *pool, char *name, int type)
{
	for(int i = 0; i < POOL_SIZE; i++)
	{
		if(pool->vars[i] == NULL)
		{
			pool->vars[i] = malloc(sizeof(struct variable));
			strcpy(pool->vars[i]->name, name);
			pool->vars[i]->type = type;
			return;
		}
	}
}

void var_pool_free(struct variable_pool *pool)
{
	for(int i = 0; i < POOL_SIZE; i++)
	{
		if(pool->vars[i] != NULL)
		{
			free(pool->vars[i]);
		}
	}
}