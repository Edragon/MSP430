/*msp430.c*/
/*---430���õ�ϵͳ�Ӻ���---------*/
#include  <MSP430X14X.h>
#define   uchar   unsigned char
#define   uint    unsigned int
/*--------------------------------------*/
/*------------430�����λ--------------*/
void reset_mcu()
{WDTCTL=0x0000;
}
/*--------------------------------*/
/*-------����ϵͳʱ��-------------*/
void set_sysclk()
{ uchar i;
  BCSCTL1 &= ~XT2OFF;
  do
  {
   IFG1 &= ~OFIFG;
   for(i=0xff;i>0;i--);
  }
  while((IFG1&OFIFG)!=0);
  BCSCTL2 |= SELM_2;  //MCLK ---8MHz
  BCSCTL2 |= SELS+DIVS_3; //SMCLK --- 1MHz
  for(i=0Xff;i>0;i--);
  //P5DIR|=0X10;
  //P5SEL|=0X10;
}
/*-------------------------------*/
/*--------------------------------*/
  void set_timer_a0()
   {TACTL |= TASSEL_2+MC_1;         
    CCR0 = 3000;
    CCTL0 = CCIE;    //ʹ�ò���Ƚ��ж�
    _EINT();
   }
/*----------------------------------*/
 void set_timer_b0()
 { 
   TBCTL |= TBSSEL_2+MC_2+TBCLR;
 }
 void stop_timer_b0()
 { 
   TBCTL |= MC_0;
 }
