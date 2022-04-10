#include "Gen_include.h"
#include "Gen_Structconfig.h"

_flag flag;
_pcUp pcUp;
_attitude attitude;

uint8_t  data_buf[9]={0};
int16_t  ROLL=0,PITCH=0,YAW=0;
int16_t  rpy[3]={0},Acc[3]={0},Gyr[3]={0},Mag[3]={0},Q[4]={0};

STime			stcTime;
SAcc 			stcAcc;
SGyro 		stcGyro;
SAngle 		stcAngle;
SMag 			stcMag;
SDStatus 	stcDStatus;
SPress 		stcPress;
SLonLat 	stcLonLat;
SGPSV 		stcGPSV;

_acc acc;
_motion motion;
_heart heart;
uint8_t waveform[128] = {0};				//波形采集数组，采集128个点，OLED的宽度为128个像素
uint8_t waveform_copy[128] = {0};		//波形采集数组，采集128个点，OLED的宽度为128个像素
uint8_t waveform_flag = 0;				//波形采样时间计数，采样128次之后才一次性显示出来
