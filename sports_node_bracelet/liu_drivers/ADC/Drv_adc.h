#ifndef __ADC_H
#define __ADC_H	
#include "Gen_include.h"

void  ADC1_Init(void); 				//ADC,DMA通道初始化
u16 Get_Adc(u8 ch); 				  //获得某个通道值 
float measure_T(void);
#endif 















