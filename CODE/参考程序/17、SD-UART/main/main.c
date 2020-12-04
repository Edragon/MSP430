/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

���ܣ�����MSP430F5438A SD����������д��SD����Ȼ���SD���������ݺ��͵�����

˵����SD������Һ������

���Թ��ߣ��������֡�SD��

���뻷����IAR5.3
Time��2015��8��1

By�����ǿƼ�
��ַ��gods-star.com
����֧�֣����ǿƼ�.com
�Ա����꣺gods-star.taobao.com
QQ��3184693939
******************************************************************************/

#include "main.h"

unsigned char Write_Array[15] = "ABCDEF";
unsigned char Read_Array[15];
unsigned char Write_Flag = 0, Read_Flag = 0, Init_Flag = 0;

void main()
{
  Init_System();                        //��ʼ��ϵͳ
  Init_SPI();                           // ��ʼ��SPI
  Init_Flag = Init_SD();      	        // SD����ʼ��  
  
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