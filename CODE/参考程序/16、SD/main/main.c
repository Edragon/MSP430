/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

功能：测试SD卡，将数据“ABCDEF”写入SD卡的扇区，然后从扇区中将数据读出存在Read_Array中
说明：本程序需要使用仿真器设置断点查看Read_Array中的值是不是“ABCDEF”，如果是
      则SD卡的写入和读取测试成功。
      SD卡座在液晶背面

编译环境：IAR5.3
Time：2015、8、1

中景园电子
淘宝店铺：https://shop73023976.taobao.com
******************************************************************************/
#include "main.h"

unsigned char Write_Array[15] = "ABCDEF";
unsigned char Read_Array[20];
unsigned char Write_Flag = 0, Read_Flag = 0, Init_Flag = 0;

void main()
{
  WDTCTL = WDTPW + WDTHOLD;             //关闭看门狗
  
  Init_SPI();                           // 初始化SPI
  Init_Flag = Init_SD();      	        // SD卡初始化  
  Write_Flag = SdWriteBlock(Write_Array, 0, 15);  
  Read_Flag = SdReadBlock(Read_Array, 0, 15);
  
  while(1);
}