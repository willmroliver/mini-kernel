#ifndef INC_UART_H
#define INC_UART_H

#include <core/types.h>
#include <lib/devicetree/dt.h>

#define DEFAULT_BAUD_RATE 230400

typedef struct {
	volatile u32 DR;	    // 0x0 
	volatile u32 RSR_ECR;	    // 0x4
	volatile u32 _reserved0[4]; // 0x8-0x14
	volatile u32 FR;	    // 0x18
	volatile u32 _reserved1;    // 0x1c
	volatile u32 ILPR;	    // 0x20
	volatile u32 IBRD;	    // 0x24
	volatile u32 FBRD;	    // 0x28
	volatile u32 LCR_H;         // 0x2c
	volatile u32 CR;            // 0x30
	volatile u32 IFLS;          // 0x34
	volatile u32 IMSC;          // 0x38
	volatile u32 RIS;           // 0x3c
	volatile u32 MIS;           // 0x40
	volatile u32 ICR;           // 0x44
	volatile u32 DMACR;         // 0x48
} uart_base_t;

void uart_driver_dt_init(uart_base_t *,
			 struct fdt_node *node, 
			 int clk_hz, 
			 int baud_rate);

/**
 * Initializes the device with the specified clock frequency and baud rate.
 * SHOULD NOT be called if UART is enabled.
 * 
 * @param hz The clock frequency in Hz.
 * @param rate The desired baud rate.
 */
void uart_baud_init(uart_base_t *, int, int);

void uart_enable(uart_base_t *, int);

void uart_fifo_enable(uart_base_t *, int);

void uart_transmit_enable(uart_base_t *, int);

void uart_receive_enable(uart_base_t *, int);

int uart_flags_busy(uart_base_t *);

int uart_flags_tx_full(uart_base_t *);

int uart_flags_rx_empty(uart_base_t *);

void uart_transmit_init(uart_base_t *, int);

void uart_receive_init(uart_base_t *, int);

/**
 * Initializes UART to receive and transmit.
 * Be sure to initialize the baud rate divisor first.
 * 
 * @param fifo If non-zero, enables FIFO mode.
 */
void uart_init(uart_base_t *, int);

char uart_read(uart_base_t *);

void uart_write(uart_base_t *, char);

#endif
