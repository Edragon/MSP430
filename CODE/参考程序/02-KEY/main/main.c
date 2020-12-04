/******************************************************************************
	           中景园电子 MSP430F5438A 

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

#include <msp430F5438A.h>                 //头文件

#define CPU_F ((double)1045000UL)         //系统时钟~1.045Mhz
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) //延时1us
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))    //延时1ms

void main()
{
  WDTCTL = WDTPW + WDTHOLD;                         //关看门狗
  
  P2DIR |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 按键所在IO设置为输出 
  P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 输出高 
  
  P1DIR |= BIT3 + BIT4 + BIT5+ BIT6;                //LED所在IO设置为输出 
  P1OUT &= ~(BIT3 + BIT4 + BIT5+ BIT6);             //LED输出状态设置为灭
  
  P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3);            //P2.0~3 按键所在IO设置为输入  
  P2REN |= BIT0 + BIT1 + BIT2 + BIT3;               //上拉下拉电阻使能
  P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 为上拉
  
  while(1)
  {
    if(!(P2IN&BIT0))                   //P2.0按下
    {
      Delay_ms(10);
      if(!(P2IN&BIT0))                //确认P2.0按下
      {
        P1OUT ^= BIT3;                //LED显示状态取反
        while(!(P2IN&BIT0));          //等待按键松开
      }
    }
    
    if(!(P2IN&BIT1))                   //P2.1按下
    {
      Delay_ms(10);
      if(!(P2IN&BIT1))                //确认P2.1按下
      {
        P1OUT ^= BIT4;                //LED显示状态取反
        while(!(P2IN&BIT1));          //等待按键松开
      }
    }
    
    if(!(P2IN&BIT2))                   //P2.2按下
    {
      Delay_ms(10);
      if(!(P2IN&BIT2))                //确认P2.2按下
      {
        P1OUT ^= BIT5;                //LED显示状态取反
        while(!(P2IN&BIT2));          //等待按键松开
      }
    }
    
    if(!(P2IN&BIT3))                   //P2.3按下
    {
      Delay_ms(10);
      if(!(P2IN&BIT3))                //确认P2.3按下
      {
        P1OUT ^= BIT6;                //LED显示状态取反
        while(!(P2IN&BIT3));          //等待按键松开
      }
    }    
  }
}