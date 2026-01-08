#include <core/types.h>
#include <core/endian.h>
#include <core/mem.h>
#include <lib/devicetree/dt.h>
#include <sys/sys.h>
#include <sys/mmu.h>

extern void *_sheap, *_eheap;

void __boot_main(void *fdt, paddr_t pt_start, paddr_t pt_end, paddr_t pt_next)
{
	struct mem_bump mem;
	struct fdt_node *dt;

	__init_mmu((paddr_t)pt_start, (paddr_t)pt_end, pt_next);
	__init_exception_handlers(0);

	mem_bump_init(&mem, (vaddr_t)&_sheap);
	__mem_global_set((struct mem_ix *)&mem);

	dt = fdt_parse(fdt, 0);

	for (;;);
}
