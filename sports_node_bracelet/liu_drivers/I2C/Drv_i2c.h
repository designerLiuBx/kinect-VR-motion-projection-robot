#ifndef __I2C_H
#define __I2C_H	
#include "Gen_include.h"

void IIC_Init(void);
uint8_t I2C_ReadByte(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t Reg_addr);
void I2C_WriteByte(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t Reg_addr,uint8_t value);

#endif 
