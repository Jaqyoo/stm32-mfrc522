#include "delay.h"



void delay_us(uint32_t us)
{
	uint32_t i, j;
	for(i = us; i>0; i--)
		for(j = 12; j>0; j--);
}

void delay_ms(uint16_t ms)
{
	uint16_t i, j;
	for(i = ms; i>0; i--)
		for(j = 12000; j>0; j--);
}