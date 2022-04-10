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
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);     //ʹ�ܴ�������
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);		 	//ʹ������˿�

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);   //���Ӷ˿�������
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);   //���Ӷ˿�������

 
	 
    GPIO_InitStructe.GPIO_Mode=GPIO_Mode_AF;                  //ģʽ����Ϊ����
    GPIO_InitStructe.GPIO_OType=GPIO_OType_PP;                //ǿ����������������׼ȷ��
    GPIO_InitStructe.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_10;                     //�������Ϊ5��6
    GPIO_InitStructe.GPIO_PuPd=GPIO_PuPd_UP;                  //Ϊ�˼������ݴ������ʧ������������                 
	  GPIO_Init(GPIOB,&GPIO_InitStructe);
	
    USART_InitStructe.USART_BaudRate=bound;                       //���ò�����
    USART_InitStructe.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ��������
    USART_InitStructe.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;     //�շ�ģʽ
    USART_InitStructe.USART_Parity=USART_Parity_No;               //����żУ��λ
    USART_InitStructe.USART_StopBits=USART_StopBits_1;            //1��ֹͣλ
    USART_InitStructe.USART_WordLength=USART_WordLength_8b;       //ÿ�η����ֽڳ���Ϊ8���ֽ�
    USART_Init(USART3,&USART_InitStructe);

    USART_Cmd(USART3,ENABLE);                                    //ʹ�ܴ���---��������ͨ��
		
   	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);        //��������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;    //����1�ж�ͨ��
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	  NVIC_Init(&NVIC_InitStructure);	                    //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}
//����һ���ֽ�����
//input:byte,�����͵�����
void USART_send_byte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//�ȴ��������
	USART3->DR=byte;	
}
//���Ͷ��ֽ�����
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		USART_send_byte(Buffer[i++]);
	}
}
//���Ͷ��ֽ�����+У���
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		if(i<(Length-1))
		Buffer[Length-1]+=Buffer[i];//�ۼ�Length-1ǰ������
		USART_send_byte(Buffer[i++]);
	}
}
//����һ֡����
void send_out(int16_t *data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[20],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//���㻺��TX_DATA
	TX_DATA[i++]=0X5A;//֡ͷ
	TX_DATA[i++]=0X5A;//֡ͷ
	TX_DATA[i++]=send;//�����ֽ�
	TX_DATA[i++]=2*length;//���ݳ���
	for(k=0;k<length;k++)//�������ݵ�����TX_DATA
	{
		TX_DATA[i++]=(uint16_t)data[k]>>8;
		TX_DATA[i++]=(uint16_t)data[k];
	}
	USART_Send(TX_DATA,sizeof(TX_DATA));	
}

uint8_t stata_reg=0;//����״̬�Ĵ���
//5�����ݻ��棬��ӦAcc-Q�����泤��Ϊ7������6������+1��У�������
//���泤��Ϊ9������8����Ԫ������+1��У�������
uint8_t data1_buf[7]={0},data2_buf[7]={0},data3_buf[7]={0},
data4_buf[7]={0},data5_buf[9]={0};
uint8_t CHeck(uint8_t *data)
{
  uint8_t i=0,flag=0,length=0,sum=0x5a+0x5a;
	if(stata_reg)
	{
	//	GPIO_ResetBits(GPIOB,GPIO_Pin_9);//LED��
		if(stata_reg&ACC)//�жϼ��������ݽ������
		{
			stata_reg^=ACC;//���־λ
			flag=0x15;//��¼�����ֽ�
			length=6;//��¼���ݳ���
			/*ת�����ݵ�data�����⴮���ж϶�data1_buf��Ӱ��*/
		  memcpy(data,data1_buf,sizeof(data1_buf));
		  sum=sum+flag+length;//�ۼ�֡ͷ�������ֽڡ����ݳ���
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
		for(i=0;i<length;i++)//�ۼ�����
		{
		 sum+=data[i];
		}
		if(sum!=data[i])//�ж�У����Ƿ���ȷ
		return 0;
		else
			return flag;//���ع����ֽ�	
	}
	else
		return 0;
}

void send_Instruction(void)
{
	uint8_t send_data[3]={0};
	send_data[0]=0xa5;
	send_data[1]=0x15;//�Ӽƹ����ֽ�
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);//ָ��У���
	USART_Send_bytes(send_data,3);//���ͼ��ٶ����ָ��
	delay_ms(1);
	send_data[0]=0xa5;
	send_data[1]=0x25;
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);
	USART_Send_bytes(send_data,3);//�����������������ָ��
	delay_ms(1);
	send_data[0]=0xa5;
	send_data[1]=0x35;
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);
	USART_Send_bytes(send_data,3);//���ʹų��������ָ��
	delay_ms(1);
	send_data[0]=0xa5;
	send_data[1]=0x45;
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);
	USART_Send_bytes(send_data,3);//����ŷ�����������ָ��
	delay_ms(1);
	send_data[0]=0xa5;
	send_data[1]=0x65;
	send_data[2]=(uint8_t)(send_data[0]+send_data[1]);
	USART_Send_bytes(send_data,3);//������Ԫ���������ָ��
}



