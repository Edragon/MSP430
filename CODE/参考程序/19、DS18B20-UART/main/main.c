/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

���ܣ�����DS18B20�����¶�ֵ���͵�����

���Թ��ߣ���������

���뻷����IAR5.3
Time��2015��8��1

By�����ǿƼ�
��ַ��gods-star.com
����֧�֣����ǿƼ�.com
�Ա����꣺gods-star.taobao.com
QQ��3184693939
******************************************************************************/
#include "main.h"

extern uchar dN[9];           //����Ҫ��ʾ���ַ���

void main()
{
  Init_System();                        //��ʼ��ϵͳ
  
  UART_SendString("System Start...\r\n");
    
  while(1)
  {
    Disp_Numb(Ds18b20_Convert());
    UART_SendString(dN); 
    UART_SendString("\r\n");
    Delay_ms(1000); 
  }
}