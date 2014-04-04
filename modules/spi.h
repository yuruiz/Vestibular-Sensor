#ifndef SPI_H
#define SPI_H
#include "io430.h"
#include "in430.h"

#define IO_SPI_DIR    P2DIR |=BIT0+BIT1+BIT3+BIT4  //p2的0,1,3，4管脚为输出,其余为输入
#define RF_SDN_1      P2OUT |=BIT0   //p2_0口为输出模式，做为RF_SDN的控制
#define RF_SDN_0      P2OUT &=~BIT0
#define RF4432_SDI_1  P2OUT |=BIT1   //p2_1口为SPI数据输入
#define RF4432_SDI_0  P2OUT &=~BIT1
#define RF4432_SEL_1  P2OUT |=BIT4  //p2_4口为SPI片选
#define RF4432_SEL_0  P2OUT &=~BIT4
#define RF4432_SCLK_1 P2OUT |=BIT3  //p2_3口为SPI时钟输出
#define RF4432_SCLK_0 P2OUT &=~BIT3
#define RF4432_SDO    (P2IN &BIT2) //p2_2口为SPI数据输出
#define RF4432_IRQ    (P2IN &BIT5)  //p2_5口为SI4432工作状态指示端口

#endif