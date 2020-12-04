/*
点阵显示实验，显示爱心图像
 */

#include <msp430x14x.h>

unsigned char const line[]={0x00,0x6C,0x92,0x82,0x44,0x28,0x10,0x00};// 心的形状
unsigned char const seg[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe}; //分别对应相应的行亮

void delay(unsigned int cnt)
{
 while(--cnt);
}

main()
{
 P1OUT=0XFF;
 P2OUT=0XFF;
 unsigned char i;
 while(1)
      {
 
           P1OUT=line[i]; //取显示数据
	   P2OUT=seg[i];  //取段码
	   delay(200);    //每行扫描间隙延时
	   i++;
	   if(8==i)
           {
             i=0;
           }
      }
}