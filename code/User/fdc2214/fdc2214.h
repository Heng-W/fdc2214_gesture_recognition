#ifndef _FDC2214_H
#define _FDC2214_H

#include "headfile.h"

#define IIC_WR  0       /* 写控制bit */
#define IIC_RD  1       /* 读控制bit */


#define FDC2214_ADDR     0x2A  //IIC写的数据地址,ADDR = 0时，I2C地址= 0x2A，当ADDR = 1时，I2C地址= 0x2B

#define DATA_CH0         0x00
#define DATA_LSB_CH0     0x01
#define DATA_CH1         0x02
#define DATA_LSB_CH1     0x03
#define DATA_CH2         0x04
#define DATA_LSB_CH2     0x05
#define DATA_CH3         0x06
#define DATA_LSB_CH3     0x07

#define RCOUNT_CH0       0x08
#define RCOUNT_CH1       0x09
#define RCOUNT_CH2       0x0A
#define RCOUNT_CH3       0x0B

#define OFFSET_CH0       0x0C
#define OFFSET_CH1       0x0D
#define OFFSET_CH2       0x0E
#define OFFSET_CH3       0x0F

#define SETTLECOUNT_CH0  0x10
#define SETTLECOUNT_CH1  0x11
#define SETTLECOUNT_CH2  0x12
#define SETTLECOUNT_CH3  0x13

#define CLOCK_DIVIDERS_CH0  0x14
#define CLOCK_DIVIDERS_CH1  0x15
#define CLOCK_DIVIDERS_CH2  0x16
#define CLOCK_DIVIDERS_CH3  0x17

#define DRIVE_CURRENT_CH0   0x1E
#define DRIVE_CURRENT_CH1   0x1F
#define DRIVE_CURRENT_CH2   0x20
#define DRIVE_CURRENT_CH3   0x21

#define STATUS_CONFIG 0x19
#define MUX_CONFIG    0x1B
#define CONFIG        0x1A
#define MANUFACTURER_ID 0x7E

extern uint32_t Data_FDC;

int FDC2214_init(void);
uint32_t FCD2214_ReadCH(uint8_t index);
float Cap_Calculate(uint8_t index);

#endif
