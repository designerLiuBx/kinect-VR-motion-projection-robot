#include "Drv_oled.h"
#include "Drv_font.h"

void OLED_Write_Data(unsigned char IIC_Command)
{
	I2C_WriteByte(I2C1,0x78,0x40,IIC_Command);
}

void OLED_Write_Command(unsigned char IIC_Command)
{
	I2C_WriteByte(I2C1,0x78,0x00,IIC_Command);
}

void OLED_Display_On(void)
{
	I2C_WriteByte(I2C1,0x78,0x00,0X8D);
	I2C_WriteByte(I2C1,0x78,0x00,0X14);
	I2C_WriteByte(I2C1,0x78,0x00,0XAF);
}


//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	I2C_WriteByte(I2C1,0x78,0x00,0X8D);
	I2C_WriteByte(I2C1,0x78,0x00,0X10);
	I2C_WriteByte(I2C1,0x78,0x00,0XAE);
}	

//���ù������
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	I2C_WriteByte(I2C1,0x78,0x00,0xb0+y);
	I2C_WriteByte(I2C1,0x78,0x00,((x&0xf0)>>4)|0x10);
	I2C_WriteByte(I2C1,0x78,0x00,x&0x0f);
}   	  
//����
void OLED_Clear(void)  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		I2C_WriteByte(I2C1,0x78,0x00,0xb0+i); //����ҳ��ַ��0~7��
		I2C_WriteByte(I2C1,0x78,0x00,0x00);//������ʾλ�á��е͵�ַ
		I2C_WriteByte(I2C1,0x78,0x00,0x10);//������ʾλ�á��иߵ�ַ   
		for(n=0;n<132;n++)
			I2C_WriteByte(I2C1,0x78,0x40,0);
	} //������ʾ
}

void OLED_Init()
{
	IIC_Init();
	I2C_WriteByte(I2C1,0x78,0x00,0xAE);  //display off
	I2C_WriteByte(I2C1,0x78,0x00,0x20);	//Set Memory Addressing Mode	
	I2C_WriteByte(I2C1,0x78,0x00,0x02);	//0x00,Horizontal Addressing Mode;0x01,Vertical Addressing Mode;0x02,Page Addressing Mode (RESET);11,Invalid
	I2C_WriteByte(I2C1,0x78,0x00,0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	I2C_WriteByte(I2C1,0x78,0x00,0xc8);	//Set COM Output Scan Direction
	I2C_WriteByte(I2C1,0x78,0x00,0x00);	//---set low column address
	I2C_WriteByte(I2C1,0x78,0x00,0x10); //---set high column address
	I2C_WriteByte(I2C1,0x78,0x00,0x40); //--set start line address
	I2C_WriteByte(I2C1,0x78,0x00,0x81); //--set contrast control register
	
	//I2C_WriteByte(I2C1,0x78,0x00,0x10);
	
	I2C_WriteByte(I2C1,0x78,0x00,0xff);
	I2C_WriteByte(I2C1,0x78,0x00,0xa1);	//--set segment re-map 0 to 127
	I2C_WriteByte(I2C1,0x78,0x00,0xa6);	//--set normal display
	I2C_WriteByte(I2C1,0x78,0x00,0xa8);	//--set multiplex ratio(1 to 64)
	I2C_WriteByte(I2C1,0x78,0x00,0x3f);	//
	I2C_WriteByte(I2C1,0x78,0x00,0xa4);	//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	I2C_WriteByte(I2C1,0x78,0x00,0xd3);	//-set display offset
	I2C_WriteByte(I2C1,0x78,0x00,0x00);	//-not offset
	I2C_WriteByte(I2C1,0x78,0x00,0xd5);	//--set display clock divide ratio/oscillator frequency
	I2C_WriteByte(I2C1,0x78,0x00,0xf0);	//--set divide ratio
	I2C_WriteByte(I2C1,0x78,0x00,0xd9);	//--set pre-charge period
	I2C_WriteByte(I2C1,0x78,0x00,0x22);	//
	I2C_WriteByte(I2C1,0x78,0x00,0xda);	//--set com pins hardware configuration
	I2C_WriteByte(I2C1,0x78,0x00,0x12);
	I2C_WriteByte(I2C1,0x78,0x00,0xdb);	//--set vcomh
	I2C_WriteByte(I2C1,0x78,0x00,0x20);	//0x20,0.77xVcc
	I2C_WriteByte(I2C1,0x78,0x00,0x8d);	//--set DC-DC enable
	I2C_WriteByte(I2C1,0x78,0x00,0x14);	//
	I2C_WriteByte(I2C1,0x78,0x00,0xaf);	//--turn on oled panel //����ʾ
	//I2C_WriteByte(I2C1,0x78,0x00,0xae);//--turn on oled panel //�ر���ʾ�����ߣ�
	//I2C_WriteByte(I2C1,0x78,0x00,0xa5);//ȫ����ʾ
	delay_ms(5);
	OLED_Clear();
	//OLED_DrawBMP(0,0,128,8,hljdx);
	delay_ms(1000);
	OLED_Clear();
	OLED_DrawBMP(32,0,96,8,xiaohui);
	delay_ms(1000);
	OLED_Clear();
	OLED_DrawBMP(40,2,56,6,liu[1]);
	OLED_DrawBMP(56,2,72,6,liu[2]);
	OLED_DrawBMP(72,2,88,6,liu[3]);
	//OLED_DrawBMP(16,4,48,8,shiyanshi[0]);
	//OLED_DrawBMP(48,4,80,8,shiyanshi[1]);
	//OLED_DrawBMP(80,4,112,8,shiyanshi[2]);
	//OLED_Clear();
	//OLED_Clear();
	//OLED_DrawBMP(48,0,80,8,xiaohui);
	delay_ms(1000);
	OLED_Clear();
	
}

void OLED_DrawBlack(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1)
{ 	
 unsigned char x,y;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
				I2C_WriteByte(I2C1,0x78,0x40,0);
	    }
	}
} 

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
					OLED_Write_Data(BMP[j++]);
	    }
	}
} 

void OLED_Roll(unsigned char state,unsigned char direction,unsigned char speed)
{
	if(state == 1)
	{
		I2C_WriteByte(I2C1,0x78,0x00,0x2E);		//�رչ���
		if(direction == 1)
		{
		I2C_WriteByte(I2C1,0x78,0x00,0x27);        //ˮƽ��������ҹ��� 26/27
		}else 
			if(direction == 0)
				I2C_WriteByte(I2C1,0x78,0x00,0x26);
			else 
				return;
		I2C_WriteByte(I2C1,0x78,0x00,0x00);        //�����ֽ�
		I2C_WriteByte(I2C1,0x78,0x00,0x00);        //��ʼҳ 0
		I2C_WriteByte(I2C1,0x78,0x00,speed);        //����ʱ����
		I2C_WriteByte(I2C1,0x78,0x00,0x07);        //��ֹҳ 7
		I2C_WriteByte(I2C1,0x78,0x00,0x00);        //�����ֽ�
		I2C_WriteByte(I2C1,0x78,0x00,0xFF);        //�����ֽ�
		I2C_WriteByte(I2C1,0x78,0x00,0x2F);        //��������
	}else 
		if(state == 0)
		I2C_WriteByte(I2C1,0x78,0x00,0x2E);
}

void OLED_Refresh(char hour,char minute,char second)
{
	OLED_DrawBMP(32,0,48,8,snumber[10]);
	OLED_DrawBMP(80,0,96,8,snumber[10]);
	OLED_DrawBMP(0,0,16,8,snumber[hour/10]);
	OLED_DrawBMP(16,0,32,8,snumber[hour%10]);
	OLED_DrawBMP(48,0,64,8,snumber[minute/10]);
	OLED_DrawBMP(64,0,80,8,snumber[minute%10]);
	OLED_DrawBMP(96,0,112,8,snumber[second/10]);
	OLED_DrawBMP(112,0,128,8,snumber[second%10]);
}
void OLED_Temperature(float	temperature)
{

		if(temperature <= 0.0000001f)
		{
			OLED_DrawBMP(0,2,16,8,snumber[11]);
			temperature	= fabs(temperature);
		}
		else
		OLED_DrawBMP(0,2,16,8,snumber[13]);
		OLED_DrawBMP(16,2,32,8,snumber[(int)temperature/10]);
		OLED_DrawBMP(32,2,48,8,snumber[(int)temperature%10]);
		OLED_DrawBMP(48,2,64,8,snumber[12]);
		OLED_DrawBMP(64,2,80,8,snumber[((int)((temperature-(int)temperature)*10))]);
		OLED_DrawBMP(80,2,96,8,snumber[((int)((temperature-(int)temperature)*100)%10)]);
		OLED_DrawBMP(96,2,128,8,t);
}

//====================================================
u8 OLED_GRAM[128][8];
void OLED_Clear2(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();
}
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Command (0xb0+i);
		OLED_Write_Command (0x00); 
		OLED_Write_Command (0x10);
		for(n=0;n<128;n++)
			OLED_Write_Data(OLED_GRAM[n][i]);
	}   
}


//OLED������ʾ����
void OLED_Waveform_display(void)
{
	int i;	
	uint8_t n;
	int j,k=0;
	uint8_t waveform2[128] = {0};				//���βɼ����飬�ɼ�128���㣬OLED�Ŀ��Ϊ128������
	if(waveform_flag == 1)
	{
		waveform_flag = 0;
		for(i=127;i>=0;i--)
		{
			for(n=0;n<64;n++)
			{
				OLED_DrawPoint(i,n,0);
			}
			//�����߲��������������ã�����ע�͵�,��#if 1��Ϊ#if 0
			//���ò��β��㺯�������ǲ��ο�������������
			//#if 1
			for (j=127; j>0; j--)
			{
				waveform2[j] = waveform[j];
			}
			/*
			for (j=127; j>63; j--)
			{
				waveform2[127-(127-j)] = waveform[ j ];
				waveform2[127-(127-j)-1] = waveform[j];
			}
			*/
			for(j=0;j<63;j++)
			{
				waveform2[k]=waveform[j+64];
				waveform2[k+1]=(waveform[j+64] + waveform[j+64+1] )/2;
				k+=2;
			}
			k=0;
			if(i!=0)
			{
				if(myabs((int)waveform2[i]-(int)waveform2[i-1])>1)
				{
					if(waveform2[i] > waveform2[i-1])
					{
						for(n=waveform2[i-1];n<waveform2[i];n++)
						{
							OLED_DrawPoint(i,n,1);		//����Ӧ�����ص��ϴ�ӡ
						}
					}else
					{
						for(n=waveform2[i];n<waveform2[i-1];n++)
						{
							OLED_DrawPoint(i,n,1);		//����Ӧ�����ص��ϴ�ӡ
						}
					}
					
				}			
			}
			OLED_DrawPoint(i,waveform2[i],1);		//����Ӧ�����ص��ϴ�ӡ
			//#endif*/
		}
		OLED_Refresh_Gram();						//������ʾ��OLED
	}
}
//���δ����������ڶ�ʱ����ִ�У�2msִ��һ��
void Waveform_deal(float x)
{
	//uint8_t temp;
	float xiao;
	static u8 waveSample_times = 0;
	
	int i;
	
	waveSample_times ++;
	if(waveSample_times == 10)
	{
		waveSample_times = 0;
		waveform_flag = 1;		
	
//		temp = (u8)x/1;		//adcȡ��������
//		xiao = x;			
//		xiao -= temp;			//adcȡС������
//		xiao *= 100;			//adc��С������ת�����������Ŵ�һ�ٱ�
		xiao = x;
		//���濪ʼ�������ݴ����Ա�ʹ��Һ����ʾ
		if(xiao<-10)				//��xiao��ֵ������50-80֮��
		{
			xiao = -10;
		}else if(xiao>10)
		{
			xiao = 10;
		}
		xiao += 10;
		xiao *= 3;		
		xiao = 64-xiao;			//ʹ���εĿ����0-60֮�䣬OLED�Ŀ��Ϊ64
		
		//��������ʾ�����Ĵ����������һ�����������ʾ����
		waveform_copy[127] = waveform[127];
		for(i=126;i>=0;i--)
		{
			waveform_copy[i] = waveform[i];			
			waveform[i] = waveform_copy[i+1];
		}
		waveform[127] = xiao;
	}		
}
int myabs(int a)
{
	if(a<0)
		return -a;
	else 
		return a;
}
