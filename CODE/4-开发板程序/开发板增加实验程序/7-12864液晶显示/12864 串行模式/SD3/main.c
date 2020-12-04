/***************************************************
程序功能：用串行数据通信方式显示图片
****************************************************/
#include  <msp430x14x.h>
#include "cryfucns.h"
#include "zd.h"

/****************主函数****************/
void main(void)
{    
    WDTCTL = WDTPW + WDTHOLD;            //关闭看门狗
    
    Ini_Lcd();
    Clear_GDRAM();
    
    Draw_PM(zdf);                    //显示阴码格式的子弹飞画面                           
    
    LPM4;
}
