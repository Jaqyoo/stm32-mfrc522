#include "uart.h"

void uartInit(void)
{
	GPIO_InitTypeDef ginit;
	USART_InitTypeDef uinit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	//PA9 TX
	ginit.GPIO_Mode = GPIO_Mode_AF_PP;
	ginit.GPIO_Pin = GPIO_Pin_9;
	ginit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &ginit);
	//PA10 RX
	ginit.GPIO_Pin = GPIO_Pin_10;
	ginit.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &ginit);
	
	uinit.USART_BaudRate = 115200;
	uinit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uinit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	uinit.USART_Parity = USART_Parity_No;
	uinit.USART_StopBits = USART_StopBits_1;
	uinit.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &uinit);
	
	USART_Cmd(USART1, ENABLE);
}

int sendchar(int ch)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 
	USART_SendData(USART1, ch);
	return (ch);
}

int getkey(void)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return ((int)USART_ReceiveData(USART1));
}
