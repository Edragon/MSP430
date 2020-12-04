/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

���ܣ�����MSP430F5438A-�ĸ����������°���LED״̬��ת

Ӳ��ԭ��ͼ��  LED1    -->   P1.3
              LED2    -->   P1.4
              LED3    -->   P1.5
              LED4    -->   P1.6
              
              KEY1    -->   P2.0
              KEY2    -->   P2.1
              KEY3    -->   P2.2
              KEY4    -->   P2.3

�о�԰����
�Ա����ӣ�https://shop73023976.taobao.com
******************************************************************************/

#include <msp430F5438A.h>           //ͷ�ļ�

#define CPU_F ((double)1045000UL)        //ϵͳʱ��~1.045Mhz
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) //��ʱ1us
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))    //��ʱ1ms

void main()
{
  WDTCTL = WDTPW + WDTHOLD;                         //�ؿ��Ź�
  
  P2DIR |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 ��������IO����Ϊ��� 
  P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 ����� 
  
  P1DIR |= BIT3 + BIT4 + BIT5+ BIT6;                //LED����IO����Ϊ��� 
  P1OUT &= ~(BIT3 + BIT4 + BIT5+ BIT6);             //LED���״̬����Ϊ��
  
  P2REN |= BIT0 + BIT1 + BIT2 + BIT3;               //������������ʹ��
  P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 Ϊ����
  P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3);            //P2.0~3 ����Ϊ���� - KEY  
  P2IE |= BIT0 + BIT1 + BIT2 + BIT3;                //P2.0~3 ����Ϊ�ⲿ�жϷ�ʽ
  P2IES &= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 ����Ϊ�½����ж�
  P2IFG &= ~(BIT0 + BIT1 + BIT2 + BIT3);
  
  _EINT();                                          //�����ж�
  
  while(1);
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  Delay_ms(20);
  P1OUT ^= (BIT3 + BIT4 + BIT5+ BIT6);
  P2IFG &= ~(BIT0 + BIT1 + BIT2 + BIT3);                 
}