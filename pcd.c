#include "pcd.h"

void PCD_WReg(uint8_t addr, uint8_t value)
{
	PCD_HAL_W(addr<<1);
	PCD_HAL_W(value);
}

uint8_t PCD_RReg(uint8_t addr)
{
	PCD_HAL_W((addr<<1)|0x80);
	return(PCD_HAL_R());
}

void PCD_ClearBits(uint8_t addr, uint8_t mask)
{
	PCD_WReg(addr, PCD_RReg(addr) &(~mask));
}

void PCD_SetBits(uint8_t addr, uint8_t mask)
{
	PCD_WReg(addr, PCD_RReg(addr) | mask);
}
