#ifndef INC_UART_H
#define INC_UART_H

#define DEFAULT_BAUD_RATE 230400

typedef struct {
	volatile unsigned int DR;	       // 0x0 
	volatile unsigned int RSR_ECR;	       // 0x4
	volatile unsigned int _reserved0[4];   // 0x8-0x14
	volatile unsigned int FR;	       // 0x18
	volatile unsigned int _reserved1;      // 0x1c
	volatile unsigned int ILPR;	       // 0x20
	volatile unsigned int IBRD;	       // 0x24
	volatile unsigned int FBRD;	       // 0x28
	volatile unsigned int LCR_H;           // 0x2c
	volatile unsigned int CR;              // 0x30
	volatile unsigned int IFLS;            // 0x34
	volatile unsigned int IMSC;            // 0x38
	volatile unsigned int RIS;             // 0x3c
	volatile unsigned int MIS;             // 0x40
	volatile unsigned int ICR;             // 0x44
	volatile unsigned int DMACR;           // 0x48
} uart_base_t;

#define UART ((uart_base_t *)(0x9000000))

/**
 * Initializes the device with the specified clock frequency and baud rate.
 * SHOULD NOT be called if UART is enabled.
 * 
 * @param hz The clock frequency in Hz.
 * @param rate The desired baud rate.
 */
void uart_baud_init(int, int);

void uart_enable(int);

void uart_fifo_enable(int);

void uart_transmit_enable(int);

void uart_receive_enable(int);

int uart_flags_busy();

int uart_flags_tx_full();

int uart_flags_rx_empty();

void uart_transmit_init(int);

void uart_receive_init(int);

/**
 * Initializes UART to receive and transmit.
 * Be sure to initialize the baud rate divisor first.
 * 
 * @param fifo If non-zero, enables FIFO mode.
 */
void uart_init(int);

char uart_read();

void uart_write(char);

#endif
