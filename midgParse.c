#include "midgParse.h"


void midgParse(unsigned char * sentence) {
	tMidgData midgControlData;
		
	if (getChecksum(sentence[3]+6, sentence)){
		switch (sentence[3]){
			case NAV_PV:
			
			break;
			case NAV_SENSOR:
				// timestamp
				midgControlData.timeStamp.chData[0] = sentence[0];
				midgControlData.timeStamp.chData[1] = sentence[1];
				midgControlData.timeStamp.chData[2] = sentence[2];
				midgControlData.timeStamp.chData[3] = sentence[3];
				// Rates
				midgControlData.p.chData[0] 		= sentence[4];
				midgControlData.p.chData[1] 		= sentence[5];
				midgControlData.q.chData[0] 		= sentence[6];
				midgControlData.q.chData[1] 		= sentence[7];
				midgControlData.r.chData[0] 		= sentence[8];
				midgControlData.r.chData[1] 		= sentence[9];
				// Accels
				midgControlData.ax.chData[0] 		= sentence[10];
				midgControlData.ax.chData[1] 		= sentence[11];
				midgControlData.ay.chData[0] 		= sentence[12];
				midgControlData.ay.chData[1] 		= sentence[13];
				midgControlData.az.chData[0] 		= sentence[14];
				midgControlData.az.chData[1] 		= sentence[15];
				// Euler
				midgControlData.yaw.chData[0] 		= sentence[16];
				midgControlData.yaw.chData[1] 		= sentence[17];
				midgControlData.pitch.chData[0] 	= sentence[18];
				midgControlData.pitch.chData[1] 	= sentence[19];
				midgControlData.roll.chData[0] 		= sentence[20];
				midgControlData.roll.chData[1] 		= sentence[21];
				// Quaternions
				midgControlData.qw.chData[0] 		= sentence[22];
				midgControlData.qw.chData[1] 		= sentence[23];
				midgControlData.qw.chData[2] 		= sentence[24];
				midgControlData.qw.chData[3] 		= sentence[25];
				midgControlData.qx.chData[0] 		= sentence[26];
				midgControlData.qx.chData[1] 		= sentence[27];
				midgControlData.qx.chData[2] 		= sentence[28];
				midgControlData.qx.chData[3] 		= sentence[29];
				midgControlData.qy.chData[0] 		= sentence[30];
				midgControlData.qy.chData[1] 		= sentence[31];
				midgControlData.qy.chData[2] 		= sentence[32];
				midgControlData.qy.chData[3] 		= sentence[33];
				midgControlData.qz.chData[0] 		= sentence[34];
				midgControlData.qz.chData[1] 		= sentence[35];
				midgControlData.qz.chData[2] 		= sentence[36];
				midgControlData.qz.chData[3] 		= sentence[37];
				// flags
				midgControlData.flags 				= sentence[38];
				
			break;
		}
	} else {
		printf("=========== Checksum Failed!\n\r");
	}
	printMidgData(midgControlData);
}

unsigned char getChecksum(unsigned char msgLen, unsigned char * msg){
	unsigned char checksum0 = 0, checksum1 = 0, n;
	
	for (n=2; n <msgLen-2; n++) {
		checksum0=checksum0+msg[n];
		checksum1=checksum1+checksum0;
	}
	
	return (checksum0==msg[msgLen-1] && checksum1==msg[msgLen]);
}

void printMidgData (tMidgData midg2Print) {
	printf("\n\r%d\t%f\t%f\t%f", midg2Print.timeStamp.uiData,
			midg2Print.p.shData*100*DEG2RAD,
			midg2Print.q.shData*100*DEG2RAD,
			midg2Print.r.shData*100*DEG2RAD);
}