#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "Gen_include.h"

#define UART2_REC_NUM  			200  	//定义最大接收字节数 200
void usart2_init(u32 bound);

#endif

