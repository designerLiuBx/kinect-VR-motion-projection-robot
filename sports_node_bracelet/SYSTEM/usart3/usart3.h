#ifndef __USART3_H
#define __USART3_H
#include "Gen_include.h"

#include "sys.h"
#define ACC 0X01//加计控制位
#define GYR 0X02//陀螺控制位
#define MAG 0X04//磁场控制位
#define RPY 0X08//欧拉角控制位
#define Q4  0X10//四元数控制位
void usart3_init(u32 bound);
void USART_send_byte(uint8_t byte);
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length);
void USART_Send(uint8_t *Buffer, uint8_t Length);	
void send_out(int16_t *data,uint8_t length,uint8_t send);
void send_Instruction(void);
uint8_t CHeck(uint8_t *data);
#endif
