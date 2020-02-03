#include "headfile.h"

//************************************************************


int main()
{
    __disable_irq();
    ILI9341_Init();          //LCD ≥ı ºªØ
    USART_Config();      // ≈‰÷√¥Æø⁄
    LED_GPIO_Config();

    delay_init();
    disp_init();
    i2c_init();
    delay_ms(500);

    FDC2214_init();
    delay_ms(500);

    offset0 = GetOffset(0);
    key_init();
    Time2_Init();
    Time4_Init();

    __enable_irq();

    while (1)
    {
        //vcan_sending(var_float, sizeof(var_float));
    }

}








/* ------------------------------------------end of file---------------------------------------- */

