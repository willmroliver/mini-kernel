#include "uart.h"

#define QEMU_CLK_HZ 24000000
#define BAUD_RATE 230400

extern void _boot_main()
{
	char *word = "Hello, World!\n", buf[1024];
	int i = 0;
	char c;

	uart_baud_init(QEMU_CLK_HZ, BAUD_RATE);
	uart_init(1);

	while (1)
		uart_write(uart_read());

	for (;;);
}

