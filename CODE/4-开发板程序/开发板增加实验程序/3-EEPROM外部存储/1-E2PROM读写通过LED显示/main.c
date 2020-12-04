//P5.2－－－SDA
//P5.1－－－SCL

#include "msp430x14x.h"
#define SCL   0X40;
#include "i2c.h"
unsigned char ii;
void main(void)
{
  P5DIR = 0XFF;
  WDTCTL = WDTHOLD+WDTPW;
  P2DIR = 0xff;                   //设置P2口方向为输出
  P2OUT = 0xff;
  while(1)

  { 
    P2OUT = 0xFF;
    WriteWord(0x2A,0x10);
    _NOP();
    Delay(500);
    if(ReadWord(0x2A)==0X10);
    P2OUT = 0x0;
    _NOP();
    Delay(500);
   } 
  
} 
