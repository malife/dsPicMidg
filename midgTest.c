#include <stdlib.h>
#include <stdarg.h>
#include <uart.h>

#include "midgTest.h"
#include "midg.h"
#include "circBuffer.h"

#include "bufferedUART.h"

void printToUart2 (const char *fmt, ...){
	va_list ap;
	char buf [300];
	
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end (ap);
	putsUART2((unsigned int*)buf);
    while(BusyUART2());	
}

void printToUart1 (const char *fmt, ...){
	va_list ap;
	char buf [300];
	
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end (ap);
	putsUART1((unsigned int*)buf);
    while(BusyUART1());	
}

void copyMidgToUart2() {
    unsigned char buf[MIDG_CHUNKSIZE];
    
    midgRead(buf);
    
    // add NUL terminator after bytes
    buf[buf[0]+1] = 0;
    
    // should send the bytes as they came in
    //printToUart2("%s", buf[1]);
    
    // send the number of bytes read from the buffer
    printToUart2("%u\n", buf[0]);
}

void copyMidgToUart1() {
    unsigned char buf[MIDG_CHUNKSIZE];
    int i;
    
    midgRead(buf);
    
    /*
    // add NUL terminator after bytes
    buf[buf[0]+1] = 0;
    
    // send the number of bytes read from the buffer
    printToUart1("%u:<%s>\r\n\0", buf[0], &buf[1]);
    */
    
    printToUart1("%u:<", buf[0]);
    for ( i = 1; i <= buf[0]; i++ ) {
        putcUART1(buf[i]);
        while(BusyUART1());
    }
    printToUart1(">\r\n\0");
}
