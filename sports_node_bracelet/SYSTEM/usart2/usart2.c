#include "sys.h"
#include "usart2.h"	

u8 receive_str2[UART2_REC_NUM];     //���ջ�������,���USART_REC_LEN���ֽ� char UART2Buffer[200];
u8 uart2_byte_count=0;  //int BufferPoint = 0;


void usart2_init(u32 bound)
{
    GPIO_InitTypeDef  GPIO_InitStructe;
    USART_InitTypeDef  USART_InitStructe;
  	NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);     //ʹ�ܴ�������
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);		 	//ʹ������˿�

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);   //���Ӷ˿�������
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);   //���Ӷ˿�������

 
	 
    GPIO_InitStructe.GPIO_Mode=GPIO_Mode_AF;                  //ģʽ����Ϊ����
    GPIO_InitStructe.GPIO_OType=GPIO_OType_PP;                //ǿ����������������׼ȷ��
    GPIO_InitStructe.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;                     //�������Ϊ5��6
    GPIO_InitStructe.GPIO_PuPd=GPIO_PuPd_UP;                  //Ϊ�˼������ݴ������ʧ������������                 
	  GPIO_Init(GPIOD,&GPIO_InitStructe);
	
    USART_InitStructe.USART_BaudRate=bound;                       //���ò�����
    USART_InitStructe.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ��������
    USART_InitStructe.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;     //�շ�ģʽ
    USART_InitStructe.USART_Parity=USART_Parity_No;               //����żУ��λ
    USART_InitStructe.USART_StopBits=USART_StopBits_1;            //1��ֹͣλ
    USART_InitStructe.USART_WordLength=USART_WordLength_8b;       //ÿ�η����ֽڳ���Ϊ8���ֽ�
    USART_Init(USART2,&USART_InitStructe);

    USART_Cmd(USART2,ENABLE);                                    //ʹ�ܴ���---��������ͨ��
		
   	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);        //��������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;    //����1�ж�ͨ��
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	  NVIC_Init(&NVIC_InitStructure);	                    //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}
char XBuffer[30];
char HBuffer[30];

void USART2_IRQHandler(void)
{
	u8 rec_data;
	int i;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж� 
	{
		rec_data =(u8)USART_ReceiveData(USART2);         //(USART1->DR) ��ȡ���յ�������
		if(rec_data=='E')		  	                         //�����S����ʾ��������Ϣ����ʼλ
		{
			uart2_byte_count=0x01; 
		}
		else if(rec_data=='D')		                         //���E����ʾ��������Ϣ���͵Ľ���λ
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
	USART_ClearFlag(USART2,USART_FLAG_RXNE);//���жϱ�־
}

