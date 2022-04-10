#include "sys.h"
#include "usart3_it.h"
#include "usart3.h"
#include "string.h"
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
extern uint8_t stata_reg;//接收状态寄存器
extern uint8_t data1_buf[7],data2_buf[7],data3_buf[7],
data4_buf[7],data5_buf[9];
//////////////
/*更据ACC、GYR等不同种数据对应接收，便于后期加工使用*/
///////////
void USART3_IRQHandler(void)
{
	static uint8_t rebuf[250]={0},i=0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		rebuf[i++]=USART_ReceiveData(USART3);
		if(rebuf[0]!=0x55)//判断帧头
		{
			i=0;
			return;
		}
		if(i<11){return ;}
		else
		{
		switch(rebuf[1])
		{
			case 0x50:	memcpy(&stcTime,&rebuf[2],8);break;
			case 0x51:	memcpy(&stcAcc,&rebuf[2],8);break;
			case 0x52:	memcpy(&stcGyro,&rebuf[2],8);break;
			case 0x53:	memcpy(&stcAngle,&rebuf[2],8);break;
			case 0x54:	memcpy(&stcMag,&rebuf[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&rebuf[2],8);break;
			case 0x56:	memcpy(&stcPress,&rebuf[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&rebuf[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&rebuf[2],8);break;
		}
		i=0;
		}
	}
	USART_ClearFlag(USART3,USART_FLAG_RXNE);//清中断标志
}	

