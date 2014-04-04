#ifndef I2C_H
#define I2C_H


#define I2C_DIR P3DIR
#define I2C_OUT P3OUT
#define I2C_IN  P3IN
#define I2C_SCL BIT3
#define I2C_SDA BIT1

#define SDA_1   P3OUT |=  I2C_SDA              //SDA = 1
#define SDA_0   P3OUT &=~ I2C_SDA              //SDA = 0
#define SCL_1   P3OUT |=  I2C_SCL              //SCL = 1
#define SCL_0   P3OUT &=~ I2C_SCL              //SCL = 0


#define SDA_IN   P3DIR &=~ I2C_SDA           //set SDA INPUT
#define SDA_OUT  P3DIR |=  I2C_SDA            //set SDA OUTPUT
#define READ_SDA ((P3IN >> 3) & 0x01)        //Read SDA


void InitIIC();

void Start();

void Stop();

void Ack();

void NoAck();

unsigned char TestAck();

void Write8Bit(unsigned char input);

unsigned char Read8Bit();

void delay(unsigned int n);

#endif
