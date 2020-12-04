
#include  <MSP430x14x.h>

#define   uchar   unsigned char
#define   uint    unsigned int
#define t_3ms5  2300
#define t_1ms   666
//#define rd P1IN
uchar get_code[4] = {0};
uchar dat_code=0;
uint  timer,i;
#include "msp430.h"
#include "disp_4led.h"

void delay(uint time)    //10ms--10000
{
 while(time--);
}

void dat_high()
{P1DIR |= BIT6;
 P1OUT |= BIT6;
}
void dat_low()
{P1DIR |= BIT6;
 P1OUT &= ~BIT6;
}
uchar rd_dat()
{ uchar stat;
 // P3DIR |= BIT3;
//  P3OUT |= BIT3;
 //P3DIR &= ~BIT3;
 stat = P1IN;
  return (stat);
}

uchar get_num()
{ uchar i,j,rd,dat=0;
  _DINT();             //关闭中断
  for(j=0;j<4;j++)
  { for(i=8;i>0;i--)
     {dat>>=1; 
      do
       rd=rd_dat();
      while(!(rd & BIT6));	//	wait high
      set_timer_b0();		//检测高电平时间
      do
      rd=rd_dat();
      while(rd & BIT6);        	//wait low
      timer=TBR;
      stop_timer_b0();
	  if(timer>t_1ms)dat=dat|0x80;
      else dat=dat&0x7f;  	  
      }
   get_code[j]=dat;
   }
 _EINT(); 
return(get_code[2]);             //获取按键码
}

void main( void )
{ 
  P2DIR=0XFF;
  P2OUT=0XFF;
  P1DIR = 0xBF;
  
  
  uchar rd;
  WDTCTL = WDTPW + WDTHOLD; 
  //rd_dat();
  //dat_high();
  set_timer_a0();
  while(1)
  {
judge: 
  do	  
  rd=rd_dat();
  while(P1IN & BIT6); 	//wait low
  delay(100); 
  P2OUT=0X0;
  rd=rd_dat();
  if(P1IN & BIT6)goto judge; //xiao dou 
  do
  rd=rd_dat();
  while(!(rd & BIT6));	 //wait high
  set_timer_b0();
  do
  rd=rd_dat();
  while(rd & BIT6);	 //wait low
  timer=TBR;
  stop_timer_b0(); 
  if(timer<t_3ms5){delay(1000);goto judge;}//连续按下，重复标志
  dat_code=get_num();   //取得按键码
  P2OUT=dat_code;       //LED显示
  //while(1);
  for(i=0;i<100;i++)
  delay(1000);
  goto judge; 
}
}