#ifndef _MIDG_H_
#define _MIDG_H_

#include "circBuffer.h"
#include <p33fxxxx.h>
/*
#include "apDefinitions.h"
#include <uart.h>
*/

// select which UART to use 
#define _MIDG_UART1_ 0
#define _MIDG_UART2_ 1

#if _MIDG_UART1_
#define UMODEbits U1MODEbits
#define USTAbits U1STAbits
#define UBRG U1BRG
#define URXIP IPC2bits.U1RXIP
#define URXIF IFS0bits.U1RXIF
#define URXIE IEC0bits.U1RXIE
#define URXREG U1RXREG

#elif _MIDG_UART2_
#define UMODEbits U2MODEbits
#define USTAbits U2STAbits
#define UBRG U2BRG
#define URXIP IPC7bits.U2RXIP
#define URXIF IFS1bits.U2RXIF
#define URXIE IEC1bits.U2RXIE
#define URXREG U2RXREG
#endif

#define MIDGBAUD 115200
#define MIDG_UBRG 21 
// if FCY = 40 000 000

#define MIDG_CHUNKSIZE 100

void midgInit(void);
void midgRead(unsigned char* midgChunk);


#endif /* _MIDG_H_ */
