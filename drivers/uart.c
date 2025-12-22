#include "uart.h"

void uart_baud_init(int hz, int rate)
{
	unsigned int div, int_part, frac_part;

	div = (hz * 4 + rate / 2) / rate;   	
	int_part  = div / 64;
	frac_part = div % 64;

	UART->IBRD = int_part;
	UART->FBRD = frac_part;
}

void uart_enable(int enable)
{
	unsigned int ctrl = UART->CR;

	if (enable)
		ctrl |= 0x1;
	else
		ctrl &= (~0x1);

	UART->CR = ctrl;
}

void uart_fifo_enable(int enable)
{
	char ctrl = UART->LCR_H;

	if (enable)
		ctrl |= 0x10;
	else 
		ctrl &= (~0x10);

	UART->LCR_H = ctrl;
}

void uart_transmit_enable(int enable)
{
	unsigned int ctrl = UART->CR;
	unsigned int mask = (0x1 << 8);

	if (enable)
		ctrl |= mask;
	else 
		ctrl &= (~mask);

	UART->CR = ctrl;
}

void uart_receive_enable(int enable)
{
	unsigned int ctrl = UART->CR;
	unsigned int mask = (0x1 << 9);

	if (enable)
		ctrl |= mask;
	else 
		ctrl &= (~mask);

	UART->CR = ctrl;
}

int uart_flags_busy()
{
	unsigned int flags = UART->FR;
	return flags & 0x8;
}

int uart_flags_tx_full()
{
	unsigned int flags = UART->FR;
	return flags & 0x20;
}

int uart_flags_rx_empty()
{
	unsigned int flags = UART->FR;
	return flags & 0x10;
}

void uart_transmit_init(int fifo)
{
	uart_enable(0);
	while (UART->FR & 0x8); // BUSY
	uart_fifo_enable(0);

	uart_transmit_enable(1);
	
	if (fifo)
		uart_fifo_enable(1);

	uart_enable(1);
}

void uart_receive_init(int fifo)
{
	uart_enable(0);
	while (UART->FR & 0x8); // BUSY
	uart_fifo_enable(0);

	uart_receive_enable(1);

	if (fifo)
		uart_fifo_enable(1);

	uart_enable(1);
}

void uart_init(int fifo)
{
	int ctrl = UART->CR;
	int tx_rx_mask = (0x3 << 8);

	uart_enable(0);
	while (UART->FR & 0x8); // BUSY
	uart_fifo_enable(0);

	UART->CR = ctrl | tx_rx_mask;

	if (fifo)
		uart_fifo_enable(1);
	
	uart_enable(1);
}

char uart_read()
{
	int data;

	while (UART->FR & 0x10); // RXFE
	data = UART->DR;

	return (char)data;
}

void uart_write(char b)
{
	int mask = 0x8 | 0x20; // BUSY or TXFF 

	while (UART->FR & mask);
	UART->DR = b;
}
