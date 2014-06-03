#include "uart.h"
#include "io430.h"

void UartInit()
{
    USART_SEL |= UTXD0 + URXD0;     //set the pinout as secondary functin
    UCTL0 = CHAR;    //8 data bits , 1 stop bit , No parity bit
    UTCTL0 |= SSEL0;     //setlect UCLK = ACLK
    UBR00 = 0x3;        //set the baud rate 9600 bit/s, clock == 32k
    UBR10 = 0;
    UMCTL0 = 0x4A;
//    UTCTL0 |= SSEL1;
//    UBR00=0x41;           //set the baud rate 9600 bit/s, clock == 8M
//    UBR10=0x03; 
//    UMCTL0=0x09;
    UCTL0 &= ~SWRST;
    ME1 |= UTXE0 + URXE0;  //open UART0
    IE1 |= URXIE0 ;        //enable UART0 interupt
}

void SendUart(unsigned char *pBuffer, unsigned char n_byte)
{
    unsigned char q0;

    for (q0 = 0; q0 < n_byte; q0++)
    {
        while ((IFG1 & UTXIFG0) == 0); //check if the sending finished

        TXBUF0 = *pBuffer;

        pBuffer++;
    }
}
