#include "init.h"
#include "dt.h"
#include <sys/mmu.h>
#include <core/str.h>

static int __dt_uart_tst(struct fdt_node *node, void *arg)
{
	static const char *prop = "compatible";
	static const char *compatible = "arm,pl011";

	struct fdt_prop_desc *p = dt_prop(node, prop);
	if (p == 0)
		return 0;

	if (strcmp((char *)p->data, compatible) == 0) {
		*(struct fdt_node **)arg = node;
		return 1;
	}
	
	return 0;
}

uart_base_t *dt_init_uart_pl011(struct fdt_node *dt)
{
	
	struct mmu_mapping mem;
	struct fdt_node *node = 0;

	dt_traverse(dt, __dt_uart_tst, &node);

	if (node == 0)
		return 0;

	mem.pa = node->reg[0].addr;
	mem.va = -1;
	mem.size = node->reg[0].size;
	mem.attr_indx = 1;

	mmu_map(&mem, 0);

	// @TODO - dynamically determine clk_hz from node `clocks` phandle
	return uart_driver_init(mem.va, 0x16e3600, DEFAULT_BAUD_RATE);
}
