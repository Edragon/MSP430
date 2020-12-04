/***********************************************
程序功能：对应4X4键盘的键值在1602液晶上显示
************************************************/
#include <msp430x14x.h>
#include "Keypad.h"
#include "cry1602.h"
#define uchar unsigned char
//引用外部变量的声明    
extern unsigned char key_val;         
extern unsigned char key_Flag; 
/********************主函数********************/
void main(void)
{
    uchar Strings[]={"Key Value:"};
    uchar temp;
    
    WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗   
    
    LcdReset();                     //初始化LCD
    Init_Keypad();                  //初始化键盘端口
    DispNChar(2,0,10,Strings);        //显示提示文字
    while(1)
    {
        Key_Event();
    
        if(key_Flag == 1)
        {
            key_Flag = 0;
            
            //显示十位
            temp = key_val / 10;
            if(temp)
                Disp1Char(6,1,0x30+temp);  //数字的ASCII码值等于其本身数值加上0x30
            else
                Disp1Char(6,1,0x20);       //0x20是空格对应的ASCII码
            //显示个位
            temp = key_val % 10;
            Disp1Char(7,1,0x30+temp);   
        }
    }
}

