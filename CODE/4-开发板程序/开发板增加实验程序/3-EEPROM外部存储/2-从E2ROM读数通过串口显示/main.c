/*********************************************************
程序功能：PC机通过串口可以控制从EEPROM的指定地址中读出一个
          字节的数据，或者向指定地址写入一个字节的数据
特别说明：在PC机发送EEPROM内的存储地址或者是写入数据时，必须
          使用十六进制发送，而在发送读写控制命令时要使用默认
          的发送ASCII码的设置，这点要注意，否则操作无法完成
-----------------------------------------------------------
通信格式：N.8.1,  9600
-----------------------------------------------------------
测试说明：打开串口调试精灵，正确设置通信格式，根据屏幕显示的
          提示信息发送控制数据进行测试
**********************************************************/
#include <msp430.h>
#include "uart0.h"
#define uchar unsigned char
uchar i;

uchar step = 0xff;
unsigned char ii;
void main( void )
{
  
    
    WDTCTL = WDTPW + WDTHOLD;           //关狗
    
    P5DIR |= BIT2 + BIT1;               //设置相应端口为输出状态
    P5OUT |= BIT2 + BIT1;
    InitUART();                         //初始化UART端口
    
    DisplayConfigMenu();
     
    while(1)
    {
        Send1Char('>');
        Send1Char('\n');
        i=Get1Char();
        HandleConfigMenu(i);
    }
    
}
