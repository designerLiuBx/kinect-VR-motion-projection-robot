#ifndef _UPPERCOMPUTER_ANO_V7_H__
#define _UPPERCOMPUTER_ANO_V7_H__
#include "sys.h"
#include "Gen_include.h"
//���ݷ����궨�壬�ڷ��ʹ���һ���ֽ�ʱ������int16��float32_t����Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)   (*((char* )(&dwTemp) ))
#define BYTE1(dwTemp)   (*((char* )(&dwTemp)+1))
#define BYTE2(dwTemp)   (*((char* )(&dwTemp)+2))
#define BYTE3(dwTemp)   (*((char* )(&dwTemp)+3))
#define ANO_V7 0X03
extern USART_TypeDef * ANO_V7_SendOutPut_uart;
extern u8 ANO_v7_Data_i;
extern int ANO_v7_Data[100];
static void ANO_v7_UartSendData(u8 *dataSend,u8 length);
void ANO_v7_Send_user(u8 Function_frame,int *data_data) ;
#endif
