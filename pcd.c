#include "pcd.h"

void PCD_WReg(uint8_t addr, uint8_t value)
{
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
	//printf("Clearing Bits.\n");
	tmp = PCD_RReg(addr);
	//printf("tmp = %x.\n", tmp);
	tmp = tmp &(~mask);
	//printf("tmp = %x.\n", tmp);
	PCD_WReg(addr, tmp);
	//tmp = PCD_RReg(addr);
	//printf("tmp = %x.\n", tmp);
	//printf("Clearing ends.\n");
}

void PCD_SetBits(uint8_t addr, uint8_t mask)
{
	uint8_t tmp = 0;
	//printf("Setting Bits\n");
	tmp = PCD_RReg(addr);
	//printf("tmp = %x.\n", tmp);
	tmp = tmp | mask;
	//printf("tmp = %x.\n", tmp);
	PCD_WReg(addr, PCD_RReg(addr) | mask);
	//tmp = PCD_RReg(addr);
	//printf("tmp = %x.\n", tmp);
	//printf("Setting ends.\n");
}

void PCD_AntennaOn(void)
{
	PCD_SetBits(TxControlReg, 0x03);
}

void PCD_AntennaOff(void)
{
	PCD_ClearBits(TxControlReg, 0x03);
}
