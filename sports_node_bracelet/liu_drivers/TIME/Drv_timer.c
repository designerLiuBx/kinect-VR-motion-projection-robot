#include "Drv_timer.h"
#include "Gen_include.h"
volatile u32 g_TimingTick = 0;
volatile u32 g_TimingTickOld = 0;
extern uint8_t  data_buf[9];
extern int16_t  ROLL,PITCH,YAW;
extern int16_t  rpy[3],Acc[3],Gyr[3],Mag[3],Q[4];
//ͨ�ö�ʱ��2�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void dev_SysTick_init(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM3ʱ��

        /* Time base configuration */
        TIM_TimeBaseStructure.TIM_Period = 65535;               
        TIM_TimeBaseStructure.TIM_Prescaler = 9000-1; 
        TIM_TimeBaseStructure.TIM_ClockDivision =  TIM_CKD_DIV1;    
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
        
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
        
        TIM_SetCounter(TIM2, 0);

        /* TIM enable counter */
        TIM_Cmd(TIM2, ENABLE);
}
u32 GetTimingTick(void)
{
        return g_TimingTick;
}

void Task_SysTick(void)
{
  u32 temp = TIM_GetCounter(TIM2);
	if (temp > 10000)
	{
	 TIM_SetCounter(TIM2, 0);
	 g_TimingTickOld = g_TimingTickOld + temp;
	 temp = 0;
	}
  g_TimingTick = g_TimingTickOld + temp;
	
/**********************************����ŷ��������*************************/
		switch(CHeck(data_buf))//����������ݽ�����ϣ����������ָ��
		{
			
			case 0x15:{//�������������
					Acc[0]=(data_buf[0]<<8)|data_buf[1];
					Acc[1]=(data_buf[2]<<8)|data_buf[3];
					Acc[2]=(data_buf[4]<<8)|data_buf[5];
					send_out(Acc,3,0x15);
		}
			break;
			case 0x25:{//�������������
					Gyr[0]=(data_buf[0]<<8)|data_buf[1];
					Gyr[1]=(data_buf[2]<<8)|data_buf[3];
					Gyr[2]=(data_buf[4]<<8)|data_buf[5];
				  attitude.pitchV=Gyr[0]*0.01;
				  attitude.rollV=Gyr[1]*0.01;
					send_out(Gyr,3,0x25);
			}
				break;
			case 0x35:{//�ų��������
					Mag[0]=(data_buf[0]<<8)|data_buf[1];
					Mag[1]=(data_buf[2]<<8)|data_buf[3];
					Mag[2]=(data_buf[4]<<8)|data_buf[5];
					send_out(Mag,3,0x35);
			}
				break;
			case 0x45:{//ŷ�����������
					ROLL=(data_buf[0]<<8)|data_buf[1];   //��ת
					PITCH=(data_buf[2]<<8)|data_buf[3];  //����
					YAW=(data_buf[4]<<8)|data_buf[5];    //����
					rpy[0]=ROLL;
					rpy[1]=PITCH;
					rpy[2]=YAW;
					//attitude.roll = ROLL-18000;
				  //attitude.roll = (attitude.roll>2000||attitude.roll<-2000)? attitude.roll_pre :attitude.roll;
					attitude.pitch = 0.1*(int)(PITCH*0.1);
					attitude.roll  = 0.1*(int)((ROLL)*0.1);
					attitude.roll  = (attitude.roll>0)? (180-attitude.roll):-(180+attitude.roll);
				
				  //attitude.roll_pre=attitude.roll;
					send_out(rpy,3,0x45);
			}
				break;
			case 0x65:{//��Ԫ���������
					Q[0]=(data_buf[0]<<8)|data_buf[1];
					Q[1]=(data_buf[2]<<8)|data_buf[3];
					Q[2]=(data_buf[4]<<8)|data_buf[5];
					Q[3]=(data_buf[6]<<8)|data_buf[7];
					send_out(Q,4,0x65);
			}
				break;
			default:break;
		}	
	
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	TIM_TimeBaseStructure.TIM_Period=arr;
	TIM_TimeBaseStructure.TIM_Prescaler=psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
 

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
 
 
	TIM_Cmd(TIM3, ENABLE);		 
}
 

void TIM3_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
		Waveform_deal(heart.X);
			
		}
} 


