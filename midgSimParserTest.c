#include <stdio.h>
#include <stdlib.h>

#include "midgSim.h"
#include "midg.h"

int main(int argc, char** argv) {

	if ( argc != 2 ) {
		printf("usage: ./result \"input file\"\n");
		exit(1);
	}
   
   midgInit();
   
   long int size = midgSimGetInput( argv[1] );
   
   int i;
   int bytesRead = 0;
   unsigned char midgReadBuf[MIDG_CHUNKSIZE];
   while ( bytesRead < size ) {
      midgSimStep();
      
      midgRead(midgReadBuf);
      
      printf("%u:", midgReadBuf[0]);
      for ( i = 1; i <= midgReadBuf[0]; i++ ) {
         printf("%02X", midgReadBuf[i]);
      }
      bytesRead += midgReadBuf[0];
      
      printf(" (%u remaining in buffer)\n", midgReadBuf[MIDG_CHUNKSIZE-1]);
      printf("%d bytes read so far.\n", bytesRead);
   }
   
   // FIXME: somehow this code gets more than was in the input file.
   //        midgRead() is returning more bytes than it should: up to the rest
   //        of the last message following the last bytes "9BC62D" is filled
   //        with 0s.
   printf("%d bytes read.  now exiting.\n", bytesRead);
   exit(EXIT_SUCCESS);
}

