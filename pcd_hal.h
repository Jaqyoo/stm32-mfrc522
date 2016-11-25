#ifndef PCD_HAL_H
#define PCD_HAL_H

#include "stm32f10x.h"

void PCD_Hal_Config(void);
void PCD_Hal_Init(void);
void PCD_Hal_Open(void);
void PCD_Hal_Close(void);

void PCD_CS_En(void);
void PCD_CS_Dis(void);
uint8_t PCD_HAL_W(uint8_t value);
uint8_t PCD_HAL_R(void);

void PCD_WReg(uint8_t addr, uint8_t value);
uint8_t PCD_RReg(uint8_t addr);

#endif
