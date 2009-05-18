// Code by: John B. Burr, based on bufferedUART.c by
// Started 5/17/09

#include "midg.h"

// globals to be set up in init()
struct CircBuffer uartBufferData;  // not to ever be used!
CBRef uartBuffer;   // use the pointer!

// FIXME: still has gps-specific calls and setup in it!
// UART and Buffer initialization
void uartInit (void){
	// initialize the circular buffer
	uartBuffer = (struct CircBuffer*)&uartBufferData;
	newCircBuffer(uartBuffer);
	
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
	USTAbits.URXISEL	= 2;		// RX interrupt when 3 chars are in
	USTAbits.OERR		= 0;		// clear overun error
	
	// UxBRG Register (set by #define in midg.h)
	// ==============
	UBRG = MIDG_UBRGI;			// Set the baud rate for MIDG's default

	// Enable the port;
	UMODEbits.UARTEN	= 1;		// Enable the port
	USTAbits.UTXEN		= 1;		// Enable TX
	
	// wait for the UART to settle
	int i;
	for( i = 0; i < 32700; i += 1 )
	{
		Nop();
	}

	// Configure the GPS sentences and change the baud Rate
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

	// Initialize the Interrupt  
  	URXIP   = 6;    		// Interrupt priority 6  
  	URXIF   = 0;    		// Clear the interrupt flag
  	URXIE   = 1;    		// Enable interrupts
 
  	// Enable the port;
	USTAbits.UTXEN		= 0;		// Disable TX	
  	UMODEbits.UARTEN	= 1;		// Enable the port		
}
