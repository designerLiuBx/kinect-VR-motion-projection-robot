#include "Drv_key.h"
#include "delay.h" 
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
unsigned char zero[1][16]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
	//������ʼ������
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
 
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	  static u8 key_up=1;//�������ɿ���־
	  if(key_up&&(KEY4==0||KEY1==0||KEY2==0||KEY3==0))
	{  
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0) 
			
		return 1;
		else if(KEY2==0)
			
		return 2;
		else if(KEY3==0)
			
		return 3;
		else if(KEY4==0)
			
		return 4;
	}else if(KEY4==1&&KEY1==1&&KEY2==1&&KEY3==1)key_up=1; 	    
 	return 0;// �ް�������
}

void Key_Run(void)
{
	u8 key;           //�����ֵ
  key=KEY_Scan(0);		//�õ���ֵ
	if(key)
	{						   
		switch(key)
		{				
			case KEY1_PRES:	//����LED0��ת
				if( 2 == flag.display)
				{
					switch(flag.index)        
					{			
						case 1:
							flag.display = 3;
							break;
						case 2:
							flag.tBase += 0.1;
							break;
						case 3:
							motion.steps = 0;	
							motion.dis = 0;	
						
  						OLED_DrawBMP(8*(7),0,8*8,2,zero[0]);
							break;
						case 4:
							break;
					}
				}
				break;
			case KEY2_PRES:	//����LED1��ת
				flag.display = 1;
				OLED_Clear();		
				break;
			case KEY3_PRES:	//ͬʱ����LED0,LED1��ת 
				if( 1 == flag.display)		
				{
					flag.display = 2;
					OLED_Clear();
				}
				if( 2 == flag.display)
						{
							switch(flag.index)        
							{			
								case 1:
									break;
								case 2:
									flag.tBase -= 0.1;
									break;
								case 3:
									break;
								case 4:
									break;
							}
						}
					break;
			case KEY4_PRES:	//���Ʒ�����
				flag.index = (flag.index<4)? flag.index + 1 : 1 ;
				break;
		}
	}
}
