#include <core/types.h>
#include <core/endian.h>
#include <core/mem.h>
#include <lib/devicetree/dt.h>
#include <arch/exception.h>

extern char _sheap[], _eheap[];

extern void _boot_main(void *fdt)
{
	struct mem_bump mem;
	struct fdt_node *dt;

	arch_init_exception_handler();

	mem_bump_init(&mem, (vaddr_t)_sheap);
	__mem_global_set((struct mem_ix *)&mem);

	dt = fdt_parse(fdt, 0);

	for (;;);
}
