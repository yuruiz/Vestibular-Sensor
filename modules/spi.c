#include "spi.h"

char SpiMasterInit(long baud,char dataBits,char mode,char clkMode)
{
    long int brclk;                 //�����ʷ�����ʱ��Ƶ��
    
    U0CTL |= SWRST;                 //��ʼ
    
    //����ѡ��λ��Ϊ1�����͵������Լ����գ����ڲ��ԣ�����ʹ��ʱע�͵�
    //UxCTL |= LISTEN;
    
    U0CTL |= SYNC + MM;             //SPI ����ģʽ
    
    //ʱ��Դ����
    U0TCTL &=~ (SSEL0+SSEL1);       //���֮ǰ��ʱ������
    if(baud<=16364)                 //
    {
      U0TCTL |= SSEL0;              //ACLK�����͹���
      brclk = 32768;                //�����ʷ�����ʱ��Ƶ��=ACLK(32768)
    }
    else
    {
      U0TCTL |= SSEL1;              //SMCLK����֤�ٶ�
      brclk = 1000000;              //�����ʷ�����ʱ��Ƶ��=SMCLK(1MHz)
    }
    
    //------------------------���ò�����-------------------------   
    if(baud < 300||baud > 115200)   //�����ʳ�����Χ
    {
        return 0;
    }
    //���ò����ʼĴ���
    int fen = brclk / baud;         //��Ƶϵ��
    if(fen<2)return (0);            //��Ƶϵ���������2
    else
    {
        UBR00 = fen / 256;
        UBR10 = fen % 256;
    }
    
    //------------------------��������λ-------------------------    
    switch(dataBits)
    {
        case 7:case'7': U0CTL &=~ CHAR; break;      //7λ����
        case 8:case'8': U0CTL |= CHAR;  break;      //8λ����
        default :       return(0);                  //��������
    } 
    //------------------------����ģʽ---------------------------    
    switch(mode)
    {
        case 3:case'3': U0TCTL |= STC;  USPI3ON;    break;  //����ģʽ
        case 4:case'4': U0TCTL &=~ STC; USPI4ON;    break;  //����ģʽ
        default :       return(0);                          //��������
    }
    
    //------------------------����UCLKģʽ-----------------------  
    switch(clkMode)
    {
        case 0:case'0': U0TCTL &=~ CKPH; UxTCTL &=~ CKPL;   break;  //ģʽ0
        case 1:case'1': U0TCTL &=~ CKPH; UxTCTL |= CKPL;    break;  //ģʽ1
        case 2:case'2': U0TCTL |= CKPH;  UxTCTL &=~ CKPL;   break;  //ģʽ2
        case 3:case'3': U0TCTL |= CKPH;  UxTCTL |= CKPL;    break;  //ģʽ3
        default :       return(0);                                  //��������
    }
    
    UxME |= USPIEx;             //ģ��ʹ��
    
    UCTL0 &= ~SWRST;            // Initialize USART state machine
    
    UxIE |= URXIEx + UTXIEx;    // Enable USART0 RX interrupt 
    
    return(1);                  //���óɹ�
}

void SpiLpm()
{
    if(UxTCTL&SSEL0) LPM3;  //����ACLK ��ʱ�ӣ�����LPM3����(����ACLK)
    else             LPM0;  //����SMCLK��ʱ�ӣ�����LPM0����(���ر�SMCLK)
}

void SpiWriteDat(char c)
{ 
    while (TxFlag==0) SpiLpm();  // �ȴ���һ�ֽڷ��꣬������
    TxFlag=0;                     //
    U0TXBUF=c;
}

 char SpiReadDat()
{ 
    while (RxFlag==0) SpiLpm(); // �յ�һ�ֽ�?
    RxFlag=0;
    return(U0RXBUF);
}

//*******************Emulate SPI*****************************8

unsigned char SPI_Read(void)
{
        unsigned char i,temp=0;
        RF4432_SCLK_0;
        delay_nus(1);
for(i=0;i<8;i++)
{
   temp<<=1;
   RF4432_SCLK_1;
   delay_nus(1);
   if (RF4432_SDO)        //��ȡ���λ����������ĩβ��ͨ������λ��������ֽ�
   temp |= 0x01;
   RF4432_SCLK_0;
   delay_nus(1);
}
   RF4432_SCLK_0;
delay_nus(1);
return temp;
}
void SPI_Write(unsigned char txdata)
{
        unsigned char i;
       RF4432_SCLK_0;
       delay_nus(1);
      for(i=0;i<8;i++)
      { 
        RF4432_SCLK_0;
        if(0x80&txdata)
        RF4432_SDI_1;
        else RF4432_SDI_0;
       delay_nus(1);
       RF4432_SCLK_1;
        delay_nus(1);
       txdata<<=1;
      }
    RF4432_SCLK_0;
    delay_nus(1);

}
//********************************************************************************//
//* ������:                                                   RF4432_WriteReg( )                                            //
//* �������ܣ�                   RF4432�Ĵ���д�뺯��                             //
//********************************************************************************//
void  SpiWriteRegister(unsigned char  addr, unsigned char value)
{
        RF4432_SEL_0; 
        delay_nus(1);                 
        SPI_Write(addr|0x80); //��ַ���λ��1     
        SPI_Write(value);          
        RF4432_SEL_1;  
        delay_nus(1);                
}
//********************************************************************************//
//* ������:                                         RF4432_ReadReg(unsigned char  addr)                                  //
//* �������ܣ�               RF4432�Ĵ�����ȡ����                                 //
//********************************************************************************//
unsigned char  SpiReadRegister(unsigned char  addr)
{
        unsigned char value;
        RF4432_SEL_0; 
         delay_nus(1);                  
        SPI_Write(addr|0x7f);//��ַ���λ��0      
        value = SPI_Read();          
        RF4432_SEL_1; 
        delay_nus(1);         
        return value;
}