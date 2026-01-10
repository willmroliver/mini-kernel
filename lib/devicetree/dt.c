#include "dt.h"

void dt_traverse(struct fdt_node* node, 
		 int (*cb)(struct fdt_node *node, void *arg), 
		 void *arg)
{
	if (cb(node, arg))
		return;

	for (int i = 0; i < node->n_sub; i++)
		dt_traverse(node->subnodes + i, cb, arg);
}

struct fdt_prop_desc *dt_prop(struct fdt_node *node, const char *name)
{
	for (int i = 0; i < node->n_prop; i++)
		if (strcmp(node->props[i].name, name) == 0)
			return node->props + i;
	return 0;
}
