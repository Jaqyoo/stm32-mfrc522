#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"
#include "pcd_hal.h"
#include "pcd.h"

int main()
{
	char a;
	uartInit();
	PCD_Hal_Init();
	printf("hello.\n");
	
	PCD_WReg(0x02, 0x40);
	a = PCD_RReg(0x02);
	printf("Read Reg: %x.\n", a);
	
	PCD_ClearBits(0x02, 0x40);
	a = PCD_RReg(0x02);
	printf("ClearBits: %x.\n", a);
	
	PCD_SetBits(0x02, 0x40);
	a = PCD_RReg(0x02);
	printf("SetBits: %x.\n", a);

	while(1)
	{
		a = getchar();
		putchar(a);
	}
}
