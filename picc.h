#ifndef PICC_H
#define PICC_H

#include "pcd.h"
#include "string.h"

#define PICC_REQIDL           0x26
#define PICC_REQALL           0x52
#define PICC_ANTICOLL1        0x93
#define PICC_ANTICOLL2        0x95
#define PICC_AUTHENT1A        0x60
#define PICC_AUTHENT1B        0x61
#define PICC_READ             0x30
#define PICC_WRITE            0xA0
#define PICC_DECREMENT        0xC0
#define PICC_INCREMENT        0xC1
#define PICC_RESTORE          0xC2
#define PICC_TRANSFER         0xB0
#define PICC_HALT             0x50        

#define PICC_OK					0
#define PICC_NOTAGERR		1
#define PICC_ERR				2

#define PICC_REQ_ISO		0X52
#define PICC_REQ_ALL		0x26

#define PICC_AUTH_A			0X60
#define PICC_AUTH_B			0X61



typedef uint8_t PiccStatus_t;

extern uint8_t MLastSelectedSnr[4];
extern unsigned char DefaultKey[6];

PiccStatus_t PICC_Comm(uint8_t PcdCommand, uint8_t *pInData, uint8_t inLen, uint8_t *pOutData, uint16_t *pOutLenBit);
PiccStatus_t PICC_Request(uint8_t req_cod, uint8_t *pTagType);
PiccStatus_t PICC_Anticoll(uint8_t *pSnr);
PiccStatus_t PICC_Select(uint8_t *pSnr);
PiccStatus_t PICC_AuthState(uint8_t auth_mode, uint8_t addr, uint8_t *pKey, uint8_t *pSnr);
PiccStatus_t PICC_Write(uint8_t addr, uint8_t *pData);
PiccStatus_t PICC_Read(uint8_t addr, uint8_t *pData);
PiccStatus_t PICC_Halt(void);



#endif
