#ifndef __SPI_H
#define __SPI_H
#include "Gen_include.h"

void SPI4_Init(void);			 //��ʼ��SPI3��
void SPI4_SetSpeed(u8 SpeedSet); //����SPI3�ٶ�   
u8 SPI4_ReadWriteByte(u8 TxData);//SPI3���߶�дһ���ֽ�
		 
#endif

