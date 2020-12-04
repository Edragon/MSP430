//测试说明：观察LED的追逐效果
#include  <msp430x14x.h>

void delay(unsigned int cnt) //简单的延时
{
 while(--cnt);
}

void main()
{
  WDTCTL = WDTPW + WDTHOLD;                 // 关狗
  P2DIR = 0x0f;                             // P2端口设置为输出
  P2OUT=0x0e;//给初始化值
 while(1)
      {
	   delay(30000);//delay at crystal frequency in 12MHz
	   P2OUT<<=1;//左移一位
	   P2OUT|=0x01;//最后一位补1
	   if(P2OUT==0x77)//检测是否移到LED4
          { 
		     delay(30000);//delay
	         P2OUT=0x0e;
          }
      }
}