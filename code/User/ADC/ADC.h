#ifndef __ADC_H
#define __ADC_H

#include "headfile.h"

#define  ADC1_DR_Address    ((u32)0x40012400+0x4c)
#define  CHANNEL_COUNT      4

extern __IO uint16_t ADC_ConvertedValue[CHANNEL_COUNT];

void ADC1_Init(void);
void stopADC(void);

#endif /*end of __FONT_H    */
