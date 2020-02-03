#include "i2c/i2c.h"

void i2c_delay(void)
{
    int i = 10;
    while (--i);
}

void i2c_start(void)
{
    SDA1();
    SCL1();
    i2c_delay();
    SDA0();
    i2c_delay();
    SCL0();
    i2c_delay();
}

void i2c_stop(void)
{
    SDA0();
    SCL1();
    i2c_delay();
    SDA1();
}

void i2c_send_ack(u8 ack_x)
{
    if (ack_x) SDA1();
    else       SDA0();
    i2c_delay();
    SCL1();
    i2c_delay();
    SCL0();
    i2c_delay();
}

u8 i2c_wait_ack(void)
{
    u8 re;
    SDA_IN();
    SCL1();
    i2c_delay();
    if (SDA_READ()) re = 1;
    else    re = 0;
    SCL0();
    i2c_delay();
    SDA_OUT();
    return re;
}

u8 i2c_send_ch(u8 c)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        if (c & 0x80) SDA1();
        else          SDA0();
        i2c_delay();
        SCL1();
        i2c_delay();
        SCL0();
        c <<= 1;
    }
    return i2c_wait_ack();
}

u8 i2c_read_ch(u8 ack_x)
{
    u8 i, c = 0;
    SDA_IN();
    for (i = 0; i < 8; i++)
    {
        c <<= 1;
        SCL1();
        i2c_delay();
        if (SDA_READ()) c++;
        SCL0();
        i2c_delay();
    }
    SDA_OUT();
    i2c_send_ack(ack_x);
    return c;
}


void i2c_write_reg(u8 dev_add, u8 reg, u8 data)
{
    i2c_start();
    i2c_send_ch(dev_add | 0);
    i2c_send_ch(reg);
    i2c_send_ch(data);
    i2c_stop();
}

void i2c_write_regs(u8 dev_add, u8 reg, u8* data, u8 num)
{
    i2c_start();
    i2c_send_ch(dev_add | 0);
    i2c_send_ch(reg);
    while (--num)
    {
        i2c_send_ch(*data);
        data++;
    }
    i2c_send_ch(*data);
    i2c_stop();
}

u8 i2c_read_reg(u8 dev_add, u8 reg)
{
    u8 data;
    i2c_start();
    i2c_send_ch(dev_add | 0);
    i2c_send_ch(reg);
    i2c_stop();

    i2c_start();
    i2c_send_ch(dev_add | 1);
    data = i2c_read_ch(1);
    i2c_stop();
    return data;
}

void i2c_read_regs(u8 dev_add, u8 reg, u8* data, u8 num)
{
    i2c_start();
    i2c_send_ch(dev_add | 0);
    i2c_send_ch(reg);
    i2c_stop();

    i2c_start();
    i2c_send_ch(dev_add | 1);
    while (--num)
    {
        *data = i2c_read_ch(0);
        data++;
    }
    *data = i2c_read_ch(1);
    i2c_stop();
}


u8 i2c_check_device(u8 _Address)
{
    u8 ack_data;
    i2c_start();
    ack_data = i2c_send_ch(_Address | 0); /* 检测设备的ACK应答 */
    i2c_stop();
    return ack_data;
}


void i2c_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(IIC_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = SCL_PIN | SDA_PIN;

    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IIC_PORT, &GPIO_InitStructure);

    SDA1();
    SCL1();
}


