/***************************************************
�����ܣ���̬��ʾ�����ַ�
----------------------------------------------------
����˵�����۲�Һ����ʾ
****************************************************/
#include  <msp430.h>
#include  "Cry1602.h"
#define uchar unsigned char
uchar shuzi[] = {"0123456789"};
uchar zimu1[] = {"abcdefghigklmnopqrstuvwxyz"};
uchar zimu2[] = {"ABCDEFGHIGKLMNOPQRSTUVWXYZ"};
uchar *fuhao = "~`!@#$%^&*()_+-=/|,.��;'<>?:""";
uchar *jieshu = "This is the end!";

/***********************������************************/
void main( void )
{
    unsigned char i;
    WDTCTL = WDT_ADLY_1000;               //�����ʱ������ʱ1000ms
    
    LcdReset();
    DispNChar(3,0,10,shuzi);
    //��ʱ2s
    for(i = 0; i < 3; i++)
    {

        IFG1 &= ~WDTIFG;
        while(!(IFG1 & WDTIFG));
        IFG1 &= ~WDTIFG; 
    }
    LcdWriteCommand(0x01, 1);           //�����ʾ
    DispNChar(0,0,26,zimu1);
    //��ʱ2s
    for(i = 0; i < 3; i++)
    {

        IFG1 &= ~WDTIFG;
        while(!(IFG1 & WDTIFG));
        IFG1 &= ~WDTIFG; 
    }
    LcdWriteCommand(0x01, 1);           //�����ʾ
    DispNChar(0,0,26,zimu2);
    //��ʱ2s
    for(i = 0; i < 3; i++)
    {

        IFG1 &= ~WDTIFG;
        while(!(IFG1 & WDTIFG));
        IFG1 &= ~WDTIFG; 
    }
    LcdWriteCommand(0x01, 1);           //�����ʾ
    DispStr(0,0,fuhao);
    //��ʱ2s
    for(i = 0; i < 3; i++)
    {

        IFG1 &= ~WDTIFG;
        while(!(IFG1 & WDTIFG));
        IFG1 &= ~WDTIFG; 
    }
    LcdWriteCommand(0x01, 1);           //�����ʾ
    DispStr(0,0,jieshu);
    while(1);
}
