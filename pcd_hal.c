#include "pcd_hal.h"

void PCD_Hal_Config(void)
{
	GPIO_InitTypeDef g;
	SPI_InitTypeDef s;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
	
	//PA5 SCK 
	g.GPIO_Mode = GPIO_Mode_AF_PP;
	g.GPIO_Pin = GPIO_Pin_5;
	g.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &g);
	//PA6 MISO
	g.GPIO_Mode = GPIO_Mode_IPU;
	g.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &g);
	//PA7 MOSI
	g.GPIO_Mode = GPIO_Mode_AF_PP;
	g.GPIO_Pin = GPIO_Pin_7;
	g.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &g);
	//PA4 CS (NOT NSS BY SPI)
	g.GPIO_Mode = GPIO_Mode_Out_PP;
	g.GPIO_Pin = GPIO_Pin_4;
	g.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &g);
	//PA3 RST
	g.GPIO_Mode = GPIO_Mode_Out_PP;
	g.GPIO_Pin = GPIO_Pin_3;
	g.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &g);
	
	PCD_CS_Dis();
	
	s.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	s.SPI_Mode = SPI_Mode_Master;
	s.SPI_DataSize = SPI_DataSize_8b;
	s.SPI_CPOL = SPI_CPOL_High;
	s.SPI_CPHA = SPI_CPHA_1Edge;
	s.SPI_NSS = SPI_NSS_Soft;
	s.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	s.SPI_FirstBit = SPI_FirstBit_MSB;
	s.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &s);
}

void PCD_Hal_Init(void)
{
	PCD_Hal_Config();
	PCD_Hal_Open();
}

void PCD_Hal_Open(void)
{
	//RST = 1, pcd reset phase starts
	GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);
	SPI_Cmd(SPI1, ENABLE);
}

void PCD_Hal_Close(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);
	SPI_Cmd(SPI1, DISABLE);
}

void PCD_CS_En(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
}

void PCD_CS_Dis(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
}


uint8_t PCD_HAL_W(uint8_t value)
{
	uint8_t ret;
	PCD_CS_En();
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
	SPI_I2S_SendData(SPI1, value);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	ret = SPI_I2S_ReceiveData(SPI1);
	PCD_CS_Dis();
	return ret;
}

uint8_t PCD_HAL_R(void)
{
	return PCD_HAL_W(0x00);
}

