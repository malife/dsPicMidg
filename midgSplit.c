#include "midgSplit.h"

#if __IN_DSPIC__ 
	struct CircBuffer midgParseBuffer;
#endif
	CBRef midgBuffer;


void midgParserInit(void){
	// initialize the circular buffer
#if __IN_DSPIC__ 
    midgBuffer = (struct CircBuffer* )&midgParseBuffer;
	newCircBuffer(midgBuffer);
#else
	midgBuffer = newCircBuffer(BSIZE);
#endif
}

void midgSplit (unsigned char * data) {
	
	// Static Variables: CAREFUL
	static unsigned char prevBuffer [2*MAXLOGLEN];
	static unsigned char previousComplete =1;
	static unsigned char indexLast = 0;
	static unsigned char incompleteLen = 0;
	
	// local variables
	unsigned char i;
	unsigned char headerFound=0, noMoreBytes = 1;
	
	// Add the received bytes to the protocol parsing circular buffer
    for(i = 1; i <= data[0]; i += 1 )
	{
		writeBack(midgBuffer, data[i]);
	}
	
	// update the noMoreBytes flag accordingly
    noMoreBytes = (data[0]>0)?0:1;

	while (!noMoreBytes){
		// if the previous message was complete then read from the circular buffer
		// and make sure that you are into the begining of a message
		if(previousComplete){
			while (!headerFound && !noMoreBytes) {
				// move along until you find a 0x81 or run out of bytes
				while (getLength(midgBuffer)>3 && peak(midgBuffer)!=0x81){
					readFront(midgBuffer);
				}
				// if you found a dollar then make sure the next one is an A1
				// You always make sure to have 4 bytes remaining (including
				// BOF so you can read the ID and the length)
				if(getLength(midgBuffer)>3 && peak(midgBuffer) == 0x81){
					// read it
					prevBuffer[indexLast++] = readFront(midgBuffer);
                    // if next is a 0xA1
					if (peak(midgBuffer) == 0xA1){
						// read the sync
						prevBuffer[indexLast++] = readFront(midgBuffer);
						// read the ID
						prevBuffer[indexLast++] = readFront(midgBuffer);
						// read the count
						prevBuffer[indexLast++] = readFront(midgBuffer);
						// start monitoring the pending count
						incompleteLen = prevBuffer[indexLast -1]; 
						// and signal you found a header
						headerFound = 1;
                         // and set as  incomplete the sentece
                         previousComplete = 0;
					}
				} else {
					noMoreBytes = 1;
				} // else no dollar
			} // while we found header && no more bytes
		} // if previous complete
		
		// At this point either you found a header from a previous complete
		// or you are reading from a message that was incomplete the last time
		// in any of those two cases, keep reading until you run out of bytes
		// or incompleteLen == 0		
		
		while ((incompleteLen > 0) && !noMoreBytes){
			while (incompleteLen >0 && getLength(midgBuffer)>2){
				prevBuffer[indexLast++] = readFront(midgBuffer);
				incompleteLen --;
			}
			// if you completed the Length of the payload and you have 
			// at least two bytes to read to get the checksum
			if (incompleteLen == 0 && getLength(midgBuffer)>=2){
				// read the checksum 0
				prevBuffer[indexLast++] = readFront(midgBuffer);
				// read the checksum 1
				prevBuffer[indexLast++] = readFront(midgBuffer);
				// Parse the message. 
				// Note that parse does the checksumming
				midgParse(prevBuffer);
				
				// Reset the variables
				previousComplete =1;
				indexLast = 0;
            	headerFound = 0;
            	memset(prevBuffer, 0, sizeof(prevBuffer));

			} else {
				noMoreBytes =1;
			}
		}
	} // big outer loop
}