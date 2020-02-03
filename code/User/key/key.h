#ifndef __KEY_H
#define __KEY_H

#include "headfile.h"


#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA
#define    KEY1_GPIO_PIN     GPIO_Pin_0

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY2_GPIO_PORT    GPIOC
#define    KEY2_GPIO_PIN     GPIO_Pin_13

#define    key1_val          0x01
#define    key2_val          0x02


typedef enum
{
    KEY_UP = 0,
    KEY_DOWN = 1,
    KEY_HOLD,
    KEY_LONG,
    KEY_AUTO
} KEY_STATE_e;


typedef enum
{
    Judge1 = 0,
    Judge2 = 1,
    Train1 = 2,
    ReJudge1 = 3,
    Train2 = 4,
    ReJudge2 = 5
} STATE_e;


extern STATE_e state;
extern float train1_data[3], train2_data[5];

void key_init(void);
void key_scan(void);
void key_dispose(void);
void disp_init(void);


#endif
