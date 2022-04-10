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
uint8_t waveform[128] = {0};				//���βɼ����飬�ɼ�128���㣬OLED�Ŀ��Ϊ128������
uint8_t waveform_copy[128] = {0};		//���βɼ����飬�ɼ�128���㣬OLED�Ŀ��Ϊ128������
uint8_t waveform_flag = 0;				//���β���ʱ�����������128��֮���һ������ʾ����
