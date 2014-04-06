#include "io430.h"
#include "in430.h"
#include "stdio.h"
#include "i2c.h"
#include "MPU6500.h"

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
        unsigned int iq0;
    
        //use XT2 Oscillator
        BCSCTL1 &= ~XT2OFF; //open XT2 Oscillator
    
        do
        {
            IFG1 &= ~OFIFG; //clear Oscillator flags
    
            for (iq0 = 0xFF; iq0 > 0; iq0--); //delay, wait Oscillator to work
        }
        while ((IFG1 & OFIFG) != 0); //test if XT2 works

        BCSCTL2 = SELM_2 + SELS; //select XT2 for MCLK¡¢SMCLK

    //Other peripheral initialization
    InitIIC();
    //  _EINT(); //Open global interrupt control
}

void main(void)
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    //  InitSys();

    InitSys();

    P1DIR |= 0x03;                        // Set P1.0 to output direction

    //    IIC_Start();
    unsigned char ID = ReadByte(MPU6500_DEFAULT_ADDRESS, MPU6500_RA_WHO_AM_I);

//    ID = ID >> 1;
    //    printf("123");
    printf("%x\n", ID);

    for (;;)
    {
        volatile unsigned int i;            // volatile to prevent optimization

        P1OUT ^= 0x03;                      // Toggle P1.0 using exclusive-OR
        //    printf("hello world\n");
        Delay(1000);
    }
}
