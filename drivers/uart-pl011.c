#include "uart-pl011.h"

uart_base_t *uart_driver_init(paddr_t reg,
			      int clk_hz, 
			      int baud_rate)
{
	uart_base_t *uart = (uart_base_t *)reg;
	uart_baud_init(uart, clk_hz, baud_rate);
	return uart;
}

void uart_baud_init(uart_base_t *uart, int hz, int rate)
{
	u32 div, int_part, frac_part;

	div = (hz*4 + rate/2) / rate;   	
	int_part  = div / 64;
	frac_part = div % 64;

	uart->IBRD = int_part;
	uart->FBRD = frac_part;
}

void uart_enable(uart_base_t *uart, int enable)
{
	u32 ctrl = uart->CR;

	if (enable)
		ctrl |= 0x1;
	else
		ctrl &= (~0x1);

	uart->CR = ctrl;
}

void uart_fifo_enable(uart_base_t *uart, int enable)
{
	char ctrl = uart->LCR_H;

	if (enable)
		ctrl |= 0x10;
	else 
		ctrl &= (~0x10);

	uart->LCR_H = ctrl;
}

void uart_transmit_enable(uart_base_t *uart, int enable)
{
	u32 ctrl = uart->CR;
	u32 mask = (0x1 << 8);

	if (enable)
		ctrl |= mask;
	else 
		ctrl &= (~mask);

	uart->CR = ctrl;
}

void uart_receive_enable(uart_base_t *uart, int enable)
{
	u32 ctrl = uart->CR;
	u32 mask = (0x1 << 9);

	if (enable)
		ctrl |= mask;
	else 
		ctrl &= (~mask);

	uart->CR = ctrl;
}

int uart_flags_busy(uart_base_t *uart)
{
	u32 flags = uart->FR;
	return flags & 0x8;
}

int uart_flags_tx_full(uart_base_t *uart)
{
	u32 flags = uart->FR;
	return flags & 0x20;
}

int uart_flags_rx_empty(uart_base_t *uart)
{
	u32 flags = uart->FR;
	return flags & 0x10;
}

void uart_transmit_init(uart_base_t *uart, int fifo)
{
	uart_enable(uart, 0);
	while (uart->FR & 0x8); // BUSY
	uart_fifo_enable(uart, 0);

	uart_transmit_enable(uart, 1);
	
	if (fifo)
		uart_fifo_enable(uart, 1);

	uart_enable(uart, 1);
}

void uart_receive_init(uart_base_t *uart, int fifo)
{
	uart_enable(uart, 0);
	while (uart->FR & 0x8); // BUSY
	uart_fifo_enable(uart, 0);

	uart_receive_enable(uart, 1);

	if (fifo)
		uart_fifo_enable(uart, 1);

	uart_enable(uart, 1);
}

void uart_init(uart_base_t *uart, int fifo)
{
	int ctrl = uart->CR;
	int tx_rx_mask = (0x3 << 8);

	uart_enable(uart, 0);
	while (uart->FR & 0x8); // BUSY
	uart_fifo_enable(uart, 0);

	uart->CR = ctrl | tx_rx_mask;

	if (fifo)
		uart_fifo_enable(uart, 1);
	
	uart_enable(uart, 1);
}

char uart_read(uart_base_t *uart)
{
	int data;

	while (uart->FR & 0x10); // RXFE
	data = uart->DR;

	return (char)data;
}

void uart_write(uart_base_t *uart, char b)
{
	int mask = 0x8 | 0x20; // BUSY or TXFF 

	while (uart->FR & mask);
	uart->DR = b;
}
