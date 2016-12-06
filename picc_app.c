#include "picc_app.h"

static uint8_t data2[4]  = {0x12,0,0,0};
static uint8_t DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
static uint8_t g_ucTempbuf[20];    
static uint8_t MLastSelectedSnr[4];
uint8_t RevBuffer[30];  
static uint8_t SerBuffer[20];    

PiccStatus_t PICC_UDF_Cmd()
{
	uint8_t cmd;
	uint8_t status;
	cmd = RevBuffer[0];
	
	printf("cmd = %d\n", cmd);
	
	switch(cmd)
	{
		case PICC_UDF_HALT: {
			RevBuffer[0] = PICC_UDF_HALT;
			status = PICC_Halt();
			RevBuffer[1] = status;
			break;
		}
		
		case PICC_UDF_REQUEST: {
			status = PICC_Request(0x26, &RevBuffer[2]);
			if(status != PICC_OK){
				status = PICC_Request(0x26, &RevBuffer[2]);
				if(status != PICC_OK){
					RevBuffer[0] = PICC_UDF_HALT;
					RevBuffer[1] = status;
					break;
				}
			}
			RevBuffer[0] = PICC_UDF_ANTICOLL;
			RevBuffer[1] = status;
			break;
		}
		
		case PICC_UDF_ANTICOLL: {
			status = PICC_Anticoll(&RevBuffer[2]);
			if(status!=PICC_OK){
				RevBuffer[0] = PICC_UDF_HALT;
				RevBuffer[1] = status;
				break;
			}
			memcpy(MLastSelectedSnr, &RevBuffer[2], 4);
			RevBuffer[0] = PICC_UDF_AUTHSTATE;
			RevBuffer[1] = status;
			break;
		}
		
		case PICC_UDF_SELECT:{
			status = PICC_Select(MLastSelectedSnr);
			if(status != PICC_OK){
				RevBuffer[0] = PICC_HALT;
				RevBuffer[1] = status;
				break;
			}
			RevBuffer[0] = PICC_UDF_ANTICOLL;
			RevBuffer[1] = status;
			break;				
		}
		
		case PICC_UDF_AUTHSTATE: {
			status = PICC_AuthState(RevBuffer[1], RevBuffer[3], DefaultKey, MLastSelectedSnr);
			RevBuffer[0] = PICC_HALT;
			RevBuffer[1] = status;
		}
		
		case PICC_UDF_READ: {
			status = PICC_Read(RevBuffer[1], &RevBuffer[2]);
			if(status == PICC_OK)
				RevBuffer[0] = PICC_UDF_READ_OK;
			else
				RevBuffer[0] = PICC_UDF_HALT;
			RevBuffer[1] = status;
			break;
		}
		
		case PICC_UDF_WRITE: {
			status = PICC_Write(RevBuffer[1], &RevBuffer[2]);
			RevBuffer[0] = PICC_HALT;
			RevBuffer[1] = status;
			break;
		}
		
		default :{
			RevBuffer[0] = PICC_HALT;
			RevBuffer[1] = PICC_OK;
			break;
		}
	}
	return status;
}
