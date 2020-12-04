/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

功能：测试DS18B20，将温度值发送到串口

测试工具：串口助手

编译环境：IAR5.3
Time：2015、8、1

By：天星科技
网址：gods-star.com
技术支持：天星科技.com
淘宝网店：gods-star.taobao.com
QQ：3184693939
******************************************************************************/
#include "main.h"

extern uchar dN[9];           //串口要显示的字符串

void main()
{
  Init_System();                        //初始化系统
  
  UART_SendString("System Start...\r\n");
    
  while(1)
  {
    Disp_Numb(Ds18b20_Convert());
    UART_SendString(dN); 
    UART_SendString("\r\n");
    Delay_ms(1000); 
  }
}