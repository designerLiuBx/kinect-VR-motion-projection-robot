#ifndef __GINCLUDE_H
#define __GINCLUDE_H	

#include <math.h>
#include "sys.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "usart3.h"
#include "usart2.h"
#include "usart3_it.h"
#include "usart4.h"	
#include "delay.h"
#include "string.h"
#include "stdlib.h"

#include "All_init.h"
#include "Gen_Structconfig.h"
#include "Gen_Scheduler.h"

#include "control_oled.h"
#include "motion_comp.h"

#include "Drv_I2c.h"
#include "Drv_OLED.h"
#include "Drv_led.h" 
#include "Drv_key.h"
#include "Drv_timer.h"
#include "Drv_ADS1292.h"
#include "Drv_spi.h"
#include "Drv_adc.h"
#include "UpperComputer_ANO_V7.h"

#include "math.h"
#include "arm_math.h"

//=======================================
/**************参数定义*****************/
extern _flag flag;
extern _pcUp pcUp;

extern _attitude attitude;
extern uint8_t  data_buf[9];
extern int16_t  ROLL,PITCH,YAW;
extern int16_t  rpy[3],Acc[3],Gyr[3],Mag[3],Q[4];

extern STime		stcTime;
extern SAcc 		stcAcc;
extern SGyro 		stcGyro;
extern SAngle 	stcAngle;
extern SMag 		stcMag;
extern SDStatus stcDStatus;
extern SPress 	stcPress;
extern SLonLat 	stcLonLat;
extern SGPSV 		stcGPSV;

extern _acc acc;
extern _motion motion;
extern _heart heart;

extern uint8_t waveform[128];				//波形采集数组，采集128个点，OLED的宽度为128个像素
extern uint8_t waveform_copy[128];		//波形采集数组，采集128个点，OLED的宽度为128个像素
extern uint8_t waveform_flag;				//波形采样时间计数，采样128次之后才一次性显示出来
extern float32_t  ECG_Signal_inputF32, ECG_Signal_outputF32;
extern float32_t  JY61_Signal_inputF32, JY61_Signal_outputF32;
extern arm_fir_instance_f32 ECG_S,JY61_S;
	
//=======================================
/***************中断优先级******************/
#define NVIC_GROUP NVIC_PriorityGroup_3		//中断分组选择

#define NVIC_TIME_P       0					//定时器中断配置，暂未使用
#define NVIC_TIME_S       0

#define NVIC_UART1_P			3			//串口1中断配置 //gps
#define NVIC_UART1_S			0
/***********************************************/

#endif
