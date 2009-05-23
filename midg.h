#ifndef _MIDG_H_
#define _MIDG_H_

#include "circBuffer.h"
#include <p33fxxxx.h>
/*
#include "apDefinitions.h"
#include <uart.h>
*/

// select which UART to use by setting exactly 
// one of the below to 1, the other(s) to 0
#define _MIDG_UART1_ 0
#define _MIDG_UART2_ 1

#define MIDGBAUD 115200
#define MIDG_UBRG 21 
// if FCY = 40 000 000

#define MIDG_CHUNKSIZE 100

void midgInit(void);
void midgRead(unsigned char* midgChunk);


#endif /* _MIDG_H_ */
