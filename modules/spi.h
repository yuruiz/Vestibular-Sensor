#ifndef SPI_H
#define SPI_H
#include "io430.h"
#include "in430.h"

#define IO_SPI_DIR    P2DIR |=BIT0+BIT1+BIT3+BIT4  //p2��0,1,3��4�ܽ�Ϊ���,����Ϊ����
#define RF_SDN_1      P2OUT |=BIT0   //p2_0��Ϊ���ģʽ����ΪRF_SDN�Ŀ���
#define RF_SDN_0      P2OUT &=~BIT0
#define RF4432_SDI_1  P2OUT |=BIT1   //p2_1��ΪSPI��������
#define RF4432_SDI_0  P2OUT &=~BIT1
#define RF4432_SEL_1  P2OUT |=BIT4  //p2_4��ΪSPIƬѡ
#define RF4432_SEL_0  P2OUT &=~BIT4
#define RF4432_SCLK_1 P2OUT |=BIT3  //p2_3��ΪSPIʱ�����
#define RF4432_SCLK_0 P2OUT &=~BIT3
#define RF4432_SDO    (P2IN &BIT2) //p2_2��ΪSPI�������
#define RF4432_IRQ    (P2IN &BIT5)  //p2_5��ΪSI4432����״ָ̬ʾ�˿�

#endif