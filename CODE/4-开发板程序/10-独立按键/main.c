
//测试说明：按动S1~S2按键，观察LED显示
#include <msp430x14x.h>

#define keyin    (P1IN & 0x03)


void delay(void);

/********************主函数********************/
void main( void )
{
    unsigned char temp;
    
    WDTCTL = WDTPW + WDTHOLD;   //关闭看门狗
    //P1REN=0XFF;
   
    P1DIR = 0xfc;               //设置P1.0~P.1为输入状态    
    P2DIR = 0xff;
    P2OUT = 0xff;
    while(1)
    {
        if(keyin != 0x03)       //键值有改变
        {
            delay();            //延时消抖
            if(keyin != 0x03)   //再次检测按键状态
            {   
                temp=keyin; 
                
                while(keyin != 0x03);   //等待按键被放开
                switch(temp)    //转换键值    
                {
                case 0x02:
                            P2OUT = 0xfe;break;
                case 0x01:
                            P2OUT = 0xfd;break;
               // case 0x0b:
               //            P2OUT = 0xfb;break;
               // case 0x07:
               //             P2OUT = 0xf7;break;
                default:
                            P2OUT = 0xff;break;
                }
                delay();delay();delay();delay();delay();delay();delay();delay();
            }
        }
    }
}
/*******************************************
函数名称：delay
功    能：用于消抖的延时
参    数：无
返回值  ：无
********************************************/
void delay(void)
{
    unsigned int tmp;
    
    for(tmp = 12000;tmp > 0;tmp--);
}
