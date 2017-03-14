#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

typedef struct {
    float   qdSum;          // 1.31 format
    float   qKp;
    float   qKi;
    float   qKc;
    float   qOutMax;
    float   qOutMin;
    float   qInRef; 
    float   qInMeas;
    float   qOut;
    } tPIParm;

void CalcPI( tPIParm *pParm);
void InitPI( tPIParm *pParm);		
#endif
