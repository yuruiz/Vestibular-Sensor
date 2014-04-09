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

    UartInit();

    //  _EINT(); //Open global interrupt control
}

void main(void)
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    InitSys();

    // P1DIR |= 0x03;                        // Set P1.0 to output direction

    if (!MPU_Test_Connection())
    { printf("Connection Error"); }

    for (;;)
    {
        // unsigned char strings[8] = "123456\r\n";
        // SendUart(strings, 8);
        writeBit(MPU6500_DEFAULT_ADDRESS, MPU6500_RA_PWR_MGMT_1, MPU6500_PWR1_SLEEP_BIT, 0);

        unsigned char buffer[14];

        ReadBytes(MPU6500_DEFAULT_ADDRESS, MPU6500_RA_ACCEL_XOUT_H, buffer, 14);

        unsigned int ax = (((unsigned int)buffer[0]) << 8) | buffer[1];
        unsigned int ay = (((unsigned int)buffer[2]) << 8) | buffer[3];
        unsigned int az = (((unsigned int)buffer[4]) << 8) | buffer[5];
        //        unsigned int gx = (((unsigned int)buffer[8]) << 8) | buffer[9];
        //        unsigned int gy = (((unsigned int)buffer[10]) << 8) | buffer[11];
        //        unsigned int gz = (((unsigned int)buffer[12]) << 8) | buffer[13];


        unsigned char axs[10];
        unsigned char ays[10];
        unsigned char azs[10];
        sprintf(axs, "ax: %d\r\n", ax);
        sprintf(ays, "ay: %d\r\n", ay);
        sprintf(azs, "az: %d\r\n", az);
        SendUart(axs, 10);
        SendUart(ays, 10);
        SendUart(azs, 10);
        // printf("%d\n", gx);
        // printf("%d\n", gy);
        // printf("%d\n", gz);

        // P1OUT ^= 0x03;                      // Toggle P1.0 using exclusive-OR
        // Delay(1000);
    }
}
