#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"
#include "pcd_hal.h"
#include "pcd.h"
#include "delay.h"
#include "debug.h"
#include "picc.h"
#include "picc_app.h"

extern uint8_t RevBuffer[30]; 

int main()
{
	char s[16];
	char d0[16], d1[16];
	PiccStatus_t status;
	uint8_t i;
	uint8_t pTagType[2];
	uint8_t pSnr[4] = {0};
	
	strcpy(s, "Hello card\n");
	uartInit();
	PCD_Init();
	PCD_ConfigIsoType('A');
	
	RevBuffer[0] = 1;
	RevBuffer[1] = 0;
	//printf("Scaning:");
	while(1)
	{
		if(RevBuffer[0] == 1)
			RevBuffer[0] = 2;
		status = PICC_UDF_Cmd();
		if(status == PICC_OK)
			printf("PICC_OK\n");
		else
			printf("PICC_ERR\n");
		
		
		delay_ms(1000);
	}
}
