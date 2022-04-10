#ifndef __LED_H
#define __LED_H
#include "Gen_include.h"

////////////////////////////////////////////////////////////////////////////////////	

//LED端口定义
#define LED0 PBout(9)	 
#define LED1 PBout(8)	 
#define LED2 PBout(5)	  

//函数声明
void LED_Init(void);//初始化	

#endif
