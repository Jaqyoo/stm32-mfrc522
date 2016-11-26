#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"
#include "pcd_hal.h"
#include "pcd.h"
#include "delay.h"
#include "debug.h"

int main()
{
	char a;
	char r,i;
	uartInit();
	PCD_Hal_Init();
	
	#ifdef DEBUG_MAIN
	printf("hello.\n");
	printf("start delay\n");
	delay_ms(3000);
	printf("stop delay\n");
	#endif
	
	PCD_AntennaOn();
	#ifdef DEBUG_MAIN
	a = PCD_RReg(TxControlReg);
	printf("AntennaOn: %x.\n", a);
	#endif
	
	PCD_AntennaOff();
	#ifdef DEBUG_MAIN
	a = PCD_RReg(TxControlReg);
	printf("AntennaOff: %x.\n", a);
	#endif

	PCD_Reset();

	while(1)
	{
		a = getchar();
		putchar(a);
	}
}
