/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

功能：测试MSP430F5438A-四个按键，按下按键LED状态翻转

硬件原理图：  LED1    -->   P1.3
              LED2    -->   P1.4
              LED3    -->   P1.5
              LED4    -->   P1.6
              
              KEY1    -->   P2.0
              KEY2    -->   P2.1
              KEY3    -->   P2.2
              KEY4    -->   P2.3

中景园电子
淘宝连接：https://shop73023976.taobao.com
******************************************************************************/

#include <msp430F5438A.h>           //头文件

#define CPU_F ((double)1045000UL)        //系统时钟~1.045Mhz
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) //延时1us
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))    //延时1ms

void main()
{
  WDTCTL = WDTPW + WDTHOLD;                         //关看门狗
  
  P2DIR |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 按键所在IO设置为输出 
  P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 输出高 
  
  P1DIR |= BIT3 + BIT4 + BIT5+ BIT6;                //LED所在IO设置为输出 
  P1OUT &= ~(BIT3 + BIT4 + BIT5+ BIT6);             //LED输出状态设置为灭
  
  P2REN |= BIT0 + BIT1 + BIT2 + BIT3;               //上拉下拉电阻使能
  P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 为上拉
  P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3);            //P2.0~3 设置为输入 - KEY  
  P2IE |= BIT0 + BIT1 + BIT2 + BIT3;                //P2.0~3 配置为外部中断方式
  P2IES &= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 配置为下降沿中断
  P2IFG &= ~(BIT0 + BIT1 + BIT2 + BIT3);
  
  _EINT();                                          //开总中断
  
  while(1);
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  Delay_ms(20);
  P1OUT ^= (BIT3 + BIT4 + BIT5+ BIT6);
  P2IFG &= ~(BIT0 + BIT1 + BIT2 + BIT3);                 
}