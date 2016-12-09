#include "picc.h"

#define MAXRLEN 18 

PiccStatus_t PICC_Comm(uint8_t PcdCommand, uint8_t *pInData, uint8_t inLen, uint8_t *pOutData, uint16_t *pOutLenBit)
{
	PiccStatus_t status;
	uint8_t cmd, irqEn, waitFor, lastBits, n;
	uint8_t valid, data;
	uint16_t i;
	
	status = PICC_ERR;
	cmd = PcdCommand;
	waitFor = 0x00;
	
	irqEn = 0x00;
	
	switch(cmd){
		case PCD_AUTHENT:
		{
			//allows the idle interrupt and error interrupt request to be propagated to pin IRQ
			irqEn = 0x12;
			//wait for .... interrupt
			waitFor = 0x10;
			break;
		}
		case PCD_TRANSCEIVE:
		{	
			//allows TxIEn, RxIEn, IdleIEn, LoAlertIEn, ErrIEn, TimerIEn
			irqEn = 0x77;
			//wait for receiver detected the end of a valid data stream or idle
			waitFor = 0x30;
			break;
		}
		default: break;
	}
	
	//signal on pin IRQ is equal to the IRq bit;
	PCD_WReg(ComIEnReg, irqEn|0x80);
	//indicates that the marked bits in the ComIrqReg register are cleared
	PCD_ClearBits(ComIrqReg, 0x80);
	//cancels current command execution
	PCD_WReg(CommandReg, PCD_IDLE);
	//immediately clears the internal FIFO buffer's read and write pointer
	//and ErrorReg register's BufferOvfl bit.
	PCD_SetBits(FIFOLevelReg, 0x80);
	
	for(i=0; i<inLen; i++)
		PCD_WReg(FIFODataReg, pInData[i]);
	
	PCD_WReg(CommandReg, cmd);
	if(cmd == PCD_TRANSCEIVE)
		PCD_SetBits(BitFramingReg, 0x80);	//starts the transmission of data
	
	i=200;
	do{
		n = PCD_RReg(ComIrqReg);
		i--;
	}while((i!=0) 
		&& !(n&0x01)	//timer decrements the timer value in register TCounterValReg to zero
		&& !(n&waitFor));	
	
	PCD_ClearBits(BitFramingReg, 0x80);	//stop the transmission of data
	
	if(i!=0){
		valid = PCD_RReg(ErrorReg);
		if(!(valid&0x1B)){	//not BufferOvfl, CollErr, ParityErr or ProtocolErr
			status = PICC_OK;
			
			if(n & irqEn & 0x01)	//not timer interrupt
				status = PICC_NOTAGERR;
			if(cmd == PCD_TRANSCEIVE){
				n = PCD_RReg(FIFOLevelReg);
				lastBits = PCD_RReg((ControlReg)&0x07);	//RxLastBits
				if(lastBits)
					*pOutLenBit = (n-1)*8+lastBits;
				else
					*pOutLenBit = n*8;
				
				if(n==0)
					n=1;
				else if(n>MAXRLEN)
					n=MAXRLEN;
			
				for(i=0; i<n; i++)
					pOutData[i] = PCD_RReg(FIFODataReg);				
			}
		}	
		else
			status = PICC_ERR;
	}
	
	PCD_SetBits(ControlReg, 0x80);	//timer stops immediately
	PCD_WReg(CommandReg, PCD_IDLE);	
	return status;			
}

/************************************************************
* find the cards
*	req_code[IN]:
*					0x52 = ISO14443A
*					0x26 = all cards not halted
*	pTagType[OUT]:
************************************************************/
PiccStatus_t PICC_Request(uint8_t req_code, uint8_t *pTagType)
{
	PiccStatus_t status;
	uint16_t unLen;
	uint8_t comBuf[MAXRLEN];
	
	PCD_ClearBits(Status2Reg, 0x08);
	PCD_WReg(BitFramingReg, 0x07);
	
	PCD_SetBits(TxControlReg, 0x03);
	comBuf[0] = req_code;
	
	status = PICC_Comm(PCD_TRANSCEIVE, comBuf, 1, comBuf, &unLen);
	if((status == PICC_OK)
		//&& (unLen == 0x10)
	)	{
		
		pTagType[0]		= comBuf[0];
		//printf("status = ok\n");
		pTagType[1] 	= comBuf[1];
	}
	else {
		status = PICC_ERR;
	}
	return status;
}

/***************************************
*	AntiColl
*		pSnr[OUT]: 4-byte card id
****************************************/
PiccStatus_t PICC_Anticoll(uint8_t *pSnr)
{
	PiccStatus_t status;
	uint8_t i, snr_check;
	uint16_t unLen;
	uint8_t comBuf[MAXRLEN];
	snr_check = 0;
	
	PCD_ClearBits(Status2Reg, 0x08);
	PCD_WReg(BitFramingReg, 0x00);
	PCD_ClearBits(CollReg, 0x80);
	
	comBuf[0] = PICC_ANTICOLL1;
	comBuf[1] = 0x20;
	
	status = PICC_Comm(PCD_TRANSCEIVE, comBuf, 2, comBuf, &unLen);
	
	if(status == PICC_OK){
		for(i=0; i<4; i++){
			*(pSnr+i) = comBuf[i];
			snr_check ^= comBuf[i];
		}
		if(snr_check != comBuf[i])
			status = PICC_ERR;
	}
	
	PCD_SetBits(CollReg, 0x80);
	return status;	
}

/******************************************
* pSnr[in]: 4-byte card ID
*******************************************/
PiccStatus_t PICC_Select(uint8_t *pSnr)
{
	PiccStatus_t status;
	uint8_t i;
	uint16_t unLen;
	uint8_t comBuf[MAXRLEN];
	
	comBuf[0] = PICC_ANTICOLL1;
	comBuf[1] = 0x70;
	comBuf[6] = 0;
	for(i=0; i<4; i++){
		comBuf[i+2] = *(pSnr+i);
		comBuf[6] ^= *(pSnr+i);
	}
	PCD_CalCRC(comBuf, 7, &comBuf[7]);
	PCD_ClearBits(Status2Reg, 0x08);
	
	status = PICC_Comm(PCD_TRANSCEIVE, comBuf, 9, comBuf, &unLen);
	//printf("unLen = %d\n", unLen);
	if((status == PICC_OK)
//		&& (unLen == 0x18)
	)
		status = PICC_OK;
	else
		status = PICC_ERR;
	
	return status;
}

/*
*	PICC_AuthState
*		auth_mode[IN]:
*				0x60:	Auth key A
*				0x61: Auth key B
*		addr[IN]
*		pKey[IN]: 6-byte Key
*		pSnr[IN]
*/
PiccStatus_t PICC_AuthState(uint8_t auth_mode, uint8_t addr, uint8_t *pKey, uint8_t *pSnr)
{
	PiccStatus_t status;
	uint16_t unLen;
	uint8_t i;
	uint8_t comBuf[MAXRLEN];
	
	comBuf[0] = auth_mode;
	comBuf[1] = addr;
	for(i=0; i<6; i++){
		comBuf[i+2] = *(pKey+i);
		comBuf[i+8] = *(pSnr+i);
	}
	
	status = PICC_Comm(PCD_AUTHENT, comBuf, 12, comBuf, &unLen);
	if((status != PICC_OK) || (!(PCD_RReg(Status2Reg)&0x08)))
		status = PICC_ERR;
	
	return status;
}

/*
*	Write in a block of PICC card
*		addr[IN]
*		pData[IN]: 16-byte data 
*/
PiccStatus_t PICC_Write(uint8_t addr, uint8_t *pData)
{
	PiccStatus_t status;
	uint16_t unLen;
	uint8_t i, comBuf[MAXRLEN];
	
	comBuf[0] = PICC_WRITE;
	comBuf[1] = addr;
	PCD_CalCRC(comBuf, 2, &comBuf[2]);
	
	status = PICC_Comm(PCD_TRANSCEIVE, comBuf, 4, comBuf, &unLen);
		printf("unlen = %d\n", unLen);
		printf("combuf[0] = %x\n", comBuf[0]);
	
	if((status!=PICC_OK) || (unLen!=4) || ((comBuf[0]&0x0f)!=0x0a))
		status = PICC_ERR;
	
	if(status == PICC_OK){
		for(i=0; i<16; i++)
			comBuf[i] = *(pData+i);
		
		PCD_CalCRC(comBuf, 16, &comBuf[16]);
		status = PICC_Comm(PCD_TRANSCEIVE, comBuf, 18, comBuf, &unLen);

		if((status!=PICC_OK) || (unLen!=4) || ((comBuf[0]&0x0f)!=0x0a))
			status = PICC_ERR;
	}
	
	return status;
}

/*
*	Read a block data of PICC card
*		addr[IN]
*		pData[OUT]: 16-byte data 
*/
PiccStatus_t PICC_Read(uint8_t addr, uint8_t *pData)
{
	PiccStatus_t status;
	uint16_t unLen;
	uint8_t i, comBuf[MAXRLEN];
	
	comBuf[0] = PICC_READ;
	comBuf[1] = addr;
	PCD_CalCRC(comBuf, 2, &comBuf[2]);
	
	status = PICC_Comm(PCD_TRANSCEIVE, comBuf, 4, comBuf, &unLen);
	//printf("(unlen = %d)", unLen);
	
	if((status==PICC_OK) 
		//&& (unLen==0x90)
	)
		for(i=0; i<16; i++)
			*(pData+i) = comBuf[i];
	else
		status = PICC_ERR;
	
	return status;
}

PiccStatus_t PICC_Halt(void)
{
	PiccStatus_t status;
	uint16_t unLen;
	uint8_t comBuf[MAXRLEN];
	
	comBuf[0] = PICC_HALT;
	comBuf[1] = 0;
	PCD_CalCRC(comBuf, 2, &comBuf[2]);
	status = PICC_Comm(PCD_TRANSCEIVE, comBuf, 4, comBuf, &unLen);
	
	return status;
}


