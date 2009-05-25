#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "midgSim.h"
#include "midg.h"

#define MIN(x,y) (x < y ? x : y)

// sleazy globals...
extern CBRef midgUartBuffer; // declared in midg.c

unsigned char* entireInput;   
long int entireInputSize;

long int readIndex;

long int midgSimGetInput ( char* inputFileName ) {
	FILE* input = fopen( inputFileName, "r" );
	if ( !input ) {
		fprintf(stderr, "Can't open input file \"%s\"\n", inputFileName);
		exit(EXIT_FAILURE);
	}
	printf("successfully opened %s\n", inputFileName);
   
   // try to move to end of file
   if ( fseek( input, 0, SEEK_END ) == 0 ) {
      // successfully set to end
      entireInputSize = ftell( input );
      printf("%s is %ld bytes long\n", inputFileName, entireInputSize);
      rewind(input); // reset to beginning
   } else {
      fprintf(stderr, "couldn't move to end of %s\n", inputFileName);
      fclose(input);
      exit(EXIT_FAILURE);
   }
   
   entireInput = (unsigned char*)malloc(entireInputSize*sizeof(unsigned char));
   long int currentPos = 0;
   unsigned char tempChar;
   while ( (currentPos < entireInputSize) && (fscanf( input, "%c", &tempChar ) == 1) && (tempChar!=EOF) ) {
      entireInput[currentPos] = tempChar;
      currentPos++;
   }
   fclose(input);
   
   if ( currentPos != entireInputSize ) {
      fprintf(stderr, "not everything in %s was read in: only %ld bytes read\n", inputFileName, entireInputSize);
      exit(EXIT_FAILURE);
   }

   printf("entireInput:\n");
   for ( currentPos = 0; currentPos < entireInputSize; currentPos++ ) {
      printf("%02X", entireInput[currentPos]);
   }
   printf("\n\n");

   readIndex = 0;
   
   //srand(100);
   srand( time(NULL) ); //use this for better randomness
   

   return entireInputSize;
}

// copy some bytes from entireInput to midgUartBuffer
void midgSimStep(void) {
   int offset;
   int remaining = entireInputSize - readIndex;
   int bytesToRead = (rand()%(2*MIDG_CHUNKSIZE))-(MIDG_CHUNKSIZE/2);
   
   // select the smaller of the number remaining and a random number
   // between -1/2 MIDG_CHUNKSIZE and 3/2 MIDG_CHUNKSIZE.  
   // when this is negative, we return early, not enqueuing any bytes.
   bytesToRead = MIN(remaining, bytesToRead);
   
   if ( bytesToRead <= 0 ) {
      // nothing to do :-(
      return;
   }
   
   for ( offset = 0; offset < bytesToRead; offset++ ) {
      writeBack( midgUartBuffer, entireInput[readIndex + offset] );
   }
   readIndex += bytesToRead;
}