/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

功能：测试MSP430F5438A SD卡，将数据写入SD卡，然后从SD卡读出数据后发送到串口

说明：SD卡座在液晶背面

测试工具：串口助手、SD卡

编译环境：IAR5.3
Time：2015、8、1

By：天星科技
网址：gods-star.com
技术支持：天星科技.com
淘宝网店：gods-star.taobao.com
QQ：3184693939
******************************************************************************/

#include "main.h"

unsigned char Write_Array[15] = "ABCDEF";
unsigned char Read_Array[15];
unsigned char Write_Flag = 0, Read_Flag = 0, Init_Flag = 0;

void main()
{
  Init_System();                        //初始化系统
  Init_SPI();                           // 初始化SPI
  Init_Flag = Init_SD();      	        // SD卡初始化  
  
  Write_Flag = SdWriteBlock(Write_Array, 0, 15);  
  Read_Flag = SdReadBlock(Read_Array, 0, 15);
  
  UART_SendString("System Start...\r\n");
  
  UART_SendString("-->Write_Array: ");
  UART_SendString(Write_Array);
  UART_SendString("\r\n");
  
  UART_SendString("-->Read_Array: ");
  UART_SendString(Read_Array);
  UART_SendString("\r\n");
    
  while(1);
}