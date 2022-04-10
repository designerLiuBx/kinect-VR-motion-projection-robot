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
u8 USART4_RX_BUF[USART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART4_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart4_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��USART1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);    
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART4, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
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


void UART4_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res=USART_ReceiveData(UART4);//(USART1->DR);	//��ȡ���յ�������
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
	USART4_send_byte(0xa5);//֡ͷ
	USART4_send_byte(byte);//���byte
  USART4_send_byte(byte);//У��λ
  USART4_send_byte(0x5A);//β��	
	break;
case 2:	
	   r1.d=F;
		USART4_send_byte(0xa5);//֡ͷ

    USART4_send_byte(r1.data[0]);
		USART4_send_byte(r1.data[1]);
    USART4_send_byte(r1.data[2]);
	  USART4_send_byte(r1.data[3]);

    USART4_send_byte(r1.data[0]+r1.data[1]+r1.data[2]+r1.data[3]);//У��λ
    USART4_send_byte(0x5A);//β��	
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
		UART4_Send[0]=0xa5;//֡ͷ
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
		 
		USART4_send_byte(APP_wave_form+APP_heart_rate+APP_Steps+r1.data[0]+r1.data[1]+r1.data[2]+r1.data[3]+r2.data[0]+r2.data[1]+r2.data[2]+r2.data[3]);//У��λ
    USART4_send_byte(0x5A);//β��	
		
}



