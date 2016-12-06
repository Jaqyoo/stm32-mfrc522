#ifndef PICC_APP
#define PICC_APP

#include "picc.h"

#define PICC_UDF_HALT				1
#define PICC_UDF_REQUEST		2
#define PICC_UDF_ANTICOLL		3
#define PICC_UDF_SELECT			4
#define PICC_UDF_AUTHSTATE	5
#define PICC_UDF_READ				8
#define PICC_UDF_WRITE			9
#define PICC_UDF_READ_OK		17

extern uint8_t RevBuffer[30]; 

PiccStatus_t PICC_UDF_Cmd();

#endif
