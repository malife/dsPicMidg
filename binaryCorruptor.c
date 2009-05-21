/* Just a quick program to once in a while corrupt a message by inverting
 * all the bits in a random byte.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
   unsigned char tempChar;
   FILE* input;
   FILE* output; 
   
   if ( argc != 3 ) {
      printf("usage: ./a.out \"input file\" \"output file\"\n");
      exit(1);
   }
   
   input = fopen( argv[1], "r" );
   if ( !input ) {
      fprintf(stderr, "Can't open input file %s\n", argv[1]);
      exit(1);
   }
   
   output = fopen( argv[2], "w+" );
   if ( !output ) {
      fprintf(stderr, "Can't open output file %s\n", argv[2]);
      exit(1);
   }

   printf("successfully opened %s and %s\n", argv[1], argv[2]);
   
   //srand( time(NULL) ); use this for better randomness
   srand(100);
   
   while ( (fscanf( input, "%c", &tempChar ) == 1) && (tempChar!=EOF) ) {
      printf("%02X", tempChar);
      if ( rand()%100 == 0 ) { // using the modulo isn't great, but good enough
         // roughly 1 in 100 times, invert all the bits in a byte
         fprintf( output, "%c", ~tempChar );
      } else {
         fprintf( output, "%c", tempChar );
      }
   }
   
   fclose(output);
   fclose(input);
   exit(0);
}