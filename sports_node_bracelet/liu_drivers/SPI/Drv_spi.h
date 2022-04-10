#ifndef __SPI_H
#define __SPI_H
#include "Gen_include.h"

void SPI4_Init(void);			 //初始化SPI3口
void SPI4_SetSpeed(u8 SpeedSet); //设置SPI3速度   
u8 SPI4_ReadWriteByte(u8 TxData);//SPI3总线读写一个字节
		 
#endif

