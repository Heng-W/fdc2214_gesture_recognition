#ifndef _IIC_H
#define _IIC_H

#include "stm32f10x.h"


#define    IIC_CLK     RCC_APB2Periph_GPIOB
#define    IIC_PORT    GPIOB
#define    SCL_PIN     GPIO_Pin_6
#define    SDA_PIN     GPIO_Pin_7

#define SCL1()    GPIO_SetBits(IIC_PORT, SCL_PIN)
#define SCL0()    GPIO_ResetBits(IIC_PORT, SCL_PIN)
#define SDA1()    GPIO_SetBits(IIC_PORT, SDA_PIN)
#define SDA0()    GPIO_ResetBits(IIC_PORT, SDA_PIN)
#define SDA_READ()   GPIO_ReadInputDataBit(IIC_PORT, SDA_PIN)
#define SDA_IN()    {GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |= (u32)8<<28;}//GPIOB->ODR |= 1<<7;}
#define SDA_OUT()   {GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |= (u32)3<<28;}



void  i2c_write_reg(u8 dev_add, u8 reg, u8 data);
void  i2c_write_regs(u8 dev_add, u8 reg, u8* data, u8 num);
u8 i2c_read_reg(u8 dev_add, u8 reg);
void i2c_read_regs(u8 dev_add, u8 reg, u8* data, u8 num);
u8 i2c_check_device(u8 _Address);
void  i2c_init(void);

#endif
