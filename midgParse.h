#ifndef MIDGPARSE_H
#define MIDGPARSE_H

#include "apDefinitions.h"
#include <stdio.h>

#define NAV_PV		12
#define NAV_SENSOR	10

typedef struct tMidgData{
	tUnsignedIntToChar		timeStamp;
	tShortToChar			p;
	tShortToChar			q;
	tShortToChar			r;
	tShortToChar			ax;
	tShortToChar			ay;
	tShortToChar			az;
	tShortToChar			yaw;
	tShortToChar			pitch;
	tShortToChar			roll;
	tIntToChar				qw;
	tIntToChar				qx;
	tIntToChar				qy;
	tIntToChar				qz;
	char					flags;
}tMidgData;

#ifdef __cplusplus
       extern "C"{
#endif

	unsigned char getChecksum(unsigned char msgLen, unsigned char * msg);
	void midgParse(unsigned char * sentence);
	void printMidgData (tMidgData midg2Print);

#ifdef __cplusplus
       }
#endif


#endif /* end of include guard: MIDGPARSE_H */
