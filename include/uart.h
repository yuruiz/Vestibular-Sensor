#ifndef UART_H
#define UART_H


#define USART_DIR P3DIR
#define USART_SEL P3SEL
#define USART_IN P3IN
#define USART_OUT P3OUT
#define UTXD0 BIT4
#define URXD0 BIT5

void UartInit();

void SendUart(unsigned char *pBuffer, unsigned char n_byte);
#endif
