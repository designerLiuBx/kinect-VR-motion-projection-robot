#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "Gen_include.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void dev_SysTick_init(void);
u32 GetTimingTick(void);
void Task_SysTick(void);
void dsp_cmd(void);
void TIM3_Int_Init(u16 arr,u16 psc);


#endif
