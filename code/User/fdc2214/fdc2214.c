#include "fdc2214/fdc2214.h"


u32 Data_FDC = 0;

void Set_FDC2214(u8 reg, u8 MSB, u8 LSB)
{
    u8 data[2];
    data[0] = MSB;
    data[1] = LSB;
    i2c_write_regs(FDC2214_ADDR, reg, data, 2);
}

u16 FDC_Read(u8 reg)
{
    u16 res;
    u8 data[2];
    i2c_read_regs(FDC2214_ADDR, reg, data, 2);
    res = (u16)data[0] << 8;
    res |= data[1];
    return res;
}

//��ͨ������
u32 FCD2214_ReadCH(u8 index)
{
    u32 result;
    switch (index)
    {
        case 0:
            result = FDC_Read(DATA_CH0) & 0x0FFF;
            result = (result << 16) | FDC_Read(DATA_LSB_CH0);
            break;
        case 1:
            result = FDC_Read(DATA_CH1) & 0x0FFF;
            result = (result << 16) | FDC_Read(DATA_LSB_CH1);
            break;
        case 2:
            result = FDC_Read(DATA_CH2) & 0x0FFF;
            result = (result << 16) | FDC_Read(DATA_LSB_CH2);
            break;
        case 3:
            result = FDC_Read(DATA_CH3) & 0x0FFF;
            result = (result << 16) | FDC_Read(DATA_LSB_CH3);
            break;
        default:
            break;
    }
    result &= 0x0FFFFFFF;
    return result;
}

int FDC2214_init(void)
{
    char dispBuff[50];
    u16 res;

    res = FDC_Read(MANUFACTURER_ID);
    LCD_SetFont(&Font16x24);
    LCD_SetTextColor(BLACK);
    sprintf(dispBuff, "ID:%0x", res);
    LCD_ClearLine(LINE(4));
    ILI9341_DispStringLine_EN(LINE(4), dispBuff);
    if (res == 0x5449)
    {

        Set_FDC2214(RCOUNT_CH0, 0x34, 0xFB); //�ο�����ת�����ʱ�䣨T=(RCOUNT_CH0*16)/Frefx��
        Set_FDC2214(RCOUNT_CH1, 0x34, 0xFB);
        Set_FDC2214(RCOUNT_CH2, 0x34, 0xFB);
        Set_FDC2214(RCOUNT_CH3, 0x34, 0xFB);

        Set_FDC2214(SETTLECOUNT_CH0, 0x00, 0x1B); //ת��֮ǰ���ȶ�ʱ�䣨T=(SETTLECOUNT_CHx*16)/Frefx��
        Set_FDC2214(SETTLECOUNT_CH1, 0x00, 0x1B);
        Set_FDC2214(SETTLECOUNT_CH2, 0x00, 0x1B);
        Set_FDC2214(SETTLECOUNT_CH3, 0x00, 0x1B);

        Set_FDC2214(CLOCK_DIVIDERS_CH0, 0x20, 0x02); //ѡ����0.01MHz ~ 10MHz�Ĵ�����Ƶ��
        Set_FDC2214(CLOCK_DIVIDERS_CH1, 0x20, 0x02); //Frefx = Fclk = 43.4MHz/2(2��Ƶ)
        Set_FDC2214(CLOCK_DIVIDERS_CH2, 0x20, 0x02); //CHx_REF_DIVIDER=2;CHx_FIN_SEL=2
        Set_FDC2214(CLOCK_DIVIDERS_CH3, 0x20, 0x02);

        Set_FDC2214(DRIVE_CURRENT_CH0, 0x78, 0x00); //0.146ma��������ʱ�ӽ���+ת��ʱ�������������
        Set_FDC2214(DRIVE_CURRENT_CH1, 0x78, 0x00);
        Set_FDC2214(DRIVE_CURRENT_CH2, 0x78, 0x00);
        Set_FDC2214(DRIVE_CURRENT_CH3, 0x78, 0x00);

        Set_FDC2214(STATUS_CONFIG, 0x00, 0x00); //ȫ����ֹ����㱨
        Set_FDC2214(MUX_CONFIG, 0xC2, 0x0D); //ͨ��0��1��2 ��3��ѡ��10MhzΪ�����񵴲���Ƶ�ʵ�������ã���ͨ������ͨ��
        Set_FDC2214(CONFIG, 0x14, 0x01); //����ģʽ��ʹ���ڲ��������ο�Ƶ�ʣ�INTB���Ż���״̬�Ĵ������±���λ
        return 0;
    }
    else return 1;
}

//������·�ܵ���
float Cap_Calculate(u8 index)
{
    float Cap = 0;
    Data_FDC = FCD2214_ReadCH(index);
    if (Data_FDC)
        Cap = 232021045.248 / Data_FDC;
    return (Cap * Cap);
}


