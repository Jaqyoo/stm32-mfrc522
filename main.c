#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"
#include "pcd_hal.h"
#include "pcd.h"
#include "delay.h"
#include "debug.h"

int main()
{
	char s[255];
	uartInit();
	PCD_Init();
	PCD_ConfigIsoType('A');
	
	#ifdef DEBUG_MAIN
	printf("hello.\n");
	printf("start delay\n");
	delay_ms(3000);
	printf("stop delay\n");
	#endif
	


	while(1)
	{
		printf("scaning:\n");
		scanf("%s", s);
		printf("%s\n", s);
	}
}
