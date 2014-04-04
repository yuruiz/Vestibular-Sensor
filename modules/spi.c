#include "spi.h"

char SpiMasterInit(long baud,char dataBits,char mode,char clkMode)
{
    long int brclk;                 //波特率发生器时钟频率
    
    U0CTL |= SWRST;                 //初始
    
    //反馈选择位，为1，发送的数被自己接收，用于测试，正常使用时注释掉
    //UxCTL |= LISTEN;
    
    U0CTL |= SYNC + MM;             //SPI 主机模式
    
    //时钟源设置
    U0TCTL &=~ (SSEL0+SSEL1);       //清除之前的时钟设置
    if(baud<=16364)                 //
    {
      U0TCTL |= SSEL0;              //ACLK，降低功耗
      brclk = 32768;                //波特率发生器时钟频率=ACLK(32768)
    }
    else
    {
      U0TCTL |= SSEL1;              //SMCLK，保证速度
      brclk = 1000000;              //波特率发生器时钟频率=SMCLK(1MHz)
    }
    
    //------------------------设置波特率-------------------------   
    if(baud < 300||baud > 115200)   //波特率超出范围
    {
        return 0;
    }
    //设置波特率寄存器
    int fen = brclk / baud;         //分频系数
    if(fen<2)return (0);            //分频系数必须大于2
    else
    {
        UBR00 = fen / 256;
        UBR10 = fen % 256;
    }
    
    //------------------------设置数据位-------------------------    
    switch(dataBits)
    {
        case 7:case'7': U0CTL &=~ CHAR; break;      //7位数据
        case 8:case'8': U0CTL |= CHAR;  break;      //8位数据
        default :       return(0);                  //参数错误
    } 
    //------------------------设置模式---------------------------    
    switch(mode)
    {
        case 3:case'3': U0TCTL |= STC;  USPI3ON;    break;  //三线模式
        case 4:case'4': U0TCTL &=~ STC; USPI4ON;    break;  //四线模式
        default :       return(0);                          //参数错误
    }
    
    //------------------------设置UCLK模式-----------------------  
    switch(clkMode)
    {
        case 0:case'0': U0TCTL &=~ CKPH; UxTCTL &=~ CKPL;   break;  //模式0
        case 1:case'1': U0TCTL &=~ CKPH; UxTCTL |= CKPL;    break;  //模式1
        case 2:case'2': U0TCTL |= CKPH;  UxTCTL &=~ CKPL;   break;  //模式2
        case 3:case'3': U0TCTL |= CKPH;  UxTCTL |= CKPL;    break;  //模式3
        default :       return(0);                                  //参数错误
    }
    
    UxME |= USPIEx;             //模块使能
    
    UCTL0 &= ~SWRST;            // Initialize USART state machine
    
    UxIE |= URXIEx + UTXIEx;    // Enable USART0 RX interrupt 
    
    return(1);                  //设置成功
}

void SpiLpm()
{
    if(UxTCTL&SSEL0) LPM3;  //若以ACLK 作时钟，进入LPM3休眠(仅打开ACLK)
    else             LPM0;  //若以SMCLK作时钟，进入LPM0休眠(不关闭SMCLK)
}

void SpiWriteDat(char c)
{ 
    while (TxFlag==0) SpiLpm();  // 等待上一字节发完，并休眠
    TxFlag=0;                     //
    U0TXBUF=c;
}

 char SpiReadDat()
{ 
    while (RxFlag==0) SpiLpm(); // 收到一字节?
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
   if (RF4432_SDO)        //读取最高位，保存至最末尾，通过左移位完成整个字节
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
//* 函数名:                                                   RF4432_WriteReg( )                                            //
//* 函数功能：                   RF4432寄存器写入函数                             //
//********************************************************************************//
void  SpiWriteRegister(unsigned char  addr, unsigned char value)
{
        RF4432_SEL_0; 
        delay_nus(1);                 
        SPI_Write(addr|0x80); //地址最高位置1     
        SPI_Write(value);          
        RF4432_SEL_1;  
        delay_nus(1);                
}
//********************************************************************************//
//* 函数名:                                         RF4432_ReadReg(unsigned char  addr)                                  //
//* 函数功能：               RF4432寄存器读取函数                                 //
//********************************************************************************//
unsigned char  SpiReadRegister(unsigned char  addr)
{
        unsigned char value;
        RF4432_SEL_0; 
         delay_nus(1);                  
        SPI_Write(addr|0x7f);//地址最高位置0      
        value = SPI_Read();          
        RF4432_SEL_1; 
        delay_nus(1);         
        return value;
}