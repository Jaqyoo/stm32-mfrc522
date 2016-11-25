#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"

int main()
{

	uartInit();
	printf("hello.\n");

	while(1);
}
