#include "pcd.h"

void PCD_WReg(uint8_t addr, uint8_t value)
{
	//delay_ms(2);
	PCD_CS_En();
	PCD_HAL_W(addr<<1);
	PCD_HAL_W(value);
	PCD_CS_Dis();
}

uint8_t PCD_RReg(uint8_t addr)
{
	uint8_t ret;
	PCD_CS_En();
	PCD_HAL_W((addr<<1)|0x80);
	ret = PCD_HAL_R();	
	PCD_CS_Dis();
	return ret;
}

void PCD_ClearBits(uint8_t addr, uint8_t mask)
{
	uint8_t tmp = 0;
	tmp = PCD_RReg(addr);
	tmp = tmp &(~mask);
	PCD_WReg(addr, tmp);
}

void PCD_SetBits(uint8_t addr, uint8_t mask)
{
	uint8_t tmp = 0;
	tmp = PCD_RReg(addr);
	tmp = tmp | mask;
	PCD_WReg(addr, PCD_RReg(addr) | mask);
}

void PCD_AntennaOn(void)
{
	PCD_SetBits(TxControlReg, 0x03);
}

void PCD_AntennaOff(void)
{
	PCD_ClearBits(TxControlReg, 0x03);
}

uint8_t PCD_Init(void)
{
	PCD_Hal_Init();
	return PCD_Reset();
}

uint8_t PCD_Reset(void)
{
	PCD_Hal_Close();
	delay_ms(10);
	PCD_Hal_Open();	
	
	//SoftReset 
	PCD_WReg(CommandReg, PCD_RESETPHASE);	
	delay_ms(50);
	
	//transmitter can only be started if an RF field is generated.
	//&& defines the preset value for the CRC coprocessor for the CalcCRC command
	//the preset value is 0x6363
	PCD_WReg(ModeReg, 0x3d);
//	#ifdef DEBUG_RESET
//	if(PCD_RReg(ModeReg) != 0x3d){
//		printf("ModeReg error.\n");
//		return PCD_ERR;
//	}
//	#endif
	
	//Defines the 16-bit timer reload value.
	PCD_WReg(TReloadRegL, 30);
	PCD_WReg(TReloadRegH, 0);
//	#ifdef DEBUG_RESET
//	if(PCD_RReg(TReloadRegL) != 30){
//		printf("TReloadRegL error.\n");
//		return PCD_ERR;
//	}
//	if(PCD_RReg(TReloadRegH) != 0){
//		printf("TReloadRegH error.\n");
//		return PCD_ERR;
//	}
//	#endif
	
	//timer starts automatically at the end of the transmission 
	//in all communication modes at all speeds
	//&& internal timer is running in non-gate mode
	//&& timer automatically restarts its count-down from the 
	//16-bit timer reload value instead of counting down to 0
	//the last 4-bit of TModeReg is the higher 4-b-t of 12-bit TPrescalerReg
	PCD_WReg(TModeReg, 0x8d);
	PCD_WReg(TPrescalerReg, 0x3e);
//	#ifdef DEBUG_RESET
//	if(PCD_RReg(TModeReg) != 0x8d){
//		printf("TModeReg error.\n");
//		return PCD_ERR;
//	}
//	if(PCD_RReg(TPrescalerReg) != 0x3e){
//		printf("TPrescalerReg error.\n");
//		return PCD_ERR;
//	}
//	#endif	

	//forces a 100% ASK modulation independent of the
	//ModGsPReg register setting
	PCD_WReg(TxASKReg, 0x40);
//	#ifdef DEBUG_RESET
//	if(PCD_RReg(TxASKReg) != 0x40){
//		printf("TxASKReg error.\n");
//		return PCD_ERR;
//	}
//	#endif
	
	return PCD_OK;
}


uint8_t PCD_ConfigIsoType(char type)
{
	if(type == 'A'){	//ISO14443_A
		
		//
		PCD_ClearBits(Status2Reg, 0x08);
		
		//confirm ModeReg = 0x3d
//		PCD_WReg(ModeReg, 0x3d);
//		#ifdef DEBUG_RESET
//		if(PCD_RReg(ModeReg) != 0x3d){
//			printf("ModeReg error.\n");
//			return PCD_ERR;
//		}
//		#endif
		
		//the higher 2-bit: selects the input of the contactless UART...
		//modulated signal from the internal analog module
		//&& the lower 6-bit: after data transmission the activation ...
		//of the receiver is delayed for RxWait bit-clocks ...
		//all other commands use this parameter ...
		//the counter starts immediately after the external RF field ...
		//is switch on
		PCD_WReg(RxSelReg, 0x86);
//		#ifdef DEBUG_RESET
//		if(PCD_RReg(RxSelReg) != 0x86){
//			printf("RxSelReg error.\n");
//			return PCD_ERR;
//		}
//		#endif
		
		//defines the receiver's signal voltage gain factor = 48dB
		PCD_WReg(RFCfgReg, 0x7f);
//		#ifdef DEBUG_RESET
//		if(PCD_RReg(RFCfgReg) != 0x7f){
//			printf("RFCfgReg error.\n");
//			return PCD_ERR;
//		}
//		#endif
		
		//confirm the TReloadReg, TMode, TPrescalerReg
		PCD_WReg(TReloadRegL, 30);
		PCD_WReg(TReloadRegH, 0);
		PCD_WReg(TModeReg, 0x8d);
		PCD_WReg(TPrescalerReg, 0x3e);
//		#ifdef DEBUG_RESET
//		if(PCD_RReg(TReloadRegL) != 30){
//			printf("TReloadRegL error.\n");
//			return PCD_ERR;
//		}
//		if(PCD_RReg(TReloadRegH) != 0){
//			printf("TReloadRegH error.\n");
//			return PCD_ERR;
//		}
//		if(PCD_RReg(TModeReg) != 0x8d){
//			printf("TModeReg error.\n");
//			return PCD_ERR;
//		}
//		if(PCD_RReg(TPrescalerReg) != 0x3e){
//			printf("TPrescalerReg error.\n");
//			return PCD_ERR;
//		}
//		#endif
		
		PCD_WReg(WaterLevelReg, 0x3f);
//		#ifdef DEBUG_RESET
//		if(PCD_RReg(WaterLevelReg) != 0x3f){
//			printf("WaterLevelReg error.\n");
//			return PCD_ERR;
//		}
//		#endif
		
		delay_ms(10);
		PCD_AntennaOn();
	}	else {return PCD_NOTAGERR;}
	
	return PCD_OK;
	
}

void PCD_CalCRC(uint8_t *pInData, uint8_t len, uint8_t *pOutData)
{
	uint8_t i,n;
	PCD_ClearBits(DivIrqReg, 0x04);
	PCD_WReg(CommandReg, PCD_IDLE);
	PCD_SetBits(FIFOLevelReg, 0x80);
	for(i=0; i<len; i++)
		PCD_WReg(FIFODataReg, *(pInData+i));
	
	PCD_WReg(CommandReg, PCD_CALCCRC);
	i=0xff;
	do{
		n=PCD_RReg(DivIrqReg);
		i--;
	} while((i!=0) && !(n&0x04));
	pOutData[0] = PCD_RReg(CRCResultRegL);
	pOutData[1] = PCD_RReg(CRCResultRegM);
}
