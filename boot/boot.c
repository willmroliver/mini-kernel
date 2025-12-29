#include <core/types.h>
#include <core/endian.h>
#include <mem/mem.h>
#include <lib/devicetree/dt.h>

extern vaddr_t _sheap;
extern vaddr_t _eheap;
extern vaddr_t _sdtb;

extern void _boot_main()
{
	struct fdt_node *dt;

	_mem_bump_alloc_init(_sheap, _eheap-_sheap);

	dt = fdt_parse((void *)_sdtb, 0);

	for (;;);
}
