
#include <msp430x14x.h>

// Simple implementation of the ANSI putchar() routine, writes to UART
unsigned char putchar(unsigned char ch)
{
  while((IFG1 & UTXIFG0) == 0);   //等待上一次发送完毕 
  TXBUF0 = ch;
  return ch;
}


