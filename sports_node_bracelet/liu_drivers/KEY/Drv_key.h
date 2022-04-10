#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
#include "Gen_include.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY1 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) //PE4
#define KEY2 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)	//PE3 
#define KEY3 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) //PE2
#define KEY4 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)	//PA0


/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/


#define KEY1_PRES 	1
#define KEY2_PRES	2
#define KEY3_PRES	3
#define KEY4_PRES   4

void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8);  		//����ɨ�躯��	
void Key_Run(void);
#endif
