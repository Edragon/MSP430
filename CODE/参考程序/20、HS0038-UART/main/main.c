/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

���ܣ�����HS0038�����������ֵ���͵�����

���Թ��ߣ��������֡�����ң����

���뻷����IAR5.3
Time��2015��8��1

By�����ǿƼ�
��ַ��gods-star.com
����֧�֣����ǿƼ�.com
�Ա����꣺gods-star.taobao.com
QQ��3184693939
******************************************************************************/

#include "main.h"

void main()
{
  uchar Decode_Value = 'Z';
  uchar Uart_Decode_Array[10] = "HS0038: -";
  
  Init_System();                        //��ʼ��ϵͳ
  
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