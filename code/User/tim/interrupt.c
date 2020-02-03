#include "tim/interrupt.h"

void Time2_Init()

{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //����Ӧ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    //���TIM2���жϴ�����λ:TIM �ж�Դ
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);


    //���ö�ʱ��TIM_TimeBaseInit��ʼ����ʱ����ʱ�ӻ���
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 3600 - 1;      //T=(1+TIM_Prescaler )/72M)*(1+TIM_Period)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);


    //ʹ��ָ����TIM�ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);


    //ʹ��TIM��ʱ��
    TIM_Cmd(TIM2, ENABLE);


    //����NVIC
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

    //����Ӧ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


    //���TIM4���жϴ�����λ:TIM �ж�Դ
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);


    //���ö�ʱ��TIM_TimeBaseInit��ʼ����ʱ����ʱ�ӻ���
    TIM_TimeBaseStructure.TIM_Period = 200 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 3600 - 1;      //T=(1+TIM_Prescaler )/72M)*(1+TIM_Period)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure);


    //ʹ��ָ����TIM�ж�
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);


    //ʹ��TIM��ʱ��
    TIM_Cmd(TIM4, ENABLE);


    //����NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


