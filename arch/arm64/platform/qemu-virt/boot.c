#include "drivers/arm/gic-v2.h"
#include <core/types.h>
#include <core/endian.h>
#include <core/mem.h>
#include <drivers/arm/uart-pl011.h>
#include <lib/devicetree/dt.h>
#include <arch/sys/gic.h>
#include <sys/mmu.h>
#include <sys/sys.h>

extern void *_sheap, *_eheap;

void __uart_irq_handler(void *arg)
{
	char c, d;
	struct uart *uart = arg;

	while (!uart_flags_rx_empty(uart)) {
		ring_push(uart->buf, uart_getc(uart));
		if ((c = ring_back(uart->buf)) == '\r')
			break;
	}

	if (c != '\r')
		return;

	ring_pop(uart->buf, (u8*)&c);
	while (ring_pop(uart->buf, (u8*)&d)) {
		uart_putc(uart, c);
		c = d;
	}

	uart_putc(uart, '\n');
}

void __boot_main(void *fdt, struct mmu_config *c) 
{
	struct gic_handler handler;
	struct mem_bump mem;
	struct fdt_node *dt;
	struct gic *gic;
	struct uart *uart;

	__init_exception_handlers(0);
	__init_mmu(c);

	mem_bump_init(&mem, (vaddr_t)&_sheap);
	__mem_global_set((struct mem_ix *)&mem);

	dt = fdt_parse(fdt, 0);

	gic = gic_devicetree_init(dt);
	uart = uart_pl011_devicetree_init(dt, gic);

	__arm_gic_global_set((struct gic_ix *)gic);

	handler.fn = __uart_irq_handler;
	handler.arg = uart;

	// @TODO - get SPI ID from prop_desc
	gic_interrupt_enable(&(gic->ix), &handler, uart->intid);

	uart_init(uart, 1);

	for (;;);
}

