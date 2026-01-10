#include <core/types.h>
#include <core/endian.h>
#include <core/mem.h>
#include <lib/devicetree/dt.h>
#include <lib/devicetree/init.h>
#include <sys/sys.h>
#include <sys/mmu.h>

extern void *_sheap, *_eheap;

void __boot_main(void *fdt, struct mmu_config *c) 
{
	struct mem_bump mem;
	struct fdt_node *dt;
	uart_base_t *uart;

	__init_exception_handlers(0);
	__init_mmu(c);

	mem_bump_init(&mem, (vaddr_t)&_sheap);
	__mem_global_set((struct mem_ix *)&mem);

	dt = fdt_parse(fdt, 0);
	uart = dt_init_uart_pl011(dt);
	uart_init(uart, 1);

	for (;;)
		uart_write(uart, '1');
}
