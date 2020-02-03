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
    GPIO_Init(GPIOC, &GPIO_InitStructure);              // PCx,����ʱ������������
}

static void ADC1_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // ��DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // ��ADCʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // ��λDMA������
    DMA_DeInit(DMA1_Channel1);

    // ���� DMA ��ʼ���ṹ��
    // �����ַΪ��ADC ���ݼĴ�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(& (ADC1->DR));

    // �洢����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;

    // ����Դ��������
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

    // ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
    DMA_InitStructure.DMA_BufferSize = CHANNEL_COUNT;

    // ����Ĵ���ֻ��һ������ַ���õ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    // �洢����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    // �������ݴ�СΪ���֣��������ֽ�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

    // �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

    // ѭ������ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

    // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;

    // ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    // ��ʼ��DMA
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    // ʹ�� DMA ͨ��
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // ADC ģʽ����
    // ֻʹ��һ��ADC�����ڵ�ģʽ
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;

    // ɨ��ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE ;

    // ����ת��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

    // �����ⲿ����ת���������������
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

    // ת������Ҷ���
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    // ת��ͨ������
    ADC_InitStructure.ADC_NbrOfChannel = CHANNEL_COUNT;

    // ��ʼ��ADC
    ADC_Init(ADC1, &ADC_InitStructure);

    // ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // ����ADC ͨ����ת��˳��Ͳ���ʱ��

    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 4, ADC_SampleTime_55Cycles5);

    // ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
    // ʹ��ADC DMA ����
    ADC_DMACmd(ADC1, ENABLE);

    // ����ADC ������ʼת��
    ADC_Cmd(ADC1, ENABLE);

    // ��ʼ��ADC У׼�Ĵ���
    ADC_ResetCalibration(ADC1);
    // �ȴ�У׼�Ĵ�����ʼ�����
    while (ADC_GetResetCalibrationStatus(ADC1));

    // ADC��ʼУ׼
    ADC_StartCalibration(ADC1);
    // �ȴ�У׼���
    while (ADC_GetCalibrationStatus(ADC1));

    // ����û�в����ⲿ����������ʹ���������ADCת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Mode_Config();
}
/*
*����ADC
*/
void stopADC(void)
{
    /* ����û�в����ⲿ����������ʹ���������ADCת�� */
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

