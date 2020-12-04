/******************************************************************************
	            MSP430F5438A最小系统V4.0

                       P1.3     |-->  LED1
                       P1.4     |-->  LED2
                       P1.5     |-->  LED3
                       P1.6     |-->  LED4

******************************************************************************/

#include "led.h"
#include "../sys/sys.h"

void Init_LED()
{
  P1OUT &= ~(BIT3 + BIT4 + BIT5 + BIT6);    //LED 灭   
  P1DIR |= BIT3 + BIT4 + BIT5 + BIT6;       //LED IO设置为输出
}

void LED_ON()
{
  P1OUT |= (BIT3 + BIT4 + BIT5 + BIT6);     //LED 亮
}

void LED_OFF()
{
  P1OUT &= ~(BIT3 + BIT4 + BIT5 + BIT6);    //LED 灭
}