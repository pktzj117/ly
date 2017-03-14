#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"


void EXTIX_Init(void);//外部中断初始化	
void EXTIX_Disable(void);

void KEY_Init(void);
void KEY_EXTI_Init(void);
extern u8 buff[10000];
extern vu16 curnum;
extern vu8 flag;
extern u8 key0value;
extern u8 key0flag;
extern u32 key0count;

#endif

