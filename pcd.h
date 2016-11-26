#ifndef PCD_H
#define PCD_H

#include "pcd_hal.h"


void PCD_WReg(uint8_t addr, uint8_t value);
uint8_t PCD_RReg(uint8_t addr);

void PCD_ClearBits(uint8_t addr, uint8_t mask);
void PCD_SetBits(uint8_t addr, uint8_t mask);

void PCD_Init(char class);
void PCD_Reset(void);

#endif
