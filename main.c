#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"

int main()
{
	char a;
	uartInit();
	printf("hello.\n");

	while(1)
	{
		a = getchar();
		putchar(a);
	}
}
