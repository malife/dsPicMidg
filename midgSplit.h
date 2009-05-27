#ifndef MIDGSPLIT_H
#define MIDGSPLIT_H

#include "apDefinitions.h"
#include "circBuffer.h"
#include "midgParse.h"
#include <string.h>

#ifdef __cplusplus
       extern "C"{
#endif

	void midgSplit (unsigned char * data);
	void midgParserInit(void);

#ifdef __cplusplus
       }
#endif

#endif /* end of include guard: MIDGSPLIT_H */
