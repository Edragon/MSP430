#include <msp430x14x.h>

// Simple implementation of the ANSI getchar() routine, reads from UART
unsigned char getchar(void)
{
  unsigned  char rval;
 
  while ((IFG1 & URXIFG0)==0);  //�ȴ������ַ�
  rval = RXBUF0;
  return rval;  	        /* Return received character */
}

