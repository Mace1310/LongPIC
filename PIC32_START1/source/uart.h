#ifndef __UART_H
#define __UART_H

void uart1Init(void);

void flushTxBuffer();

void writeByte(unsigned char var);

#endif
