#ifndef INC_UART_H
#define INC_UART_H

typedef struct {
	volatile unsigned int DR;	      // 0x0 
	volatile unsigned int RSR_ECR;	      // 0x4
	volatile unsigned int _reserved0[4];  // 0x8-0x14
	volatile unsigned int FR;	      // 0x18
	volatile unsigned int _reserved1;     // 0x1c
	volatile unsigned int ILPR;	      // 0x20
	volatile unsigned int IBRD;	      // 0x24
	volatile unsigned int FBRD;	      // 0x28
	volatile unsigned int LCR_H;          // 0x2c
	volatile unsigned int CR;             // 0x30
	volatile unsigned int IFLS;           // 0x34
	volatile unsigned int IMSC;           // 0x38
	volatile unsigned int RIS;            // 0x3c
	volatile unsigned int MIS;            // 0x40
	volatile unsigned int ICR;            // 0x44
	volatile unsigned int DMACR;          // 0x48
} uart_base_t;

#define UART ((uart_base_t *)(0x9000000))

void uart_baud_init(int hz, int rate);

void uart_enable(int);

void uart_fifo_enable(int);

void uart_transmit_enable(int);

int uart_flags_busy(int);

void uart_transmit_init();

void uart_write(char);

#endif
