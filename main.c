#include "stm32f10x.h"
#include "stdio.h"
#include "uart.h"
#include "pcd_hal.h"
#include "pcd.h"
#include "delay.h"
#include "debug.h"
#include "picc.h"
#include "picc_app.h"

#define NEW_CARD				1
#define NOT_NEW_CARD		0

uint8_t pKey[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t PICC_isNewCard(uint8_t pSnr[4]);

int main()
{
	PiccStatus_t status;
	uint8_t isNewCard;
	uint8_t pSnr[4];
	uint8_t addr;
	uint8_t pData[16];
	uint8_t i;
	
	uartInit();
	PCD_Init();
	PCD_ConfigIsoType('A');
	
	addr = 0x04;
	
	//printf("Scaning:");
	while(1)
	{
		isNewCard = PICC_isNewCard(pSnr);
		if(isNewCard == NEW_CARD){
			status = PICC_AuthState(PICC_AUTH_A, addr, pKey, pSnr);
			if(status == PICC_OK){
				printf("Auth A OK.\n");
				status = PICC_AuthState(PICC_AUTH_B, addr, pKey, pSnr);
				if(status == PICC_OK){
					printf("Auth B OK.\n");
					while(1)
					{
						PICC_Read(addr, pData);
						printf("Read data: ");
						for(i=16; i>0; i--)
							printf("%x", pData[i-1]);
						printf("\n");
						break;
					}
				}
			}
		delay_ms(1000);
		}
	}
}

uint8_t PICC_isNewCard(uint8_t pSnr[4])
{
	uint8_t pTagType[2];
	uint8_t ret,i;
	PiccStatus_t status;
	
	ret = NOT_NEW_CARD;
	
	status = PICC_Request(PICC_REQ_ISO, &pTagType[0]);
	if(status == PICC_OK){
		printf("Anticolling..\n");
		status = PICC_Anticoll(&pSnr[0]);
		if(status == PICC_OK){
			printf("Selecting..\n");
			status = PICC_Select(pSnr);
			if(status == PICC_OK){
				printf("Snr = ");
				for(i=4;i>0;i--){
					printf("%x", pSnr[i-1]);
				}
				printf("\n");
				ret = NEW_CARD;
			}
		}
	} else {}
		//printf("Request err.\n");
	
	return ret;
}
