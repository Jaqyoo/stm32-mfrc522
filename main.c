#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"
#include "delay.h"
#include "picc.h"

uint8_t pKey[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};


int main()
{
	uint8_t pSnr[4] = {0,0,0,0};
	uint8_t pOldSnr[4] = {0,0,0,0};
	uint8_t pData[16];
	uint8_t i,j, count=0;
	
	uartInit();
	PCD_Init();
	PCD_ConfigIsoType('A');

	while(1)
	{
		if(PICC_SelectCard(pSnr) == PICC_OK){
			count = 0;	//when a card is on the PCD, loop returning PICC_OK and PICC_ERR, so reset the count
			if(PICC_isNewCard(pSnr, pOldSnr) == PICC_OK){
				printf("new card ");
				for(i=0; i<4; i++)	{pOldSnr[i] = pSnr[i]; printf("%02X", pSnr[4-i]);}
				printf("\n");
				
				for(i=0; i<64; i++){
					PICC_AppRead(pSnr, i, pKey, pKey, pData);
					printf("addr %02d: ", i);
					for(j=0;j<16;j++)
						printf("%02X ", pData[j]);
					printf("\n");
				}
			}
		}	else {	//not Select card
			if(count<2)	count++;
			else{
				count=0;
				for(i=0;i<4;i++)
					pOldSnr[i] = 0;
			}
		}
		delay_ms(1000);
	}
}


