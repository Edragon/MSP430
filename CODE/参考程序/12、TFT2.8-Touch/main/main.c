/******************************************************************************
中景园电子 	            MSP430F5438A 
店铺连接：https://shop73023976.taobao.com/

功能：测试MSP430F5438A - 2.8寸TFT液晶触摸
  
说明：触摸的地方打点

编译环境：IAR5.
中景园电子 	            MSP430F5438A 
店铺连接：https://shop73023976.taobao.com/
******************************************************************************/
 
#include "main.h"

void main(void)                    //主函数 
{ 
  Init_System();                   //初始化时钟系统、LED
  
  CLR_Screen(White);               //用背景色清屏
  
  while(1)
  {	
    if(Getpix() == 1)  
    {
      drawpoint(lx,ly,Red);       //写（x，y）点为红色
      Delay_ms(1);
    } 
  }
} 
