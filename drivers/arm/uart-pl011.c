#include "uart-pl011.h"
#include <lib/devicetree/dt.h>
#include <sys/mmu.h>
#include <core/mem.h>

struct uart *uart_driver_init(vaddr_t reg,
			      int clk_hz, 
			      int baud_rate,
			      int buf_len)
{
	struct uart *uart = kmalloc(sizeof(struct uart));
	uart->buf = ring_alloc(buf_len);
	uart->regs = (struct uart_regs *)reg;

	uart_baud_init(uart, clk_hz, baud_rate);
	return uart;
}

void uart_baud_init(struct uart *uart, int hz, int rate)
{
	u32 div, int_part, frac_part;

	div = (hz*4 + rate/2) / rate;   	
	int_part  = div / 64;
	frac_part = div % 64;

	uart->regs->IBRD = int_part;
	uart->regs->FBRD = frac_part;
}

void uart_enable(struct uart *uart, int enable)
{
	u32 ctrl = uart->regs->CR;

	if (enable)
		ctrl |= 0x1;
	else
		ctrl &= (~0x1);

	uart->regs->CR = ctrl;
}

void uart_fifo_enable(struct uart *uart, int enable)
{
	char ctrl = uart->regs->LCR_H;

	if (enable)
		ctrl |= 0x10;
	else 
		ctrl &= (~0x10);

	uart->regs->LCR_H = ctrl;
}

void uart_transmit_enable(struct uart *uart, int enable)
{
	u32 ctrl = uart->regs->CR;
	u32 mask = (0x1 << 8);

	if (enable)
		ctrl |= mask;
	else 
		ctrl &= (~mask);

	uart->regs->CR = ctrl;
}

void uart_receive_enable(struct uart *uart, int enable)
{
	u32 ctrl = uart->regs->CR;
	u32 mask = (0x1 << 9);

	if (enable)
		ctrl |= mask;
	else 
		ctrl &= (~mask);

	uart->regs->CR = ctrl;
}

int uart_flags_busy(const struct uart *uart)
{
	u32 flags = uart->regs->FR;
	return flags & 0x8;
}

int uart_flags_tx_full(const struct uart *uart)
{
	u32 flags = uart->regs->FR;
	return flags & 0x20;
}

int uart_flags_rx_empty(const struct uart *uart)
{
	u32 flags = uart->regs->FR;
	return flags & 0x10;
}

void uart_transmit_init(struct uart *uart, int fifo)
{
	uart_enable(uart, 0);
	while (uart->regs->FR & 0x8); // BUSY
	uart_fifo_enable(uart, 0);

	uart_transmit_enable(uart, 1);

	if (fifo)
		uart_fifo_enable(uart, 1);

	uart_enable(uart, 1);
}

void uart_receive_init(struct uart *uart, int fifo)
{
	uart_enable(uart, 0);
	while (uart->regs->FR & 0x8); // BUSY
	uart_fifo_enable(uart, 0);

	uart_receive_enable(uart, 1);

	if (fifo)
		uart_fifo_enable(uart, 1);

	uart_enable(uart, 1);
}

void uart_init(struct uart *uart, int fifo)
{
	int ctrl = uart->regs->CR;
	int tx_rx_mask = (0x3 << 8);

	uart_enable(uart, 0);
	while (uart->regs->FR & 0x8); // BUSY
	uart_fifo_enable(uart, 0);

	uart->regs->CR = ctrl | tx_rx_mask;
	uart->regs->IMSC |= 0xff;

	if (fifo)
		uart_fifo_enable(uart, 1);

	uart_enable(uart, 1);
}

char uart_getc(const struct uart *uart)
{
	int data;

	while (uart->regs->FR & 0x10); // RXFE
	data = uart->regs->DR;

	return (char)data;
}

void uart_putc(struct uart *uart, char b)
{
	int mask = 0x8 | 0x20; // BUSY or TXFF 

	while (uart->regs->FR & mask);
	uart->regs->DR = b;
}

int uart_getln(const struct uart *uart, char *buf, int len)
{
	return 0;	
}


static int __dt_uart_tst(struct fdt_node *node, void *arg)
{
	static const char *compatible = "arm,pl011";

	struct fdt_prop_desc *p = dt_prop(node, FDT_PROP_COMPATIBLE);
	if (p == 0)
		return 0;

	if (strcmp((char *)p->data, compatible) == 0) {
		*(struct fdt_node **)arg = node;
		return 1;
	}

	return 0;
}

struct uart *uart_pl011_devicetree_init(struct fdt_node *dt)
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
	return uart_driver_init(mem.va, 0x16e3600, DEFAULT_BAUD_RATE, 0x100);
}
