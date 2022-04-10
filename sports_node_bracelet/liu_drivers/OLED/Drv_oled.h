#ifndef __OLED_H
#define __OLED_H

#include "Gen_include.h"
//#include "Drv_font.h"
void OLED_Write_Data(unsigned char IIC_Command);
void OLED_Write_Command(unsigned char IIC_Command);
void OLED_Init(void);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void OLED_Clear(void);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_DrawBlack(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1);
void OLED_Roll(unsigned char state,unsigned char direction,unsigned char speed);
void OLED_Refresh(char hour,char minute,char second);
void OLED_Temperature(float	temperature);



void OLED_Clear2(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Refresh_Gram(void);
void OLED_Waveform_display(void);
void Waveform_deal(float x);
int myabs(int a);

#endif



