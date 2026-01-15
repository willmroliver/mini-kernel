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

struct fdt_node *dt_interrupt_root(struct fdt_node *node)
{
	struct fdt_prop_desc 
		*ctrl = dt_prop(node, FDT_PROP_INT_CONTROLLER),
		*irq = dt_prop(node, FDT_PROP_INTERRUPTS);
	struct fdt_node *root = 0;
	
	if (ctrl && !irq)
		return node;
	
	for (int i = 0; i < node->n_sub; i++)
		if ((root = dt_interrupt_root(node->subnodes + i)))
			return root;
	
	return 0;
}
