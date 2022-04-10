#include "sys.h"
#include "usart3.h"	
#include "string.h"
typedef unsigned char uint8;
typedef unsigned int uint32;
void usart3_init(u32 bound)
{
    GPIO_InitTypeDef  GPIO_InitStructe;
    USART_InitTypeDef  USART_InitStructe;
  	NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);     //使能串口外设
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);		 	//使能输出端口

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);   //连接端口与外设
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);   //连接端口与外设

 
	 
    GPIO_InitStructe.GPIO_Mode=GPIO_Mode_AF;                  //模式设置为复用
    GPIO_InitStructe.GPIO_OType=GPIO_OType_PP;                //强制推挽输出提高数据准确性
    GPIO_InitStructe.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_10;                     //输出引脚为5和6
    GPIO_InitStructe.GPIO_PuPd=GPIO_PuPd_UP;                  //为了减少数据传输的损失特设上拉！！                 
	  GPIO_Init(GPIOB,&GPIO_InitStructe);
	
    USART_InitStructe.USART_BaudRate=bound;                       //设置波特率
    USART_InitStructe.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件流控制
    USART_InitStructe.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;     //收发模式
    USART_InitStructe.USART_Parity=USART_Parity_No;               //无奇偶校验位
    USART_InitStructe.USART_StopBits=USART_StopBits_1;            //1个停止位
    USART_InitStructe.USART_WordLength=USART_WordLength_8b;       //每次发送字节长度为8个字节
    USART_Init(USART3,&USART_InitStructe);

    USART_Cmd(USART3,ENABLE);                                    //使能串口---开启串口通信
		
   	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);        //开启相关中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;    //串口1中断通道
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	                    //根据指定的参数初始化VIC寄存器、
}
//发送一个字节数据
//input:byte,待发送的数据
void USART_send_byte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//等待发送完成
	USART3->DR=byte;	
}
//发送多字节数据
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		USART_send_byte(Buffer[i++]);
	}
}
//发送多字节数据+校验和
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		if(i<(Length-1))
		Buffer[Length-1]+=Buffer[i];//累加Length-1前的数据
		USART_send_byte(Buffer[i++]);
	}
}
//发送一帧数据
void send_out(int16_t *data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[20],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//清零缓存TX_DATA
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=send;//功能字节
	TX_DATA[i++]=2*length;//数据长度
	for(k=0;k<length;k++)//存入数据到缓存TX_DATA
	{
		TX_DATA[i++]=(uint16_t)data[k]>>8;
		TX_DATA[i++]=(uint16_t)data[k];
	}
	USART_Send(TX_DATA,sizeof(TX_DATA));	
}

uint8_t stata_reg=0;//接收状态寄存器
//5个数据缓存，对应Acc-Q，缓存长度为7，包含6个数据+1个校验和数据
//缓存长度为9，包含8个四元数数据+1个校验和数据
uint8_t data1_buf[7]={0},data2_buf[7]={0},data3_buf[7]={0},
data4_buf[7]={0},data5_buf[9]={0};
uint8_t CHeck(uint8_t *data)
{
  uint8_t i=0,flag=0,length=0,sum=0x5a+0x5a;
	if(stata_reg)
	{
	//	GPIO_ResetBits(GPIOB,GPIO_Pin_9);//LED亮
		if(stata_reg&ACC)//判断加数度数据接收完毕
		{
			stata_reg^=ACC;//清标志位
			flag=0x15;//记录功能字节
			length=6;//记录数据长度
			/*转移数据到data，避免串口中断对data1_buf的影响*/
		  memcpy(data,data1_buf,sizeof(data1_buf));
		  sum=sum+flag+length;//累加帧头、功能字节、数据长度
		}
		else if(stata_reg&GYR)
		{
			stata_reg^=GYR;
			flag=0x25;
			length=6;
		  memcpy(data,data2_buf,sizeof(data2_buf));
		  sum=sum+flag+length;
		}
		else if(stata_reg&MAG)
		{
			stata_reg^=MAG;
			flag=0x35;
			length=6;
		  memcpy(data,data3_buf,sizeof(data3_buf));
		  sum=sum+flag+length;
		}
		else if(stata_reg&RPY)
		{
			stata_reg^=RPY;
			flag=0x45;
			length=6;
		  memcpy(data,data4_buf,sizeof(data4_buf));
		  sum=sum+flag+length;
		}
		else if(stata_reg&Q4)
		{
			stata_reg^=Q4;
			flag=0x65;
			length=8;
		  memcpy(data,data5_buf,sizeof(data5_buf));
		  sum=sum+flag+length;
		}
		for(i=0;i<length;i++)//累加数据
		{
		 sum+=data[i];
		}
		if(sum!=data[i])//判断校验和是否正确
		return 0;
		else
			return flag;//返回功能字节	
	}
	else
		return 0;
}

void send_Instruction(void)
{
	uint8_t send_data[3]={0};
	send_data[0]=0xa5;
	send_data[1]=0x15;//加计功能字节
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);//指令校验和
	USART_Send_bytes(send_data,3);//发送加速度输出指令
	delay_ms(1);
	send_data[0]=0xa5;
	send_data[1]=0x25;
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);
	USART_Send_bytes(send_data,3);//发送陀螺仪数据输出指令
	delay_ms(1);
	send_data[0]=0xa5;
	send_data[1]=0x35;
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);
	USART_Send_bytes(send_data,3);//发送磁场数据输出指令
	delay_ms(1);
	send_data[0]=0xa5;
	send_data[1]=0x45;
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);
	USART_Send_bytes(send_data,3);//发送欧拉角数据输出指令
	delay_ms(1);
	send_data[0]=0xa5;
	send_data[1]=0x65;
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);
	USART_Send_bytes(send_data,3);//发送四元数数据输出指令
}



