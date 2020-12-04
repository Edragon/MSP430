/***********************************************************
程序功能：在液晶上用串行数据通信方式依次显示常用ASCII码

************************************************************/
#include  <msp430x14x.h>
#include "cryfucns.h"

void main(void)
{
    unsigned char i;
    
    WDTCTL = WDTPW + WDTHOLD;            //关闭看门狗
  
    Ini_Lcd();                           //初始化液晶
    
    Send(0x80,0);                        //显示的起始地址
    for(i = 0; i < 16; i++)
    {
        Send(0x30+i,0);                  //0x30~0x3f对应的ASCII字符
    }
    Send(0x90,0);                        //显示的起始地址
    for(i = 0; i < 16; i++)
    {
        Send(0x40+i,0);                  //0x40~0x4f对应的ASCII字符
    }
    Send(0x88,0);                        //显示的起始地址
    for(i = 0; i < 16; i++)
    {
        Send(0x50+i,0);                  //0x50~0x5f对应的ASCII字符
    }
    Send(0x98,0);                        //显示的起始地址
    for(i = 0; i < 16; i++)
    {
        Send(0x60+i,0);                  //0x60~0x6f对应的ASCII字符
    }
    
    _NOP();                              //此处设置断点观察液晶屏幕显示
    
    Send(0x80,0);                        //显示的起始地址
    for(i = 0; i < 16; i++)
    {
        Send(0x70+i,0);                  //0x70~0x7f对应的ASCII字符
    }
    
    LPM4;
}
    
