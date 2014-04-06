// #include "MSP430x14x.h"
#include "io430.h"
#include "i2c.h"
//P3.3  SCL
//P3.1  SDA
#define TestACK if(ReceiveAck()){return 1;}


unsigned int a[50];

static void Delay(unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n ; i++)
    {
//        _NOP();
    }
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

void NoAck(void)
{
    SDA_OUT;
    SDA_1;
    Delay(5);
    SCL_1;
    Delay(5);
    SCL_0;
    Delay(5);
}

void InitIIC(void)
{
    I2C_DIR |= I2C_SCL;  //set scl as output
    SDA_OUT; //set SDA as input
    SCL_0;
    Stop();
}

unsigned char ReceiveAck(void)
{
    unsigned char ack = 0;
    SCL_1;
    Delay(5);
    SDA_IN;
    Delay(5);
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
        if ((WriteData & 0x80) == 0x80)
        {
            SDA_1;
        }
        else
        {
            SDA_0;
        }

        Delay(5);
        SCL_1;
        Delay(5);
        SCL_0;
        Delay(5);
        WriteData = WriteData << 1;

    }

    SCL_0;
    SDA_1;
    Delay(5);
    Delay(5);
}

unsigned char Read8bit(void)
{

    unsigned char TempBit  = 0;
    unsigned char TempData = 0;
    SCL_0;
    Delay(5);
    SDA_IN;

    unsigned char i;
    for (i = 0; i < 8; i++)
    {

        SCL_1;
        Delay(5);
        SDA_IN;
        if (READ_SDA == I2C_SDA/*sda==1*/)
        {
            TempBit = 1;
        }
        else
        {
            TempBit = 0;
        }
        TempData = (TempData << 1) | TempBit;
        Delay(5);
        SCL_0;
        Delay(5);
    }

    Delay(5);
    return (TempData);
}

unsigned char ReadByte(unsigned char DEVICE_ID, unsigned char Address)
{
    unsigned char data = 0;

    unsigned char read_addr = (DEVICE_ID << 1) | BIT0;
    unsigned char wirte_addr = (DEVICE_ID << 1) & (~BIT0);

    Start();
    WriteByte(wirte_addr);

    TestACK

    WriteByte(Address);

    TestACK

    Start();

    WriteByte(read_addr);

    TestACK

    data  = Read8bit();

    NoAck();
    Stop();
    // Delay(1000);
    return (data);
}

unsigned char ReadBytes(unsigned char DEVICE_ID, unsigned char Address,  unsigned char *Buffer, unsigned int length)
{

//    unsigned char HighData = 0;
//    unsigned char LowData  = 0;
//    unsigned int  TempData = 0;

    unsigned char read_addr = (DEVICE_ID << 1) | BIT0;
    unsigned char wirte_addr = (DEVICE_ID << 1) & (~BIT0);


    Start();
    WriteByte(wirte_addr);
    TestACK
    WriteByte(Address);
    TestACK
    Start();
    WriteByte(read_addr);
    TestACK

    unsigned int i;
    for (i = 0; i < length; i++)
    {
        Buffer[i] = Read8bit();
        Acknowledge();
    }
    NoAck();
    Stop();
    Delay(1000);

    return 0;
}

unsigned int ReadWord(unsigned char DEVICE_ID, unsigned char Address)
{
    unsigned char HighData = 0;
    unsigned char LowData  = 0;
    unsigned int  TempData = 0;

    unsigned char read_addr = (DEVICE_ID << 1) | BIT0;
    unsigned char wirte_addr = (DEVICE_ID << 1) & (~BIT0);

    Start();
    WriteByte(wirte_addr);
    TestACK
    WriteByte(Address);
    TestACK
    Start();
    WriteByte(read_addr);
    TestACK
    LowData  = Read8bit();
    Acknowledge();
    HighData = Read8bit();
    NoAck();
    Stop();
    TempData = (HighData << 8) + LowData;
    return (TempData);

}
bool writeBit(unsigned char devAddr, unsigned char regAddr, unsigned char bitNum, unsigned char data)
{
    unsigned char status;
    status = readByte(devAddr, regAddr);
    status = (data != 0) ? (status | (1 << bitNum)) : (b & ~(1 << bitNum));
    return writeByte(devAddr, regAddr, status);
}

bool writeBits(unsigned char devAddr, unsigned char regAddr, unsigned char bitStart, unsigned char length, unsigned char data)
{
    unsigned char wirte_addr = (DEVICE_ID << 1) & (~BIT1);

    status = ReadByte(devAddr, regAddr);
    if (status == 1)
    {
        return false;
    }

/*         010 value to write
    76543210 bit numbers
       xxx   args: bitStart=4, length=3
    00011100 mask byte
    10101111 original value (sample)
    10100011 original & ~mask
    10101011 masked | value*/

    unsigned char mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    status &= ~(mask); // zero all important bits in existing byte
    status |= data; // combine data with existing byte
    WriteByte(devAddr, regAddr, status);
    return 0;
}

unsigned char WriteByte(unsigned char DEVICE_ID, unsigned char Address, unsigned char WriteData)
{
    unsigned char wirte_addr = (DEVICE_ID << 1) & (~BIT1);

    Start();
    WriteByte(wirte_addr);
    TestACK
    WriteByte(Address);
    TestACK
    WriteByte(WriteData);
    TestACK
    Stop();
    Delay(2000);
}

unsigned char WriteWord(unsigned char DEVICE_ID, unsigned char Address, unsigned int WriteData)
{
    unsigned char LowData  = 0;
    unsigned char HighData = 0;

    unsigned char wirte_addr = (DEVICE_ID << 1) & (~BIT1);
    LowData  = (unsigned char)WriteData;
    HighData = (unsigned char)(WriteData >> 8);
    Start();
    WriteByte(wirte_addr);
    TestACK
    WriteByte(Address);
    TestACK
    WriteByte(LowData);
    TestACK
    WriteByte(HighData);
    TestACK
    Stop();
    Delay(2000);
}
