#ifndef __STRUCT_H
#define __STRUCT_H
#include "Gen_include.h"
typedef struct
{
	u8 start_ok;
	u8 run;
	int test;
	int  key1;
	int key2;
	int key3;
	int key4;
	int index;
	int indexDst;
	int display;
	float tBase;
}_flag;
typedef struct
{
	int data1;
}_pcUp;
typedef struct
{
	double pitch;
	double roll;
	double roll_pre;
	double yaw;
	double pitchV;
	double rollV;
}_attitude;

typedef struct
{
	short x;
	short y;
	short z;
	double dst;
}_acc;
typedef struct
{
	int steps;
	int dis;
	int flag;
	int angle;
}_motion;
typedef struct 
{
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
}STime;
typedef struct 
{
	short a[3];
	short T;
}SAcc;
typedef struct 
{
	short w[3];
	short T;
}SGyro;
typedef struct 
{
	short Angle[3];
	short T;
}SAngle;
typedef struct   
{
	short h[3];
	short T;
}SMag;

typedef struct 
{
	short sDStatus[4];
}SDStatus;

typedef struct 
{
	long lPressure;
	long lAltitude;
}SPress;

typedef struct 
{
	long lLon;
	long lLat;
}SLonLat;

typedef struct 
{
	short sGPSHeight;
	short sGPSYaw;
	long lGPSVelocity;
}SGPSV;
typedef struct
{
	int X;
	int XQ;
	int H;
}_heart;
#endif
