#include "tim/interrupt.h"

void Time2_Init()

{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //打开相应的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    //清除TIM2的中断待处理位:TIM 中断源
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);


    //设置定时器TIM_TimeBaseInit初始化定时器的时钟基数
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 3600 - 1;      //T=(1+TIM_Prescaler )/72M)*(1+TIM_Period)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);


    //使能指定的TIM中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);


    //使能TIM定时器
    TIM_Cmd(TIM2, ENABLE);


    //设置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void Time4_Init()

{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //打开相应的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


    //清除TIM4的中断待处理位:TIM 中断源
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);


    //设置定时器TIM_TimeBaseInit初始化定时器的时钟基数
    TIM_TimeBaseStructure.TIM_Period = 200 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 3600 - 1;      //T=(1+TIM_Prescaler )/72M)*(1+TIM_Period)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure);


    //使能指定的TIM中断
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);


    //使能TIM定时器
    TIM_Cmd(TIM4, ENABLE);


    //设置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


