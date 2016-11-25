#ifndef UART_H
#define UART_H

#include "stm32f10x.h"
#include "stdio.h"

void uartInit(void);
int sendchar(int c);
int getkey(void);

#endif
