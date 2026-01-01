#include <core/types.h>
#include <core/endian.h>
#include <core/mem.h>
#include <lib/devicetree/dt.h>
#include <sys/sys.h>

extern char _sheap[], _eheap[];

void __boot_main(void *fdt)
{
	struct mem_bump mem;
	struct fdt_node *dt;

	__init_exception_handlers(0);

	mem_bump_init(&mem, (vaddr_t)_sheap);
	__mem_global_set((struct mem_ix *)&mem);

	dt = fdt_parse(fdt, 0);

	for (;;);
}
