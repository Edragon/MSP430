/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

功能：测试HS0038，将红外解码值发送到串口

测试工具：串口助手、红外遥控器

编译环境：IAR5.3
Time：2015、8、1

By：天星科技
网址：gods-star.com
技术支持：天星科技.com
淘宝网店：gods-star.taobao.com
QQ：3184693939
******************************************************************************/

#include "main.h"

void main()
{
  uchar Decode_Value = 'Z';
  uchar Uart_Decode_Array[10] = "HS0038: -";
  
  Init_System();                        //初始化系统
  
  UART_SendString("System Start...\r\n");
  UART_SendString(Uart_Decode_Array);
  UART_SendString("\r\n");
    
  while(1)
  {
    Decode_Value = Decode_HS0038();
    if(Decode_Value != 'Z')
    {
      Uart_Decode_Array[8] = Decode_Value;
      Uart_Decode_Array[9] = '\0';
      UART_SendString(Uart_Decode_Array);
      Delay_ms(50);
    }  
  }
}