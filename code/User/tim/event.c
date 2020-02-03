#include "tim/event.h"


static u8 cnt, last_hand_flag; //hand_cnt;
u8 hand_flag = 0;
float val;
float offset0 = 0;
float value0, value0_pre = 0;
static u8 val_cnt[6];
char* str0[] = {"ShiTou", "JianDao", "Bu"};


void TIM2_deal(void)
{
    char dispBuff[50];
    value0 = Cap_Mid();
    value0 = fliter(value0_pre, value0);
    value0_pre = value0;
    val = value0;
    cnt ++;
    hand_judge();

    if (cnt >= 100)
    {
        cnt = 0;
        LCD_SetFont(&Font16x24);
        LCD_SetTextColor(GREEN);
        sprintf(dispBuff, "IN0:%.3f pF", value0);
        LCD_ClearLine(LINE(6));
        ILI9341_DispStringLine_EN(LINE(6), dispBuff);
    }
    if (cnt % 50 == 0)
        hand_dispose();
}

void hand_judge(void)
{
    u8 i, j, pos = 0;
    switch (state)
    {
        case Judge1 :
            if (val > 1 && val < 2.7) val_cnt[1]++;
            else if (val > 2.7 && val < 3.9) val_cnt[2]++;
            else if (val > 3.9 && val < 6) val_cnt[3]++;
            else val_cnt[0]++;
            break;
        case Judge2 :
            if (val > 1 && val < 2.6) val_cnt[1]++;
            else if (val > 2.6 && val < 3.3) val_cnt[2]++;
            else if (val > 3.3 && val < 3.7) val_cnt[3]++;
            else if (val > 3.7 && val < 4.3) val_cnt[4]++;
            else if (val > 4.3 && val < 6) val_cnt[5]++;
            else val_cnt[0]++;
            break;
        case ReJudge1 :
            if (val > 1 && val < get_ave(train1_data, 0, 1))  val_cnt[1]++;
            if (val > get_ave(train1_data, 0, 1) && val < get_ave(train1_data, 1, 2)) val_cnt[2]++;
            if (val > get_ave(train1_data, 1, 2) && val < 6) val_cnt[3]++;
            if (val < 1 || val > 6) val_cnt[0]++;
            break;
        case ReJudge2 :
            if (val > 1 && val < get_ave(train2_data, 0, 1)) val_cnt[1]++;
            if (val > get_ave(train2_data, 0, 1) && val < get_ave(train2_data, 1, 2)) val_cnt[2]++;
            if (val > get_ave(train2_data, 1, 2) && val < get_ave(train2_data, 2, 3)) val_cnt[3]++;
            if (val > get_ave(train2_data, 2, 3) && val < get_ave(train2_data, 3, 4)) val_cnt[4]++;
            if (val > get_ave(train2_data, 3, 4) && val < 6) val_cnt[5]++;
            if (val < 1 || val > 6) val_cnt[0]++;
            break;
        default:
            break;
    }
    if (cnt % 20 == 0)
    {
        for (i = 0; i < 5; i++)
            for (j = 1; j < 6 - i; j++)
                if (val_cnt[i + j] > val_cnt[i])
                    pos = i + j;

        hand_flag = pos;
        for (i = 0; i < 6; i++) val_cnt[i] = 0;
    }
}



void hand_dispose(void)
{
    char dispBuff[50];
    if (last_hand_flag != hand_flag)
    {
        if (!hand_flag) LCD_ClearLine(LINE(9)); //无手势
        else
        {
            if (state == Judge1 || state == ReJudge1)        //猜拳
            {
                LCD_SetTextColor(BLACK);
                sprintf(dispBuff, "This is %s", str0[hand_flag - 1]);
                LCD_ClearLine(LINE(9));
                ILI9341_DispStringLine_EN(LINE(9), dispBuff);
            }

            else if (state == Judge2 || state == ReJudge2)                       //数字划拳
            {
                LCD_SetTextColor(BLACK);
                sprintf(dispBuff, "This is %d", hand_flag);
                LCD_ClearLine(LINE(9));
                ILI9341_DispStringLine_EN(LINE(9), dispBuff);
            }
        }
    }
    last_hand_flag = hand_flag;
}



float GetOffset(u8 index)
{
    float sum = 0;
    int i;
    for (i = 0; i < 20; i++)
        sum += Cap_Calculate(index);
    return sum / 20;
}

float get_ave(float a[], u8 m, u8 n)
{
    u8 i;
    float sum = 0;
    for (i = m; i <= n; i++)
        sum += a[i];
    return sum / (n - m + 1);
}

float fliter(float pre, float cur)
{
    return 0.2 * pre + 0.8 * cur;
}

float Cap_Mid(void)
{
    float h, j, k, tmp;
    float tmpsum = 0;
    int n = 0;
    for (; n < 4; n++)
    {
        h = Cap_Calculate(0);
        j = Cap_Calculate(0);
        k = Cap_Calculate(0);
        if (h > j)
        {
            tmp = h;
            h = j;
            j = tmp;
        }
        if (k > j)
            tmp = j;
        else if (k > h)
            tmp = k;
        else
            tmp = h;
        tmpsum += tmp;
    }
    tmpsum = tmpsum / 4;
    return tmpsum - offset0;
}


