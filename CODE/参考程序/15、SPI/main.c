/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

功能：SPI通信
说明：需要两个开发板或者逻辑分析仪

硬件原理图：  
//                   MSP430F5438A
//                 -----------------
//             /|\|                 |
//              | |                 |
//              --|RST              | 
//                |                 |
//                |             P3.4|-> Data Out (UCA0SIMO)
//                |                 |
//                |             P3.5|<- Data In (UCA0SOMI)
//                |                 |
//                |             P3.0|-> Serial Clock Out (UCA0CLK)

测试工具：逻辑分析仪
中景园电子
淘宝店铺：https://shop73023976.taobao.com
******************************************************************************/

#include <msp430f5438a.h>

unsigned char MST_Data_Array[10] = "12345ABCDE";

int main(void)
{
  unsigned char i;
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  P3SEL |= BIT1 + BIT2 + BIT3;              // P3.5,4,0 option select
                                            // P3.1 --> MOSI
                                            // P3.2 --> MISO
                                            // P3.3 --> CLK

  UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 0x02;                           // /2
  UCB0BR1 = 0;                              //
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCB0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  __delay_cycles(100);                      // Wait for slave to initialize

  while(1)
  {
    for(i=0; i<10; i++)
    {
      while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
      UCB0TXBUF = MST_Data_Array[i];                     // Transmit first character
    }
    __delay_cycles(1000);                      // Wait for slave to initialize
    __delay_cycles(1000);                      // Wait for slave to initialize
    __delay_cycles(1000);                      // Wait for slave to initialize
    __delay_cycles(1000);                      // Wait for slave to initialize
    __delay_cycles(1000);                      // Wait for slave to initialize
  }
}