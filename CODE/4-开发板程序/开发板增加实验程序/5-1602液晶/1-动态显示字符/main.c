/***************************************************
�����ܣ���̬��ʾ���֡�welcome!��
----------------------------------------------------
����˵�����۲�Һ����ʾ
****************************************************/
#include  <msp430.h>
#include  "Cry1602.h"
#define uchar unsigned char
uchar *s1 = "welcome!";

void main( void )
{
    uchar i;
    WDTCTL = WDT_ADLY_250;               //�����ʱ������ʱ16ms
    
    LcdReset();
    DispStr(4,0,s1);
    LocateXY(0,9);                      //ȷ��д���ַ�����ʾλ��
    LcdWriteCommand(0x07, 1);           //������ʾ����
    for(i = 12; i > 0; i--) 
    {
        LcdWriteData(0x20);
        //��ʱ250ms
        IFG1 &= ~WDTIFG;
        while(!(IFG1 & WDTIFG));
        IFG1 &= ~WDTIFG; 
    }
    while(1)
    {
        LcdWriteCommand(0x05, 1);           //������ʾ����
        
        for(i = 24; i > 0; i--)  
        {
            LcdWriteData(0x20);
            //��ʱ250ms
            IFG1 &= ~WDTIFG;
            while(!(IFG1 & WDTIFG));
            IFG1 &= ~WDTIFG; 
        }
    
        
        LcdWriteCommand(0x07, 1);           //������ʾ����
        
        for(i = 24; i > 0; i--)
        {
            LcdWriteData(0x20);
            //��ʱ250ms
            IFG1 &= ~WDTIFG;
            while(!(IFG1 & WDTIFG));
            IFG1 &= ~WDTIFG; 
        }
    }    
}
