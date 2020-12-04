#include <msp430.h>
#include "i2c.h"
#include <ctype.h> 

typedef unsigned char uchar;

/*******************************************
函数名称：InitUART
功    能：初始化UART端口
参    数：无
返回值  ：无
********************************************/
void InitUART(void)
{
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= URXE0 + UTXE0;                             // Enable USART0 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
}
/*******************************************
函数名称：Send1Char
功    能：向PC机发送一个字符
参    数：sendchar--要发送的字符
返回值  ：无
********************************************/
void Send1Char(uchar sendchar)
{
      while (!(IFG1 & UTXIFG0));    //等待发送寄存器为空         
      TXBUF0 = sendchar;   
}
/*******************************************
函数名称：PutSting
功    能：向PC机发送字符串
参    数：ptr--指向发送字符串的指针
返回值  ：无
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            Send1Char(*ptr++);                     // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';                              //发送换行指令
}
/*******************************************
函数名称：Get1Char
功    能：接收一个来自PC机的字符
参    数：无
返回值  ：接收到的字符
********************************************/
uchar Get1Char(void)
{
    while (!(IFG1 & URXIFG0));    //等待接收到字符        
    return  RXBUF0;
}
/*******************************************
函数名称：Get1Char
功    能：接收一个来自PC机的字符
参    数：无
返回值  ：接收到的字符
********************************************/
void DisplayConfigMenu(void)
{
    PutString("Choose Read or Write operation:");
    PutString("R-- Read a char from EEPROM");
    PutString("W-- Write a char from EEPROM");
    PutString("*-- any other key to redispaly the menu");
}
/*******************************************
函数名称：HandleConfigMenu
功    能：处理PC的配置函数
参    数：inputvalue--接收到的来自PC机的字符
返回值  ：无
********************************************/
void HandleConfigMenu(uchar inputvalue)
{
    uchar addr,wdata,result;
    uchar hi,lo;
    switch(toupper(inputvalue)) 
    {
    case 'R':
              PutString("Please send the memory address to read:");       
              addr = Get1Char();
              Send1Char('>'); Send1Char('\n');
              result = ReadWord(addr);
              Send1Char(0x30);        //屏幕显示0
              Send1Char(0x78);        //屏幕显示x
              hi = (result & 0xf0) >> 4; 
              if(hi > 0x09)
              {
                  Send1Char(0x37 + hi);    //屏幕显示字母（A~F）
              }
              else
              {
                  Send1Char(0x30 + hi);     //屏幕显示数字
              }
              lo = result & 0x0f;
              if(lo > 0x09)
              {
                  Send1Char(0x37 + lo);    //屏幕显示字母（A~F）
              }
              else
              {
                  Send1Char(0x30 + lo);     //屏幕显示数字
              }
              Send1Char('\n');
              break;
    case 'W':
             PutString("Please send the memory address to write:");
              addr = Get1Char();
              Send1Char('>'); Send1Char('\n');
              PutString("Please send the data to write:");
              wdata = Get1Char();
              Send1Char('>'); Send1Char('\n');
               if( WriteWord(addr,wdata) )
              {  
                 PutString("Write successfully!");    
              }
              else
              {
                  PutString("Fail to write!");
              }
              break;

    default:
              DisplayConfigMenu();  
    }
}

