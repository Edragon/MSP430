#include<msp430x14x.h>
#define disp_data  P4OUT
//#define dig P6OUT
#define uchar unsigned char
#define uint  unsigned int
unsigned char seg_bcd[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//�������ʾ����0-9
 // CCRO ����Ƚ��жϴ���
 #pragma vector=TIMERA0_VECTOR
 __interrupt void Timer_A0 (void)
{ 
   CCR0  = 1500;          //��������CCR0����/�Ƚ����ݼĴ�������,���������
   P4DIR |= 0X0;//����
   //P6DIR |= 0xff;//λ��
   if(dat_code>9)dat_code=10;
   switch (dat_code)      //dat_codeΪ������,��ͬ��ң������ͬ����ʹ�÷������鿴�õ��İ���ֵ���޸�case������ֵ
    { case 0x4c:disp_data=seg_bcd[1];break;//����ң��������1��ʹ���ҵ�ң�����õ���dat_codeΪ4c��
      case 0x2b:disp_data=seg_bcd[2];break;//����ң��������2
      case 0xa3:disp_data=seg_bcd[3];break;//����ң��������3
      case 0x28:disp_data=seg_bcd[4];break;//����ң��������4�������������Ҫ��ʾ����ֵ
      default:break;
    }
}

