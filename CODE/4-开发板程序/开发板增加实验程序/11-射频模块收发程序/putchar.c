
#include <msp430x14x.h>

// Simple implementation of the ANSI putchar() routine, writes to UART
unsigned char putchar(unsigned char ch)
{
  while((IFG1 & UTXIFG0) == 0);   //�ȴ���һ�η������ 
  TXBUF0 = ch;
  return ch;
}


