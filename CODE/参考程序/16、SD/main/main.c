/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

���ܣ�����SD���������ݡ�ABCDEF��д��SD����������Ȼ��������н����ݶ�������Read_Array��
˵������������Ҫʹ�÷��������öϵ�鿴Read_Array�е�ֵ�ǲ��ǡ�ABCDEF���������
      ��SD����д��Ͷ�ȡ���Գɹ���
      SD������Һ������

���뻷����IAR5.3
Time��2015��8��1

�о�԰����
�Ա����̣�https://shop73023976.taobao.com
******************************************************************************/
#include "main.h"

unsigned char Write_Array[15] = "ABCDEF";
unsigned char Read_Array[20];
unsigned char Write_Flag = 0, Read_Flag = 0, Init_Flag = 0;

void main()
{
  WDTCTL = WDTPW + WDTHOLD;             //�رտ��Ź�
  
  Init_SPI();                           // ��ʼ��SPI
  Init_Flag = Init_SD();      	        // SD����ʼ��  
  Write_Flag = SdWriteBlock(Write_Array, 0, 15);  
  Read_Flag = SdReadBlock(Read_Array, 0, 15);
  
  while(1);
}