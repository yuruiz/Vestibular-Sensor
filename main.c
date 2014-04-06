#include "io430.h"
#include "in430.h"
#include "stdio.h"
#include "i2c.h"
#include "MPU6500.h"
#include "uart.h"

#pragma vector=USART0RX_VECTOR
__interrupt void UartRx()
{
    //    RxFlag=1;
    __low_power_mode_off_on_exit();
}


#pragma vector=USART0TX_VECTOR
__interrupt void UartTx ()
{
    //    TxFlag=1;
    __low_power_mode_off_on_exit();
}

static void Delay(unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n ; i++)
    {
        //        _NOP();
    }
}

//System Initialization
void InitSys()
{
        //use XT2 Oscillator
        // BCSCTL1 &= ~XT2OFF; //open XT2 Oscillator

        // do
        // {
        //     IFG1 &= ~OFIFG; //clear Oscillator flags

        //     unsigned int iq0;
        //     for (iq0 = 0xFF; iq0 > 0; iq0--); //delay, wait Oscillator to work
        // }
        // while ((IFG1 & OFIFG) != 0); //test if XT2 works

        // BCSCTL2 = SELM_2 + SELS; //select XT2 for MCLK¡¢SMCLK

    //Other peripheral initialization
    InitIIC();
    MPU6500_Init();

    // UartInit();

    //  _EINT(); //Open global interrupt control
}

void main(void)
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    InitSys();

    // P1DIR |= 0x03;                        // Set P1.0 to output direction

    if(!MPU_Test_Connection())
        printf("Connection Error");

    for (;;)
    {
        // SendUart("123456", 6);

        unsigned char buffer[14];

        readBytes(MPU6500_DEFAULT_ADDRESS, MPU6500_RA_ACCEL_XOUT_H, 14, buffer);

        unsigned int ax = (((int16_t)buffer[0]) << 8) | buffer[1];
        unsigned int ay = (((int16_t)buffer[2]) << 8) | buffer[3];
        unsigned int az = (((int16_t)buffer[4]) << 8) | buffer[5];
        unsigned int gx = (((int16_t)buffer[8]) << 8) | buffer[9];
        unsigned int gy = (((int16_t)buffer[10]) << 8) | buffer[11];
        unsigned int gz = (((int16_t)buffer[12]) << 8) | buffer[13];

        printf("%d\n", ax);
        printf("%d\n", ay);
        printf("%d\n", az);
        printf("%d\n", gx);
        printf("%d\n", gy);
        printf("%d\n", gz);

        // P1OUT ^= 0x03;                      // Toggle P1.0 using exclusive-OR
        // Delay(1000);
    }
}
