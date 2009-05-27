#include <stdio.h>
#include <stdlib.h>

#include "midgSim.h"
#include "midg.h"
#include "midgSplit.h"

int main(int argc, char** argv) {

	if ( argc != 2 ) {
		printf("usage: ./result \"input file\"\n");
		exit(1);
	}
   
    /*******************************************/
    //sandbox just for testing...
    
    // testing checksum correctness by reproducing a message from the MIDG.
    // checksum = 0x33 0xBF
    //unsigned char test[] = MIDG_MSG(0x0A,0x27,0x00,0x00,0x7E,0xF4,0x00,0x2C,0x00,0x21,0x00,0x46,0xFF,0xF2,0x00,0x12,0xFC,0x27,0x01,0x55,0xFF,0xD7,0xFF,0x61,0x3F,0xF7,0x0C,0x40,0xFF,0x1D,0x60,0xCD,0xFF,0xBD,0xC0,0x89,0x01,0xE7,0xA5,0xEE,0x00);
    
/*    unsigned char test[] = MSG_DIV(2,50);
    
    int z, y;
    printf("test message: ");
    for ( z = 0; z < test[3]+6; z++ ) {
        printf("%02X ", (unsigned int)test[z]);
    }
    printf("\n");
    
    midgMsgAppendChecksum( test );
    // end sandbox
 */   
   midgInit();
   midgParserInit();
   
   long int size = midgSimGetInput( argv[1] );
   
   int i;
   int bytesRead = 0;
   unsigned char midgReadBuf[MIDG_CHUNKSIZE];
   while ( bytesRead < size ) {
      midgSimStep();
      
      midgRead(midgReadBuf);
      
	  midgSplit(midgReadBuf);
	
/*
      printf("%u:", midgReadBuf[0]);
      for ( i = 1; i <= midgReadBuf[0]; i++ ) {
         printf("%02X", midgReadBuf[i]);
      }
      bytesRead += midgReadBuf[0];
      
      printf(" (%u remaining in buffer)\n", midgReadBuf[MIDG_CHUNKSIZE-1]);
      printf("%d bytes read so far.\n", bytesRead);
*/
   }
   
   printf("%d bytes read.  now exiting.\n", bytesRead);
   exit(EXIT_SUCCESS);
}

