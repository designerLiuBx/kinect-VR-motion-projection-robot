#include "sys.h"
#include "usart4.h"
char UART4_flag1=0;
char UART4_flag2=0;
char UART4_Buffer[2]={0};
char UART4_Send[14]={0};
extern char APP_wave_form;
extern char APP_heart_rate;
extern char APP_Steps;
extern float APP_temperature;
extern float APP_distance;
//#define USART4_REC_LEN 50
u8 USART4_RX_BUF[USART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART4_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart4_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);    
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART4, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(UART4, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	/*
    DMA_DeInit(DMA1_Stream4);
    while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE){}   

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;         
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;     
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)UART4_Send; 
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;        
    DMA_InitStructure.DMA_BufferSize = 14;                
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;      
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;       
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;            
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
    DMA_Init(DMA1_Stream4, &DMA_InitStructure);

    DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Stream4_IRQn;           
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;          
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		DMA_Cmd(DMA1_Stream4, DISABLE); 
		*/
}


void UART4_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res=USART_ReceiveData(UART4);//(USART1->DR);	//读取接收到的数据
		if(UART4_flag1==1)
		{
		UART4_Buffer[UART4_flag2++]=Res;
		}
			else if(Res==0xA5)
			{
			UART4_flag1=1;
			}
		if(Res==0x5A)
		{
			UART4_flag1=0;
			UART4_flag2=0;
		}
  } 

} 
void USART4_send_byte(uint8_t byte)
{
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);
	UART4->DR=byte;	
}


void lanyatongxin(u8 i,u8 byte,float F)
{
	union result
     {
         float d;
         unsigned char data[4];
     }r1;
		 
	switch(i)
{
	
		 
case 1:
	USART4_send_byte(0xa5);//帧头
	USART4_send_byte(byte);//输出byte
  USART4_send_byte(byte);//校验位
  USART4_send_byte(0x5A);//尾部	
	break;
case 2:	
	   r1.d=F;
		USART4_send_byte(0xa5);//帧头

    USART4_send_byte(r1.data[0]);
		USART4_send_byte(r1.data[1]);
    USART4_send_byte(r1.data[2]);
	  USART4_send_byte(r1.data[3]);

    USART4_send_byte(r1.data[0]+r1.data[1]+r1.data[2]+r1.data[3]);//校验位
    USART4_send_byte(0x5A);//尾部	
		break;
}


}

void APP_packet()
{
	union 
     {
         float d;
         unsigned char data[4];
     }r1;
  union 
     {
         float d;
         unsigned char data[4];
     }r2;
		r1.d=APP_temperature;
		r2.d=APP_distance;
		 /*
		UART4_Send[0]=0xa5;//帧头
		UART4_Send[1]=APP_wave_form;
		UART4_Send[2]=APP_heart_rate;
		UART4_Send[3]=APP_Steps;
		UART4_Send[4]=r1.data[0];
		UART4_Send[5]=r1.data[1];
		UART4_Send[6]=r1.data[2];
		UART4_Send[7]=r1.data[3];
		UART4_Send[8]=r1.data[0];
		UART4_Send[9]=r1.data[1];
		UART4_Send[10]=r1.data[2];
		UART4_Send[11]=r1.data[3];
	  UART4_Send[12]=APP_wave_form+APP_heart_rate+APP_Steps+r1.data[0]+r1.data[1]+r1.data[2]+r1.data[3]+r2.data[0]+r2.data[1]+r2.data[2]+r2.data[3];
		UART4_Send[13]=0x5A;
		DMA_Cmd(DMA1_Stream4, ENABLE);                                       
    USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE); 
		 */
	  USART4_send_byte(0xa5);
		USART4_send_byte(APP_wave_form);
		USART4_send_byte(APP_heart_rate);
		USART4_send_byte(APP_Steps);	
    USART4_send_byte(r1.data[0]);
		USART4_send_byte(r1.data[1]);
    USART4_send_byte(r1.data[2]);
	  USART4_send_byte(r1.data[3]);
		 
		USART4_send_byte(r2.data[0]);
		USART4_send_byte(r2.data[1]);
    USART4_send_byte(r2.data[2]);
	  USART4_send_byte(r2.data[3]);
		 
		USART4_send_byte(APP_wave_form+APP_heart_rate+APP_Steps+r1.data[0]+r1.data[1]+r1.data[2]+r1.data[3]+r2.data[0]+r2.data[1]+r2.data[2]+r2.data[3]);//校验位
    USART4_send_byte(0x5A);//尾部	
		
}



