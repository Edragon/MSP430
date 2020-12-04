/***************************************************
程序功能：静态显示各种字符
----------------------------------------------------
测试说明：观察液晶显示
****************************************************/
#include  <msp430.h>
#include  "Cry1602.h"
#define uchar unsigned char
uchar shuzi[] = {"0123456789"};
uchar zimu1[] = {"abcdefghigklmnopqrstuvwxyz"};
uchar zimu2[] = {"ABCDEFGHIGKLMNOPQRSTUVWXYZ"};
uchar *fuhao = "~`!@#$%^&*()_+-=/|,.、;'<>?:""";
uchar *jieshu = "This is the end!";

/***********************主函数************************/
void main( void )
{
    unsigned char i;
    WDTCTL = WDT_ADLY_1000;               //间隔定时器，定时1000ms
    
    LcdReset();
    DispNChar(3,0,10,shuzi);
    //延时2s
    for(i = 0; i < 3; i++)
    {

        IFG1 &= ~WDTIFG;
        while(!(IFG1 & WDTIFG));
        IFG1 &= ~WDTIFG; 
    }
    LcdWriteCommand(0x01, 1);           //清除显示
    DispNChar(0,0,26,zimu1);
    //延时2s
    for(i = 0; i < 3; i++)
    {

        IFG1 &= ~WDTIFG;
        while(!(IFG1 & WDTIFG));
        IFG1 &= ~WDTIFG; 
    }
    LcdWriteCommand(0x01, 1);           //清除显示
    DispNChar(0,0,26,zimu2);
    //延时2s
    for(i = 0; i < 3; i++)
    {

        IFG1 &= ~WDTIFG;
        while(!(IFG1 & WDTIFG));
        IFG1 &= ~WDTIFG; 
    }
    LcdWriteCommand(0x01, 1);           //清除显示
    DispStr(0,0,fuhao);
    //延时2s
    for(i = 0; i < 3; i++)
    {

        IFG1 &= ~WDTIFG;
        while(!(IFG1 & WDTIFG));
        IFG1 &= ~WDTIFG; 
    }
    LcdWriteCommand(0x01, 1);           //清除显示
    DispStr(0,0,jieshu);
    while(1);
}
