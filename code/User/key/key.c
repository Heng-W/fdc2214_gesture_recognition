#include "key/key.h"


static uint8_t key_cnt, short_flag, long_flag, TrainCnt;
uint8_t key_process;
STATE_e state;
KEY_STATE_e key_state = KEY_UP;

int out = 500;
float train1_data[3], train2_data[5];
float data1[9] = {0}, data2[15] = {0};
char* str1[] = {"Judge Model", "Judge Model", "Train Model", "Judge Model", "Train Model", "Judge Model"};
char* str2[] = {"Cai Quan", "Hua Quan", "Cai Quan2", "Cai Quan2", "Hua Quan2", "Hua Quan2"};


void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
    // 设置按键的引脚为浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
}


KEY_STATE_e key_get(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_DOWN)
    {
        return KEY_DOWN;
    }
    return KEY_UP;
}


uint8_t key_read(void)
{
    uint8_t key_val = 0;
    if (key_get(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_DOWN) key_val += 0x01;
    if (key_get(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_DOWN) key_val += 0x02;
    return key_val;
}

void key_scan(void)
{
    uint8_t key_value;
    key_value = key_read();

    switch (key_state)
    {
        case KEY_UP:
            if (key_value) key_state = KEY_DOWN;
            break;
        case KEY_DOWN:
            if (key_value)
            {
                key_state = KEY_HOLD;
                key_process = key_value;
            }
            else  key_state = KEY_UP;
            break;
        case KEY_HOLD:
            key_cnt++;
            if (!key_value)
            {
                key_cnt = 0;
                short_flag = 1;
                key_state = KEY_UP;
            }
            if (key_cnt > 100)
            {
                key_state = KEY_LONG;
                long_flag = 1;
                key_process = key_value;
            }
            break;
        case KEY_LONG:
            key_cnt++;
            if (!key_value)
            {
                key_cnt = 0;
                key_state = KEY_UP;
                LCD_ClearLine(LINE(11));
                LCD_ClearLine(LINE(12));
            }
            break;
        default:
            break;
    }
}



void key1_short_dispose(void)
{
    char dispBuff[50];
    LCD_ClearLine(LINE(11));
    LCD_ClearLine(LINE(12));

    if (state == Train1)
    {
        if (TrainCnt < 9)
        {
            LCD_SetFont(&Font16x24);
            LCD_SetTextColor(BLUE2);
            sprintf(dispBuff, "Training not finished");
            ILI9341_DispStringLine_EN(LINE(11), dispBuff);
        }
        TrainCnt = 0;
    }
    else if (state == Train2)
    {
        if (TrainCnt < 15)
        {
            LCD_SetFont(&Font16x24);
            LCD_SetTextColor(BLUE2);
            sprintf(dispBuff, "Training not finished");
            ILI9341_DispStringLine_EN(LINE(11), dispBuff);
        }
        TrainCnt = 0;
    }
    state++;
    if (state >= 6)
        state = Judge1;

    LCD_SetFont(&Font16x24);
    LCD_SetTextColor(RED);
    sprintf(dispBuff, "%s", str1[state]);
    LCD_ClearLine(LINE(1));
    ILI9341_DispStringLine_EN(LINE(1), dispBuff);

    LCD_SetTextColor(BLUE);
    sprintf(dispBuff, "%s", str2[state]);
    LCD_ClearLine(LINE(2));
    ILI9341_DispStringLine_EN(LINE(2), dispBuff);
}

void key2_short_dispose(void)
{
    char dispBuff[50];
    if (state == Train1 && TrainCnt < 9)
    {
        data1[TrainCnt] = val;
        TrainCnt++;

        LCD_SetFont(&Font16x24);
        LCD_SetTextColor(BLUE2);
        sprintf(dispBuff, "Data%d get", TrainCnt);
        LCD_ClearLine(LINE(11));
        ILI9341_DispStringLine_EN(LINE(11), dispBuff);

        if (TrainCnt == 9)
        {
            train1_data[0] = get_ave(data1, 0, 2);
            train1_data[1] = get_ave(data1, 3, 5);
            train1_data[2] = get_ave(data1, 6, 8);

            LCD_SetFont(&Font16x24);
            LCD_SetTextColor(BLUE2);
            sprintf(dispBuff, "Training finished");
            LCD_ClearLine(LINE(11));
            LCD_ClearLine(LINE(12));
            ILI9341_DispStringLine_EN(LINE(11), dispBuff);
        }
    }
    if (state == Train2 && TrainCnt < 15)
    {
        data2[TrainCnt] = val;
        TrainCnt++;
        LCD_SetFont(&Font16x24);
        LCD_SetTextColor(BLUE2);
        sprintf(dispBuff, "Data%d get", TrainCnt);
        LCD_ClearLine(LINE(11));
        ILI9341_DispStringLine_EN(LINE(11), dispBuff);

        if (TrainCnt == 15)
        {
            train2_data[0] = get_ave(data2, 0, 2);
            train2_data[1] = get_ave(data2, 3, 5);
            train2_data[2] = get_ave(data2, 6, 8);
            train2_data[3] = get_ave(data2, 9, 11);
            train2_data[4] = get_ave(data2, 12, 14);

            LCD_SetFont(&Font16x24);
            LCD_SetTextColor(BLUE2);
            sprintf(dispBuff, "Training finished");
            LCD_ClearLine(LINE(11));
            LCD_ClearLine(LINE(12));
            ILI9341_DispStringLine_EN(LINE(11), dispBuff);
        }
    }
}

void key1_long_dispose(void)
{
    char dispBuff[50];
    offset0 = GetOffset(0);

    LCD_SetFont(&Font16x24);
    LCD_SetTextColor(BLUE2);
    sprintf(dispBuff, "OFFSET reset");
    LCD_ClearLine(LINE(11));
    ILI9341_DispStringLine_EN(LINE(11), dispBuff);
}


void key2_long_dispose(void)
{
    char dispBuff[50];
    TrainCnt = 0;

    if (state == ReJudge1)
        state = Train1 ;
    if (state == ReJudge2)
        state = Train2 ;

    LCD_SetFont(&Font16x24);
    LCD_SetTextColor(RED);
    sprintf(dispBuff, "%s", str1[state]);
    LCD_ClearLine(LINE(1));
    ILI9341_DispStringLine_EN(LINE(1), dispBuff);

    LCD_SetTextColor(BLUE);
    sprintf(dispBuff, "%s", str2[state]);
    LCD_ClearLine(LINE(2));
    ILI9341_DispStringLine_EN(LINE(2), dispBuff);

    LCD_SetTextColor(BLUE2);
    sprintf(dispBuff, "Retraining");
    LCD_ClearLine(LINE(11));
    LCD_ClearLine(LINE(12));
    ILI9341_DispStringLine_EN(LINE(11), dispBuff);

}


void key_dispose(void)
{
    if (short_flag)
    {
        short_flag = 0;
        switch (key_process)
        {
            case key1_val:
                key1_short_dispose();
                break;
            case key2_val:
                key2_short_dispose();
                break;
            default:
                break;
        }
    }

    if (long_flag)
    {
        long_flag = 0;
        switch (key_process)
        {
            case key1_val:
                key1_long_dispose();
                break;
            case key2_val:
                key2_long_dispose();
                break;
            default:
                break;
        }
    }

    if (key_state == KEY_AUTO && !(key_cnt % 10))
    {
        switch (key_process)
        {
            case key1_val:
                key1_long_dispose();
                break;
            case key2_val:
                key2_long_dispose();
                break;
            default:
                break;
        }
    }
}

void disp_init(void)
{
    char dispBuff[50];
    u8 i;
    LCD_SetFont(&Font16x24);
    for (i = 1; i <= 12; i++)
        LCD_ClearLine(LINE(i));

    LCD_SetTextColor(RED);
    sprintf(dispBuff, "%s", str1[0]);
    LCD_ClearLine(LINE(1));
    ILI9341_DispStringLine_EN(LINE(1), dispBuff);

    LCD_SetTextColor(BLUE);
    sprintf(dispBuff, "%s", str2[0]);
    LCD_ClearLine(LINE(2));
    ILI9341_DispStringLine_EN(LINE(2), dispBuff);
}
