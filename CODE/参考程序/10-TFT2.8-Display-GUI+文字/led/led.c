/******************************************************************************
	            MSP430F5438A��СϵͳV4.0

                       P1.3     |-->  LED1
                       P1.4     |-->  LED2
                       P1.5     |-->  LED3
                       P1.6     |-->  LED4

******************************************************************************/

#include "led.h"
#include "../sys/sys.h"

void Init_LED()
{
  P1OUT &= ~(BIT3 + BIT4 + BIT5 + BIT6);    //LED ��   
  P1DIR |= BIT3 + BIT4 + BIT5 + BIT6;       //LED IO����Ϊ���
}

void LED_ON()
{
  P1OUT |= (BIT3 + BIT4 + BIT5 + BIT6);     //LED ��
}

void LED_OFF()
{
  P1OUT &= ~(BIT3 + BIT4 + BIT5 + BIT6);    //LED ��
}