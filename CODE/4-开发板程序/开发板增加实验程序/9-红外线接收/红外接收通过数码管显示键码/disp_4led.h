#include<msp430x14x.h>
#define disp_data  P4OUT
//#define dig P6OUT
#define uchar unsigned char
#define uint  unsigned int
unsigned char seg_bcd[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//数码管显示数据0-9
 // CCRO 捕获比较中断处理
 #pragma vector=TIMERA0_VECTOR
 __interrupt void Timer_A0 (void)
{ 
   CCR0  = 1500;          //重新载入CCR0捕获/比较数据寄存器数据,共阳数码管
   P4DIR |= 0X0;//段码
   //P6DIR |= 0xff;//位码
   if(dat_code>9)dat_code=10;
   switch (dat_code)      //dat_code为按键码,不同的遥控器不同，请使用仿真器查看得到的按键值后修改case后面数值
    { case 0x4c:disp_data=seg_bcd[1];break;//按下遥控器按键1（使用我的遥控器得到的dat_code为4c）
      case 0x2b:disp_data=seg_bcd[2];break;//按下遥控器按键2
      case 0xa3:disp_data=seg_bcd[3];break;//按下遥控器按键3
      case 0x28:disp_data=seg_bcd[4];break;//按下遥控器按键4，你可以增加需要显示的数值
      default:break;
    }
}

