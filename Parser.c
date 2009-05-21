
/*****************************************************************/
/*																 */
/* This program is a parser code for MIDG II.					 */
/* Since 05-14-2009												 */
/* Programmer: HyukChoong Kang									 */
/*																 */
/*****************************************************************/


/*********************************************/
/* 				includes					 */
/*********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "checksum.h"

/*********************************************/
/* 				includes					 */
/*********************************************/

#define PARSEBUFSIZE 5		//MAX payload (77) + 6 frame bytes = 83 bytes, 100 bytes to be safe.
#define MESSAGEBUFSIZE 45	//Message buffer size for specifically for Message ID: 10.


/*********************************************/
/* 				main function				 */
/*********************************************/


int main(int argc, char** argv) {

	unsigned char tempChar;
	unsigned char temp[PARSEBUFSIZE];
	unsigned char message[MESSAGEBUFSIZE];
	int j=0, k=4, l=0, m=0, n=0, msg_corrupted;
	unsigned char checksum0=0, checksum1=0;
	FILE* input;
	
	if ( argc != 2 ) {
		printf("usage: ./result \"input file\"\n");
		exit(1);
	}
	//Check if the input file is valid
	input = fopen( argv[1], "r" );
	if ( !input ) {
		fprintf(stderr, "Can't open input file %s\n", argv[1]);
		exit(1);
	}
	//print of the input file is opened
	printf("successfully opened %s\n", argv[1]);
	
	//read the input file and move it to temperary buffer.
	//after the buffer is read, make three 16 bit buffers.
	//keep moving numbers to the left and see if 0xA1 comes after 0x81.
	/*What about Message ID? I'm supposed to get message ID number 10.  How should I get it? hmm...*/
	/*Is message ID unsigned char? or int, double or what?										   */
	
	// initializing temp[] to all 0xFF 
	for (j=0; j<PARSEBUFSIZE; j++) {			
		temp[j]=0xFF;
		//printf("temp[%d]=%02X\n", j, temp[j]);
	}
	//printf("\n");
	
	while ( (fscanf( input, "%c", &tempChar) == 1) && (tempChar!=EOF) ) {
		
		/*
		 instead of performing this check every time, we should move to something 
		 like the state machine we drew on the board, and only do this expensive check 
		 when we haven't found the message start yet, instead of doing every time, 
		 even though we know we're in the message. 
		 */
		if ( (temp[0]==0x81) && (temp[1]==0xA1) && (temp[2]==0x0A) && (temp[3]==0x27) && (k<MESSAGEBUFSIZE) ) {
			
			//If conditions are satisfied, then move them to my new string and write them.
			//Also, for the next 41 bytes will be attached after sync's, message ID, and a count.
			//After they are moved, the last two bytes would be the checksum.  
			//			printf("Am I here??\n");
			
			//printf("k = %d, tempChar = %02X\n", k, tempChar);
			
/***********************************************************************************/
/*		Only writing the first four bytes once.									   */
/***********************************************************************************/
			if (k <=4) {
			message[0]=temp[0];
			message[1]=temp[1];
			message[2]=temp[2];
			message[3]=temp[3];
			}
/***********************************************************************************/			

			
			

/***********************************************************************************/
/*		Writing incoming bytes after the first four bytes.						   */
/***********************************************************************************/
// increment k before the comparison because once we've recognized SYNC 0 in temp[0]
// we've already got PAYLOAD 1 in temp[4].
			if (++k< MESSAGEBUFSIZE) {
				//printf("\tsetting message[%d]=%02X\n", k, tempChar);
				
				message[k]=tempChar;
//				printf("Counting k =%d\n", k);
				
			}

/***********************************************************************************/	
		}
		

		
//Check all three 0x81, 0xA1, 0x0A, and 0x27.  If they are qualified, then put new line.
//Only Detects Message ID 10, though.  I don't Think I want it this way.
//Checking 'Count' byte as well.  
//NOW, if they are all checked out, then read the read and check 'checksum'.
/***********************************************************************************/
/*		Writing incoming bytes at the first four bytes							   */
/***********************************************************************************/			
		else {
			for (j=0; j<PARSEBUFSIZE-1; j++) {			
				temp[j]=temp[j+1];
				//printf("temp[%d]=%02X\n", j, temp[j]);
			}		
			temp[PARSEBUFSIZE-1]=tempChar;
			//printf("temp[%d]=%02X\n\n", PARSEBUFSIZE, temp[PARSEBUFSIZE]);
			k=4;
		}
/***********************************************************************************/
		
		
		

		
/***********************************************************************************/
/*		Print messages to the screen and check the checksums					   */
/***********************************************************************************/	

		if (k == MESSAGEBUFSIZE ) {
			for (m=0; m<MESSAGEBUFSIZE-1; m++) {
				printf("%02X", message[m]);
			}
			printf("%02X\n", message[m]);
			m=0;							//Printing messages on screen...
			

			for (checksum0=0, checksum1=0, n=2; n <MESSAGEBUFSIZE-2; n++) {
				checksum0=checksum0+message[n];
				checksum1=checksum1+checksum0;
											//Checksum Algorithm...
			}
			
			printf("checksum 0 and checksum1 are..%02X %02X\n",checksum0, checksum1);
			if ((checksum0==message[MESSAGEBUFSIZE-2]) && (checksum1==message[MESSAGEBUFSIZE-1])) {
				printf("The packet is valid...\n");
			}

			
/***********************************************************************************/
/*		Message Corruption Detection Function									   */
/***********************************************************************************/	

			for (l=8, msg_corrupted=0; l< MESSAGEBUFSIZE-1 || msg_corrupted==1;l++) {
				if ((message[l-3]==0x81) && (message[l-2]==0xA1) && (message[l-1]==0x0A) && 
					(message[l]==0x27) && (l<MESSAGEBUFSIZE-2)) {
					if ((checksum0!=message[MESSAGEBUFSIZE-2]) || (checksum1!=message[MESSAGEBUFSIZE-1])) {
						message[0]=message[l-3];
						message[1]=message[l-2];
						message[2]=message[l-1];
						message[3]=message[l];
						msg_corrupted=1;
						printf("Possibly some of payloads of the packet is corrupted or dropped out. Re-writing from the beginning... \n");
					}
				}
			}
		}
		//printing the only message that I want. 
/***********************************************************************************/		
	}
	
	fclose(input);
	exit(0);
}

