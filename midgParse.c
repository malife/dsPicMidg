#include "midgParse.h"


void midgParse(unsigned char * sentence) {
	tMidgData midgControlData;
		
	if (getChecksum(sentence[3]+6, sentence)){
		switch (sentence[2]){
			case NAV_PV:
			
			break;
			case NAV_SENSOR:
				// timestamp
				midgControlData.timeStamp.chData[3] = sentence[4];
				midgControlData.timeStamp.chData[2] = sentence[5];
				midgControlData.timeStamp.chData[1] = sentence[6];
				midgControlData.timeStamp.chData[0] = sentence[7];
				// Rates                                       
				midgControlData.p.chData[1] 		= sentence[8];
				midgControlData.p.chData[0] 		= sentence[9];
				midgControlData.q.chData[1] 		= sentence[10];
				midgControlData.q.chData[0] 		= sentence[11];
				midgControlData.r.chData[1] 		= sentence[12];
				midgControlData.r.chData[0] 		= sentence[13];
				// Accels                                      1
				midgControlData.ax.chData[1] 		= sentence[14];
				midgControlData.ax.chData[0] 		= sentence[15];
				midgControlData.ay.chData[1] 		= sentence[16];
				midgControlData.ay.chData[0] 		= sentence[17];
				midgControlData.az.chData[1] 		= sentence[18];
				midgControlData.az.chData[0] 		= sentence[19];
				// Euler                                       1
				midgControlData.yaw.chData[1] 		= sentence[21];
				midgControlData.yaw.chData[0] 		= sentence[22];
				midgControlData.pitch.chData[1] 	= sentence[23];
				midgControlData.pitch.chData[0] 	= sentence[24];
				midgControlData.roll.chData[1] 		= sentence[25];
				midgControlData.roll.chData[0] 		= sentence[26];
				// Quaternions                                 2
				midgControlData.qw.chData[3] 		= sentence[27];
				midgControlData.qw.chData[2] 		= sentence[28];
				midgControlData.qw.chData[1] 		= sentence[29];
				midgControlData.qw.chData[0] 		= sentence[30];
				midgControlData.qx.chData[3] 		= sentence[31];
				midgControlData.qx.chData[2] 		= sentence[32];
				midgControlData.qx.chData[1] 		= sentence[33];
				midgControlData.qx.chData[0] 		= sentence[34];
				midgControlData.qy.chData[3] 		= sentence[35];
				midgControlData.qy.chData[2] 		= sentence[36];
				midgControlData.qy.chData[1] 		= sentence[37];
				midgControlData.qy.chData[0] 		= sentence[38];
				midgControlData.qz.chData[3] 		= sentence[39];
				midgControlData.qz.chData[2] 		= sentence[40];
				midgControlData.qz.chData[1] 		= sentence[41];
				midgControlData.qz.chData[0] 		= sentence[42];
				// flags                                       4
				midgControlData.flags 				= sentence[43];
				
			break;
		}
		printMidgData(midgControlData);	
	} else {
		printf("\n\r=========== Checksum Failed!");
		printf("\n\r Message ID: %d, Length: %d", sentence[2], sentence[3] );
	}

}

unsigned char getChecksum(unsigned char msgLen, unsigned char * msg){
	unsigned char checksum0 = 0, checksum1 = 0, n;
	
	for (n = 2; n < msgLen-2; n++) {
		checksum0 = checksum0 + msg[n];
		checksum1 = checksum1 + checksum0;
	}
	
	return (checksum0==msg[msgLen-2] && checksum1==msg[msgLen-1]);
}

void printMidgData (tMidgData midg2Print) {
	printf("\n\r%d\t%f\t%f\t%f", midg2Print.timeStamp.uiData,
			midg2Print.p.shData*100.0*DEG2RAD,
			midg2Print.q.shData*100.0*DEG2RAD,
			midg2Print.r.shData*100.0*DEG2RAD);
}