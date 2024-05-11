#include "ds1307.h"

DS1307::DS1307(const byte SCL_pin, const byte SDA_pin)
{
    SCL = SCL_pin;
    SDA = SDA_pin;
}

void DS1307::I2C_start(void)
{
    pinMode(SCL, OUTPUT);
    pinMode(SDA, OUTPUT);

    digitalWrite(SCL, 1);
    digitalWrite(SDA, 1);
    delayMicroseconds(20);

    digitalWrite(SDA, 0);
    delayMicroseconds(20);

    digitalWrite(SCL, 0);
    delayMicroseconds(20);
}

void DS1307::I2C_stop(void)
{
    pinMode(SCL, OUTPUT);
    pinMode(SDA, OUTPUT);

    digitalWrite(SCL, 1);
    digitalWrite(SCL, 0);
    digitalWrite(SDA, 0);

    delayMicroseconds(20);
    digitalWrite(SCL, 1);
    delayMicroseconds(20);
    digitalWrite(SDA, 1);
}

unsigned char DS1307::I2C_w(unsigned char dat)
{
    pinMode(SCL, OUTPUT);
    pinMode(SDA, OUTPUT);

    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        digitalWrite(SDA, ((dat << i) & 0x80) ? 1 : 0);
        digitalWrite(SCL, 1);
        delayMicroseconds(20);
        digitalWrite(SCL, 0);
        delayMicroseconds(20);
    }
    digitalWrite(SCL, 1);
    delayMicroseconds(20);
    digitalWrite(SCL, 0);
    return dat;
}

unsigned char DS1307::I2C_r(void)
{
    pinMode(SCL, OUTPUT);
    pinMode(SDA, INPUT);

    char rd_bit;
    unsigned char i, dat;
    dat = 0x00;
    for (i = 0; i < 8; i++) /* For loop read data 1 byte */
    {
        delayMicroseconds(20);
        digitalWrite(SCL, 1);
        delayMicroseconds(20);     /* Set SCL */
        rd_bit = digitalRead(SDA); /* Keep for check acknowledge */
        dat = dat << 1;
        dat = dat | rd_bit; /* Keep bit data in dat */
        digitalWrite(SCL, 0);
    }
    return dat;
}
//************************I2C-END***************************//

//********************DS1307****************************//
unsigned char DS1307::DS_r(unsigned char addr)
{
    unsigned int temp, ret;
    I2C_start();   /* Start i2c bus */
    I2C_w(0xD0);   /* Connect to DS1307 */
    I2C_w(addr);   /* Request RAM address on DS1307 */
    I2C_start();   /* Start i2c bus */
    I2C_w(0XD1);   /* Connect to DS1307 for Read */
    ret = I2C_r(); /* Receive data */
    I2C_stop();

    temp = ret;                                /*BCD to HEX*/
    ret = (((ret / 16) * 10) + (temp & 0x0f)); /*for Led 7seg*/

    return ret;
}

void DS1307::DS_W(unsigned char addr, unsigned char dat)
{
    unsigned int temp;

    temp = dat;                              /*HEX to BCD*/
    dat = (((dat / 10) * 16) | (temp % 10)); /*for Led 7seg*/

    I2C_start(); /* Start i2c bus */
    I2C_w(0XD0); /* Connect to DS1307 */
    I2C_w(addr); /* Request RAM address on DS1307 */
    I2C_w(dat);  /* Connect to DS1307 for Read */
    I2C_stop();
}
//**********************DS1307-END*************************//
