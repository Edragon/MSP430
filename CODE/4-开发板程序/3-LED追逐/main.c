//����˵�����۲�LED��׷��Ч��
#include  <msp430x14x.h>

void delay(unsigned int cnt) //�򵥵���ʱ
{
 while(--cnt);
}

void main()
{
  WDTCTL = WDTPW + WDTHOLD;                 // �ع�
  P2DIR = 0x0f;                             // P2�˿�����Ϊ���
  P2OUT=0x0e;//����ʼ��ֵ
 while(1)
      {
	   delay(30000);//delay at crystal frequency in 12MHz
	   P2OUT<<=1;//����һλ
	   P2OUT|=0x01;//���һλ��1
	   if(P2OUT==0x77)//����Ƿ��Ƶ�LED4
          { 
		     delay(30000);//delay
	         P2OUT=0x0e;
          }
      }
}