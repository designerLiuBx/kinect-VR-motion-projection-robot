#include "sys.h"
#include "usart2.h"	

u8 receive_str2[UART2_REC_NUM];     //接收缓存数组,最大USART_REC_LEN个字节 char UART2Buffer[200];
u8 uart2_byte_count=0;  //int BufferPoint = 0;


void usart2_init(u32 bound)
{
    GPIO_InitTypeDef  GPIO_InitStructe;
    USART_InitTypeDef  USART_InitStructe;
  	NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);     //使能串口外设
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);		 	//使能输出端口

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);   //连接端口与外设
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);   //连接端口与外设

 
	 
    GPIO_InitStructe.GPIO_Mode=GPIO_Mode_AF;                  //模式设置为复用
    GPIO_InitStructe.GPIO_OType=GPIO_OType_PP;                //强制推挽输出提高数据准确性
    GPIO_InitStructe.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;                     //输出引脚为5和6
    GPIO_InitStructe.GPIO_PuPd=GPIO_PuPd_UP;                  //为了减少数据传输的损失特设上拉！！                 
	  GPIO_Init(GPIOD,&GPIO_InitStructe);
	
    USART_InitStructe.USART_BaudRate=bound;                       //设置波特率
    USART_InitStructe.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件流控制
    USART_InitStructe.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;     //收发模式
    USART_InitStructe.USART_Parity=USART_Parity_No;               //无奇偶校验位
    USART_InitStructe.USART_StopBits=USART_StopBits_1;            //1个停止位
    USART_InitStructe.USART_WordLength=USART_WordLength_8b;       //每次发送字节长度为8个字节
    USART_Init(USART2,&USART_InitStructe);

    USART_Cmd(USART2,ENABLE);                                    //使能串口---开启串口通信
		
   	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);        //开启相关中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;    //串口1中断通道
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	                    //根据指定的参数初始化VIC寄存器、
}
char XBuffer[30];
char HBuffer[30];

void USART2_IRQHandler(void)
{
	u8 rec_data;
	int i;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断 
	{
		rec_data =(u8)USART_ReceiveData(USART2);         //(USART1->DR) 读取接收到的数据
		if(rec_data=='E')		  	                         //如果是S，表示是命令信息的起始位
		{
			uart2_byte_count=0x01; 
		}
		else if(rec_data=='D')		                         //如果E，表示是命令信息传送的结束位
		{
				if(receive_str2[0]=='X') 
				{
					memset(XBuffer,0,30);
					for(i=1;i<strlen(receive_str2);i++)
					{
						XBuffer[i-1] = receive_str2[i];
					}
					memset(receive_str2,0,200);	
					heart.X = atoi(XBuffer);
					for(uart2_byte_count=0;uart2_byte_count<32;uart2_byte_count++)
						receive_str2[uart2_byte_count]=0x00;
					uart2_byte_count=0;
				}
				
				else if(receive_str2[0]=='H')
				{
					memset(HBuffer,0,30);
					for(i=1;i<strlen(receive_str2);i++)
					{
						HBuffer[i-1] = receive_str2[i];
					}
					memset(receive_str2,0,200);
					heart.H=atoi(HBuffer);
					for(uart2_byte_count=0;uart2_byte_count<32;uart2_byte_count++)
						receive_str2[uart2_byte_count]=0x00;
					uart2_byte_count=0;
				}
		}
		else if((uart2_byte_count>0)&&(uart2_byte_count<=UART2_REC_NUM))
		{
			 receive_str2[uart2_byte_count-1]=rec_data;
			 uart2_byte_count++;
		}                		 
	}
	USART_ClearFlag(USART2,USART_FLAG_RXNE);//清中断标志
}

