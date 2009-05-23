// Code by: John B. Burr, heavily based on bufferedUART.c
// Started 5/17/09

#include "midg.h"
#include "apDefinitions.h"

// midg.c globals to be set up in init()
struct CircBuffer midgUartBufferData;  // struct should never be used directly!
CBRef midgUartBuffer;   // use the pointer!

// dsPic specific implementation goes in here; the rest can be either on PC or embeded
#if __IN_DSPIC__
    // pick UART1 or UART2 based on midg.h #defines
    #if ((_MIDG_UART1_ == 1) && (_MIDG_UART2_ == 0))
        #warning "just a reminder: midg.c using UART1"
        #define UMODEbits U1MODEbits
        #define USTAbits U1STAbits
        #define UBRG U1BRG
        #define URXIP IPC2bits.U1RXIP
        #define URXIF IFS0bits.U1RXIF
        #define URXIE IEC0bits.U1RXIE
        #define URXREG U1RXREG
        
    #elif ((_MIDG_UART2_ == 1) && (_MIDG_UART1_ == 0))
        #warning "just a reminder: midg.c using UART2"
        #define UMODEbits U2MODEbits
        #define USTAbits U2STAbits
        #define UBRG U2BRG
        #define URXIP IPC7bits.U2RXIP
        #define URXIF IFS1bits.U2RXIF
        #define URXIE IEC1bits.U2RXIE
        #define URXREG U2RXREG
        
    #else
        #error "Check midg.h for UART selection--exactly one must be selected"
    #endif


    // FIXME: still has gps-specific calls and setup in it!
    // UART and Buffer initialization
    void midgInit (void){
        // initialize the circular buffer
        midgUartBuffer = (struct CircBuffer*)&midgUartBufferData;
        newCircBuffer(midgUartBuffer);
        
        // Configure and open the port;
        // UxMODE Register (set by #define in midg.h)
        // ==============
        UMODEbits.UARTEN	= 0;		// Disable the port		
        UMODEbits.USIDL 	= 0;		// Stop on idle
        UMODEbits.IREN		= 0;		// No IR decoder
        UMODEbits.RTSMD	= 0;		// Ready to send mode (irrelevant)
        UMODEbits.UEN		= 0;		// Only RX and TX
        UMODEbits.WAKE		= 1;		// Enable at startup
        UMODEbits.LPBACK	= 0;		// Disable loopback
        UMODEbits.ABAUD	= 0;		// Disable autobaud
        UMODEbits.URXINV	= 0;		// Normal operation (high is idle)
        UMODEbits.PDSEL	= 0;		// No parity 8 bit
        UMODEbits.STSEL	= 0;		// 1 stop bit
        UMODEbits.BRGH 	= 0;		// Low speed mode
        
        // UxSTA Register (set by #define in midg.h)
        // ==============
        //USTAbits.URXISEL	= 2;		// RX interrupt when 3 chars are in
        USTAbits.URXISEL	= 0;		// RX interrupt when just one char comes in
        USTAbits.OERR		= 0;		// clear overun error
        
        // UxBRG Register (set by #define in midg.h)
        // ==============
        UBRG = MIDG_UBRG;			// Set the baud rate for MIDG's default
    
        // Enable the port;
        UMODEbits.UARTEN	= 1;		// Enable the port
        USTAbits.UTXEN		= 1;		// Enable TX
        
        // wait for the UART to settle
        int i;
        for( i = 0; i < 32700; i += 1 )
        {
            Nop();
        }
    
    /*	// Configure the GPS sentences and change the baud Rate
        gpsSentenceConfig();
        
        // Disable the port and TX;
        UMODEbits.UARTEN	= 0;		// Disable the port	
        
        // UBRG Register
        // ==============
        UBRG = UCSCAP_UBRGF;			// Set the baud rate for operation of GPS	
        
        // Enable the port;
        UMODEbits.UARTEN	= 1;		// Enable the port	
        USTAbits.UTXEN		= 1;		// Enable TX
        
        for( i = 0; i < 32700; i += 1 )
        {
            Nop();
        }
        
        // Configure the frequency to 5 Hz
        gpsFreqConfig();
        
        // Disable the port and TX;
        UMODEbits.UARTEN	= 0;		// Disable the port	
    */
        // Initialize the Interrupt  
        URXIP   = 6;    		// Interrupt priority 6  
        URXIF   = 0;    		// Clear the interrupt flag
        URXIE   = 1;    		// Enable interrupts
     
        // Enable the port;
        USTAbits.UTXEN		= 0;		// Disable TX	
        UMODEbits.UARTEN	= 1;		// Enable the port		
    }
    
    // Interrupt service routine for MIDG UART port, selected in midg.h
    #if _MIDG_UART1_
    void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void){
    #elif _MIDG_UART2_
    void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void){
    #endif
        // Read the buffer while it has data
        // and add it to the circular buffer
        while(USTAbits.URXDA == 1){
            writeBack(midgUartBuffer, (unsigned char)URXREG);
        }
        
        // If there was an overun error clear it and continue
        if (USTAbits.OERR == 1){
            USTAbits.OERR = 0;
        }
        
        // clear the interrupt
        URXIF = 0;
    }
       
#else
    void midgInit (void){
        // initialize the circular buffer
        midgUartBuffer = newCircBuffer(BSIZE);
    }
          
#endif


void midgRead(unsigned char* midgChunk) {
    unsigned int tmpLen = getLength(midgUartBuffer);
    unsigned int i;
    
   printf("tmpLen before readFront()s = %u\n", tmpLen);
   
    // midgChunk[0] = bytes in midgChunk after read
    // midgChunk[MIDG_CHUNKSIZE-1] = bytes remaining in buffer after read
    if ( tmpLen > MIDG_CHUNKSIZE - 2 ) {
        // max after front, last removed
        midgChunk[0] = MIDG_CHUNKSIZE - 2;  
        midgChunk[MIDG_CHUNKSIZE-1] = tmpLen - midgChunk[0];
    } else {
        midgChunk[0] = tmpLen;
        // no more bytes will be left in buffer after read (based on tmpLen)
        midgChunk[MIDG_CHUNKSIZE-1] = 0;
    }
    
    for ( i = 1; i <= midgChunk[0]; i++ ) {
        midgChunk[i] = readFront(midgUartBuffer);
    }
   printf("getLength(midgUartBuffer) = %u\n", getLength(midgUartBuffer));
}
