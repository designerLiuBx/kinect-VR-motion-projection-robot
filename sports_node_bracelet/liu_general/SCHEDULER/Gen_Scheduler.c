#include "Gen_Scheduler.h"
#define ARRAYSIZE(a) (sizeof(a) / sizeof((a)[0]))
extern char APP_wave_form=0;
extern char APP_heart_rate=0;
extern char APP_Steps=0;
extern float APP_temperature=0;
extern float APP_distance=0;
static void Task1(void)	//2msִ��һ��
{
	mode_check();
	main_controller();

	//Waveform_deal(heart.X);
	//heart.XQ = heart.X; 
	//APP_packet();
	motion.angle =stcAngle.Angle[0];
	ANO_v7_Data_i = 9;
	ANO_v7_Data[0] = heart.X;	//ANO_v7_Data[0] = stcAngle.Angle[2];
	ANO_v7_Data[1] = heart.H;
	ANO_v7_Data[2] = acc.dst;
	APP_wave_form=heart.X;
	APP_heart_rate=heart.H;
	APP_Steps=motion.steps;
	APP_distance=motion.dis/100.00;
	APP_temperature=measure_T();
	APP_packet();
		//	motion.steps ;
			//motion.dis += 50;
	ANO_v7_Data[3] = motion.angle;
//	ANO_v7_Send_user(0xf1,ANO_v7_Data);	
}
static void Task2(void)	//20msִ��һ��
{
/***************************��λ������***************************/
/*	if (pcUp.data1 > 64)
		pcUp.data1 = -64;
	else pcUp.data1 ++;	
  ANO_v7_Data_i=7;*/
	

/*****************************************************************/
}

static void Task3(void)
{
}

static void Task4(void)	//200msִ��һ��
{
	LED0=!LED0;
}	

//ϵͳ�������ã�������ִͬ��Ƶ�ʵġ��̡߳�
static sched_task_t sched_tasks[] = 
{
	{Task_SysTick,    0, 0},
	{Task1,      20	, 0},
	{Task2,      20, 0},
	{Task3,     1000, 0},
	{Task4,    5000, 0},		//500ms
};
void Scheduler_Run(void)
{
	u32 index = 0;
	//��ʼ�������
  for (index = 0; index < ARRAYSIZE(sched_tasks); index++)
	{
		if (sched_tasks[index].uNextTick <= GetTimingTick())
		{	
     sched_tasks[index].uNextTick += sched_tasks[index].uLenTick;
     sched_tasks[index].task_func();
		}
  }
}
