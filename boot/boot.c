#include <core/types.h>
#include <core/endian.h>
#include <mem/mem.h>
#include <lib/devicetree/dt.h>

extern char _sheap[], _eheap[];

extern void _boot_main(void *fdt)
{
	struct fdt_node *dt;

	_mem_bump_alloc_init((vaddr_t)_sheap, (vaddr_t)_eheap-(vaddr_t)_sheap);

	dt = fdt_parse(fdt, 0);

	for (;;);
}
