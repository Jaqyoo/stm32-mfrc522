#include "pcd.h"

void PCD_WReg(uint8_t addr, uint8_t value)
{
	delay_ms(2);
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

void PCD_Init(char class)
{

}

uint8_t PCD_Reset(void)
{
	int i;
	PCD_Hal_Close();

	PCD_Hal_Open();	
	PCD_WReg(CommandReg, 0x0F);
	delay_ms(50);
		
	PCD_WReg(ModeReg, 0x3d);
	#ifdef DEBUG_RESET
	if(PCD_RReg(ModeReg) != 0x3d){
		printf("ModeReg error.\n");
		return PCD_ERR;
	}
	#endif
	
	PCD_WReg(TReloadRegL, 30);
	#ifdef DEBUG_RESET
	if(PCD_RReg(TReloadRegL) != 30){
		printf("TReloadRegL error.\n");
		return PCD_ERR;
	}
	#endif
	
	PCD_WReg(TReloadRegH, 0);
	#ifdef DEBUG_RESET
	if(PCD_RReg(TReloadRegH) != 0){
		printf("TReloadRegH error.\n");
		return PCD_ERR;
	}
	#endif
	
	PCD_WReg(TModeReg, 0x8d);
	#ifdef DEBUG_RESET
	if(PCD_RReg(TModeReg) != 0x8d){
		printf("TModeReg error.\n");
		return PCD_ERR;
	}
	#endif
	
	PCD_WReg(TPrescalerReg, 0x3e);
	#ifdef DEBUG_RESET
	if(PCD_RReg(TPrescalerReg) != 0x3e){
		printf("TPrescalerReg error.\n");
		return PCD_ERR;
	}
	#endif
	
	PCD_WReg(TxASKReg, 0x40);
	#ifdef DEBUG_RESET
	if(PCD_RReg(TxASKReg) != 0x40){
		printf("TxASKReg error.\n");
		return PCD_ERR;
	}
	#endif
	
	return PCD_OK;
}
