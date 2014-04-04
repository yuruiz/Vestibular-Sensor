// #include "MSP430x14x.h"
#include "io430.h"
#include "i2c.h"
//P3.3  SCL
//P3.1  SDA



unsigned int a[50];

static void Delay(unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n ; i++)
    {
//        _NOP();
    }
}

void InitIIC(void)
{
    I2C_DIR |= I2C_SCL;  //set scl as output
    SDA_OUT; //set SDA as input
    SCL_1;
    Delay(5);
    SDA_1;
    Delay(5);
}

void Start(void)
{
    SDA_OUT;

    SDA_1;
    Delay(5);
    SCL_1;
    Delay(5);
    SDA_0;
    Delay(5);
    SCL_0;
    Delay(5);
}

void Stop(void)
{
    SDA_OUT;

    SCL_0;
    Delay(5);
    SDA_0;
    Delay(5);
    SCL_1;
    Delay(5);
    SDA_1;
    Delay(5);
}

unsigned char ReceiveAck(void)
{
    unsigned char ack = 0;
    SCL_1;
    Delay(5);
    SDA_IN;
    Delay(5);
    // while ((READ_SDA == 0x01 /*sda==1*/) && (i < 255)/*调试方便,可以不要*/)
    // {
    //     i++;
    // }

    ack = READ_SDA;
    SDA_OUT;
    SCL_0;
    Delay(5);

    return ack;
}

void Acknowledge(void)
{
    SCL_0;
    Delay(5);
    SDA_OUT;
    SDA_0;
    SCL_1;
    Delay(5);
    SCL_0;
}

void WriteByte(unsigned char WriteData)
{
    unsigned char i;

    SDA_OUT;

    for (i = 0; i < 8; i++)
    {
        SCL_0;
        Delay(5);

        if (((WriteData >> 7) & 0x01) == 0x01)
        {
            SDA_1;
        }
        else
        {
            SDA_0;
        }

        Delay(5);
        SCL_1;
        WriteData = WriteData << 1;
        Delay(5);
    }

    SCL_0;
    SDA_1;
    Delay(5);
    Delay(5);
}

unsigned char ReadByte(void)
{
    unsigned char i;
    unsigned char TempBit  = 0;
    unsigned char TempData = 0;
    SCL_0;
    Delay(5);
    SDA_1;

    for (i = 0; i < 8; i++)
    {
        Delay(5);
        SCL_1;
        Delay(5);
        SDA_IN;

        if (READ_SDA == 0x01 /*sda==1*/)
        {
            TempBit = 1;
        }
        else
        {
            TempBit = 0;
        }

        SDA_OUT;
        TempData = (TempData << 1) | TempBit;
        SCL_0;
    }

    Delay(5);
    return (TempData);
}


unsigned int ReadWord(unsigned char unit/*address*/)
{
    unsigned char HighData = 0;
    unsigned char LowData  = 0;
    unsigned int  TempData = 0;
    Start();
    WriteByte(0xa0);
    ReceiveAck();
    WriteByte(unit);
    ReceiveAck();
    Start();
    WriteByte(0xa1);
    ReceiveAck();
    LowData  = ReadByte();
    Acknowledge();
    HighData = ReadByte();
    Stop();
    TempData = (HighData << 8) + LowData;
    Delay(1000);
    return (TempData);
}

void ReadWords(unsigned char unit/*address*/)
{
    unsigned char i;
    unsigned char HighData = 0;
    unsigned char LowData  = 0;
//    unsigned int  TempData = 0;
    Start();
    WriteByte(0xa0);
    ReceiveAck();
    WriteByte(unit);
    ReceiveAck();
    Start();
    WriteByte(0xa1);
    ReceiveAck();

    for (i = 0; i < 49; i++)
    {
        LowData  = ReadByte();
        Acknowledge();
        HighData = ReadByte();
        Acknowledge();
        a[i] = (HighData << 8) + LowData;
    }

    LowData  = ReadByte();
    Acknowledge();
    HighData = ReadByte();
    Stop();
    a[49] = (HighData << 8) + LowData;
    Delay(1000);
}

void WriteWord(unsigned char unit/*address*/, unsigned int WriteData)
{
    unsigned char LowData  = 0;
    unsigned char HighData = 0;
    LowData  = (unsigned char)WriteData;
    HighData = (unsigned char)(WriteData >> 8);
    Start();
    WriteByte(0xa0);
    ReceiveAck();
    WriteByte(unit);
    ReceiveAck();
    WriteByte(LowData);
    ReceiveAck();
    WriteByte(HighData);
    ReceiveAck();
    Stop();
    Delay(2000);
}
