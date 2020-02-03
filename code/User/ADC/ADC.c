#include "ADC/ADC.h"

__IO uint16_t ADC_ConvertedValue[CHANNEL_COUNT] = {0};

void ADC1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure PC.01  as analog input */

    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_2 |
        GPIO_Pin_3 |
        GPIO_Pin_4 |
        GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);              // PCx,输入时不用设置速率
}

static void ADC1_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 打开DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 打开ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // 复位DMA控制器
    DMA_DeInit(DMA1_Channel1);

    // 配置 DMA 初始化结构体
    // 外设基址为：ADC 数据寄存器地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(& (ADC1->DR));

    // 存储器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;

    // 数据源来自外设
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

    // 缓冲区大小，应该等于数据目的地的大小
    DMA_InitStructure.DMA_BufferSize = CHANNEL_COUNT;

    // 外设寄存器只有一个，地址不用递增
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    // 存储器地址递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    // 外设数据大小为半字，即两个字节
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

    // 内存数据大小也为半字，跟外设数据大小相同
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

    // 循环传输模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

    // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;

    // 禁止存储器到存储器模式，因为是从外设到存储器
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    // 初始化DMA
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    // 使能 DMA 通道
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // ADC 模式配置
    // 只使用一个ADC，属于单模式
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;

    // 扫描模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE ;

    // 连续转换模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

    // 不用外部触发转换，软件开启即可
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

    // 转换结果右对齐
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    // 转换通道个数
    ADC_InitStructure.ADC_NbrOfChannel = CHANNEL_COUNT;

    // 初始化ADC
    ADC_Init(ADC1, &ADC_InitStructure);

    // 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // 配置ADC 通道的转换顺序和采样时间

    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 4, ADC_SampleTime_55Cycles5);

    // ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
    // 使能ADC DMA 请求
    ADC_DMACmd(ADC1, ENABLE);

    // 开启ADC ，并开始转换
    ADC_Cmd(ADC1, ENABLE);

    // 初始化ADC 校准寄存器
    ADC_ResetCalibration(ADC1);
    // 等待校准寄存器初始化完成
    while (ADC_GetResetCalibrationStatus(ADC1));

    // ADC开始校准
    ADC_StartCalibration(ADC1);
    // 等待校准完成
    while (ADC_GetCalibrationStatus(ADC1));

    // 由于没有采用外部触发，所以使用软件触发ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Mode_Config();
}
/*
*禁用ADC
*/
void stopADC(void)
{
    /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);

    /* Disable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, DISABLE);
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, DISABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, DISABLE);

    /* Disable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);

    /* Disable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
}

