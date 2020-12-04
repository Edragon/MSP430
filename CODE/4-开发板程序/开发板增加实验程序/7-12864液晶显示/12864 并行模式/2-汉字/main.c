/***********************************************************
程序功能：在12864液晶上显示一屏汉字，选择并行数据传输方式
-----------------------------------------------------------
测试说明：观察液晶显示
***********************************************************/
#include  "msp430.h"
#include  "cry12864.h"
#define uchar unsigned char 
const uchar hang1[] = {"你带着老婆，出了"};
const uchar hang2[] = {"城，吃着火锅还唱"};
const uchar hang3[] = {"着歌，突然就被麻"};
const uchar hang4[] = {"匪给劫了！"};

/***************************主函数*************************/
void main( void )
{

    WDTCTL = WDTPW + WDTHOLD;    //关狗
    
    Ini_Lcd();                  //初始化液晶
    
    Disp_HZ(0x80,hang1,8);
    Disp_HZ(0x90,hang2,8);
    Disp_HZ(0x88,hang3,8);
    Disp_HZ(0x98,hang4,5);
    
    LPM4; 
}
