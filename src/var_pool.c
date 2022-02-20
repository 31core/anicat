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

int var_pool_set_value(struct variable_pool *pool, char *name, void *data)
{
	for(int i = 0; i < POOL_SIZE; i++)
	{
		if(pool->vars[i] != NULL && !strcmp(pool->vars[i], name))
		{
			pool->vars[i]->data = malloc(strlen(data + 1));
			strcpy(pool->vars[i]->data, data);
			return 0;
		}
	}
	return -1;
}

void var_pool_free(struct variable_pool *pool)
{
	for(int i = 0; i < POOL_SIZE; i++)
	{
		if(pool->vars[i] != NULL)
		{
			if(pool->vars[i]->data != NULL)
			{
				free(pool->vars[i]->data);
			}
			free(pool->vars[i]);
		}
	}
}