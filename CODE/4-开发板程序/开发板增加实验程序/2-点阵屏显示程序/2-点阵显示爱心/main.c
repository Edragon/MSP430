/*
������ʾʵ�飬��ʾ����ͼ��
 */

#include <msp430x14x.h>

unsigned char const line[]={0x00,0x6C,0x92,0x82,0x44,0x28,0x10,0x00};// �ĵ���״
unsigned char const seg[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe}; //�ֱ��Ӧ��Ӧ������

void delay(unsigned int cnt)
{
 while(--cnt);
}

main()
{
 P1OUT=0XFF;
 P2OUT=0XFF;
 unsigned char i;
 while(1)
      {
 
           P1OUT=line[i]; //ȡ��ʾ����
	   P2OUT=seg[i];  //ȡ����
	   delay(200);    //ÿ��ɨ���϶��ʱ
	   i++;
	   if(8==i)
           {
             i=0;
           }
      }
}