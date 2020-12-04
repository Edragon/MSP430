/******************************************************************************
	           �о�԰���� MSP430F5438A 

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

#include <msp430F5438A.h>                 //ͷ�ļ�

#define CPU_F ((double)1045000UL)         //ϵͳʱ��~1.045Mhz
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) //��ʱ1us
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))    //��ʱ1ms

void main()
{
  WDTCTL = WDTPW + WDTHOLD;                         //�ؿ��Ź�
  
  P2DIR |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 ��������IO����Ϊ��� 
  P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 ����� 
  
  P1DIR |= BIT3 + BIT4 + BIT5+ BIT6;                //LED����IO����Ϊ��� 
  P1OUT &= ~(BIT3 + BIT4 + BIT5+ BIT6);             //LED���״̬����Ϊ��
  
  P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3);            //P2.0~3 ��������IO����Ϊ����  
  P2REN |= BIT0 + BIT1 + BIT2 + BIT3;               //������������ʹ��
  P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;               //P2.0~3 Ϊ����
  
  while(1)
  {
    if(!(P2IN&BIT0))                   //P2.0����
    {
      Delay_ms(10);
      if(!(P2IN&BIT0))                //ȷ��P2.0����
      {
        P1OUT ^= BIT3;                //LED��ʾ״̬ȡ��
        while(!(P2IN&BIT0));          //�ȴ������ɿ�
      }
    }
    
    if(!(P2IN&BIT1))                   //P2.1����
    {
      Delay_ms(10);
      if(!(P2IN&BIT1))                //ȷ��P2.1����
      {
        P1OUT ^= BIT4;                //LED��ʾ״̬ȡ��
        while(!(P2IN&BIT1));          //�ȴ������ɿ�
      }
    }
    
    if(!(P2IN&BIT2))                   //P2.2����
    {
      Delay_ms(10);
      if(!(P2IN&BIT2))                //ȷ��P2.2����
      {
        P1OUT ^= BIT5;                //LED��ʾ״̬ȡ��
        while(!(P2IN&BIT2));          //�ȴ������ɿ�
      }
    }
    
    if(!(P2IN&BIT3))                   //P2.3����
    {
      Delay_ms(10);
      if(!(P2IN&BIT3))                //ȷ��P2.3����
      {
        P1OUT ^= BIT6;                //LED��ʾ״̬ȡ��
        while(!(P2IN&BIT3));          //�ȴ������ɿ�
      }
    }    
  }
}