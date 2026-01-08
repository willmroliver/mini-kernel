#include "dt.h"

void dt_traverse(struct fdt_node* node, 
		 void (*cb)(struct fdt_node *node, void *arg), 
		 void *arg)
{
	int i;

	cb(node, arg);

	for (i = 0; i < node->n_sub; i++)
		dt_traverse(node->subnodes + i, cb, arg);
}

