#include "uart.h"

void uart_baud_init(int hz, int rate)
{
	unsigned int uartdiv, int_part, frac_part;

	uartdiv = (hz * 4 + rate / 2) / rate;   	
	int_part  = uartdiv / 64;
	frac_part = uartdiv % 64;

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
	unsigned int ctrl = UART->LCR_H;

	if (enable)
		ctrl |= 0x8;
	else 
		ctrl &= (~0x8);

	UART->LCR_H = ctrl;
}

void uart_transmit_enable(int enable)
{
	unsigned int ctrl = UART->CR;
	unsigned int mask = (1 << 8);

	if (enable)
		ctrl |= mask;
	else 
		ctrl &= (~mask);

	UART->CR = ctrl;
}

int uart_flags_busy(int busy)
{
	unsigned int flags = UART->FR;
	return (flags & 0x8) == busy;
}

void uart_transmit_init()
{
	uart_enable(0);
	while (uart_flags_busy(1));
	uart_fifo_enable(0);

	uart_transmit_enable(1);
	uart_enable(1);
}

void uart_write(char b)
{
	char *dr = (char*)(void*)(&UART->DR);
	while (uart_flags_busy(1));
	*dr = b;
}
