#include "io430.h"

#ifndef I2C_H
#define I2C_H


#define I2C_DIR P3DIR
#define I2C_OUT P3OUT
#define I2C_IN  P3IN
#define I2C_SCL BIT3
#define I2C_SDA BIT1

#define SDA_1   I2C_OUT |=  I2C_SDA              //SDA = 1
#define SDA_0   I2C_OUT &=~ I2C_SDA              //SDA = 0
#define SCL_1   I2C_OUT |=  I2C_SCL              //SCL = 1
#define SCL_0   I2C_OUT &=~ I2C_SCL              //SCL = 0


#define SDA_IN   I2C_DIR &=~ I2C_SDA           //set SDA INPUT
#define SDA_OUT  I2C_DIR |=  I2C_SDA            //set SDA OUTPUT
#define READ_SDA (I2C_IN & I2C_SDA)        //Read SDA


void InitIIC(void);

unsigned int ReadWord(unsigned char DEVICE_ID, unsigned char Address);

unsigned char ReadByte(unsigned char DEVICE_ID, unsigned char Address);

unsigned char ReadBytes(unsigned char DEVICE_ID, unsigned char Address,  unsigned char *Buffer, unsigned int length);

unsigned char WriteWord(unsigned char DEVICE_ID, unsigned char Address, unsigned int WriteData);
#endif
