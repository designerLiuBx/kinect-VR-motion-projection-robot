#include "UpperComputer_ANO_V7.h"
int ANO_v7_Data[100]={0};
u8 ANO_v7_Data_i=0;
USART_TypeDef * ANO_V7_SendOutPut_uart;
int i;

static void ANO_v7_UartSendData(u8 *dataSend,u8 length)
{
     for( i = 0 ; i < length; i++) 
     {
						while((ANO_V7_SendOutPut_uart->SR&0X40)==0);  
						ANO_V7_SendOutPut_uart->DR = dataSend[i]; 
		 }

}
void ANO_v7_Send_user(u8 Function_frame,int *data_data) 
{
  static u8 ANO_DataSend[100]={0};
  u8  _cnt=0;
	u8  sum=0;
	u8 addcheck=0;
	ANO_DataSend[_cnt++]=0xAA;
	ANO_DataSend[_cnt++]=0XFF;
	ANO_DataSend[_cnt++]=Function_frame;
	ANO_DataSend[_cnt++]=0;
  for( i=0;i<ANO_v7_Data_i;i++)
	{
  ANO_DataSend[_cnt++]=BYTE0(data_data[i]); 
	ANO_DataSend[_cnt++]=BYTE1(data_data[i]);
	ANO_DataSend[_cnt++]=BYTE2(data_data[i]); 
	ANO_DataSend[_cnt++]=BYTE3(data_data[i]);
	}
 
	ANO_DataSend[3]=_cnt-4;
	for( i=0;i<(ANO_DataSend[3]+4);i++)
	{
	  sum+=ANO_DataSend[i];
		addcheck += sum;
	}
	ANO_DataSend[_cnt++]=sum;
  ANO_DataSend[_cnt++]=addcheck;
	ANO_v7_UartSendData(ANO_DataSend,_cnt);
}
