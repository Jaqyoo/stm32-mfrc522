#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"
#include "pcd_hal.h"
#include "pcd.h"

int main()
{
	char a;
	char i;
	uartInit();
	PCD_Hal_Init();
	printf("hello.\n");
	
	for(i=0;i<0xff;i++)
	{
		printf("%x\t", i);
		PCD_WReg(0x02, i);
		a = PCD_RReg(0x02);
		printf("%x\n", a);
	}

	/*
	PCD_ClearBits(0x02, 0x40);
	
	PCD_SetBits(0x02, 0xFF);
	
	PCD_AntennaOn();
	a = PCD_RReg(TxControlReg);
	printf("AntennaOn: %x.\n", a);
	
	PCD_AntennaOff();
	a = PCD_RReg(TxControlReg);
	printf("AntennaOff: %x.\n", a);
*/
	while(1)
	{
		a = getchar();
		putchar(a);
	}
}
