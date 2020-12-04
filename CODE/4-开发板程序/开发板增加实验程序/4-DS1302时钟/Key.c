#include <msp430x14x.h>
typedef unsigned char uchar;
typedef unsigned int  uint;

#define keyin    (P1IN & 0x0f)

/*******************************************
函数名称：delay
功    能：用于消抖的延时
参    数：无
返回值  ：无
********************************************/
void delay_10ms(void)
{
    uint tmp;
    
    for(tmp = 0x3fff;tmp > 0;tmp--);
}
/*******************************************
函数名称：Key4Scan
功    能：扫描四个独立式按键
参    数：无
返回值  ：键值
********************************************/
uchar Key4Scan(void)
{
    uchar temp,keyval;
    
    if(keyin != 0x0f)       //如果有键被按下
    {
        delay_10ms();            //延时消抖
        keyval = 0;
        if(keyin != 0x0f)   //再次检测按键状态
        {
            temp=keyin;
            while(keyin != 0x0f);   //等待按键被放开
            switch(temp)    //转换键值    
            {
            case 0x0e:
                        keyval = 1;break;
            case 0x0d:
                        keyval = 2;break;
            case 0x0b:
                        keyval = 3;break;
            case 0x07:
                        keyval = 4;break;
            default:
                        keyval = 0;break;
            }
        }
    }
    else
        keyval = 0;
    
    return keyval;
    
}
