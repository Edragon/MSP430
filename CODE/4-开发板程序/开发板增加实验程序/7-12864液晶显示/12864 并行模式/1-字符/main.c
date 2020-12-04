/*************************************************************
程序功能：在12864液晶上显示ASCII常用字符
**************************************************************/
#include  "msp430.h"
#include  "cry12864.h"

/***************************主函数***************************/
void main( void )
{
    unsigned int i,j;
    unsigned char tishi[] = {"This is the end!"};
    
    WDTCTL = WDTPW + WDTHOLD;    //关狗
    Ini_Lcd();                  //初始化液晶
    
    Write_Cmd(0x80);            //写第一行的显示地址
    for(i = 0; i < 16; i++)
      Write_Data(0x30 + i);     //显示0x30~0x3f对应的字符
    Write_Cmd(0x90);            //写第二行的显示地址
    for(i = 0; i < 16; i++)
      Write_Data(0x40 + i);     //显示0x40~0x4f对应的字符
    Write_Cmd(0x88);            //写第三行的显示地址
    for(i = 0; i < 16; i++)
      Write_Data(0x50 + i);     //显示0x50~0x5f对应的字符
    Write_Cmd(0x98);            //写第四行的显示地址
    for(i = 0; i < 16; i++)
      Write_Data(0x60 + i);     //显示0x60~0x6f对应的字符
    
    for(i = 1000; i > 0; i--)        //延时一会
    {
        for(j = 1000; j > 0; j--)
          _NOP();    
    }
    
    Write_Cmd(0x01);            //清屏
    
    Write_Cmd(0x80);            //写第一行的显示地址
    for(i = 0; i < 16; i++)
      Write_Data(0x70 + i);     //显示0x70~0x7f对应的字符
    
    Write_Cmd(0x88);            //写第三行的显示地址
    for(i = 0; i < 16; i++)
      Write_Data(tishi[i]);     //显示自定义的字符
    
    LPM4;
}
