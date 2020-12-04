/*msp430.c*/
/*---430常用的系统子函数---------*/
#include  <MSP430X14X.h>
#define   uchar   unsigned char
#define   uint    unsigned int
/*--------------------------------------*/
/*------------430软件复位--------------*/
void reset_mcu()
{WDTCTL=0x0000;
}
/*--------------------------------*/
/*-------设置系统时钟-------------*/
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
    CCTL0 = CCIE;    //使用捕获比较中断
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
