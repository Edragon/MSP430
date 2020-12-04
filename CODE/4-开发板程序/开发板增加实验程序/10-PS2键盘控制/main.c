/*****************************************************
程序功能：接收并解码来自标准键盘的基本按键的扫描码
解出的键码通过LED显示。
*****************************************************/
#include  <msp430x14x.h>
#include "Keyboard.h"
#include "gdata.h"
#define  uchar unsigned char
#define  uint  unsigned int
uchar flag=1,flagbeep;
#define SIDval  P5IN & BIT1//SIDval  P5IN & BIT6
 void  PORT(void);
 
 void beep()
 {
   uint i;
   for(i=0;i<40000;i++)
   P5OUT=0xfe;
   for(i=0;i<40000;i++)
   P5OUT=0xff;   
 }
/****************主函数****************/
void main(void)
{
    uchar disptmp,i;
    
    WDTCTL = WDTPW + WDTHOLD;   //关闭看门狗
    
    /*------选择系统主时钟为8MHz-------*/
    BCSCTL1 &= ~XT2OFF;                 // 打开XT2高频晶体振荡器
    do
    {
        IFG1 &= ~OFIFG;                 // 清除晶振失败标志
        for (i = 0xFF; i > 0; i--);     // 等待8MHz晶体起振
    }
        while ((IFG1 & OFIFG));             // 晶振失效标志仍然存在？
    BCSCTL2 |= SELM_2 + SELS;           // MCLK和SMCLK选择高频晶振
    
    TACCTL0 |= CCIE;                    //使能比较中断
    TACTL |= TASSEL_2 + ID_3 ;          //计数时钟选择SMLK=8MHz，1/8分频后为1MHz

    P2DIR = 0xff;
    P2OUT = 0xff;
    
    Init_KB();                  //初始化键盘端口
    //_EINT();                    //打开全局中断
  
    while(1)
    {
      while(flag)
      {
        PORT();
      }
        disptmp = GetChar();    //读取键值对应的ASCII码
        
        if((disptmp > 0x2f)&&(disptmp < 0x3a)) //如果接收到的字符是0~9
        {
            switch(disptmp) 
            {
              case '0':
                        P2OUT = ~0x55;
                        break;
              case '1':
                        P2OUT = ~0x1;
                        break;
              case '2':
                        P2OUT = ~0x2;
                        break;
              case '3':
                        P2OUT = ~0x4;
                        break;
              case '4':
                        P2OUT = ~0x8;
                        break;
              case '5':
                        P2OUT = ~0x10;
                        break;
              case '6':
                        P2OUT = ~0x20;
                        break;
              case '7':
                        P2OUT = ~0x40;
                        break;
              case '8':
                        P2OUT = 0x7f;  
                        break;
              case '9':
                        P2OUT = 0x7e;
                        break;
              default:
                        _NOP();
              }
          }
        else if(disptmp == 'b')
        {flagbeep=1;
            P2OUT = 0xff;
        }
        else if(disptmp == 's')
        {flagbeep=0;
            P5OUT = BIT0;
            P2OUT = 0x0;
        }
        flag=1;
    }
} 
/*******************************************
函数名称：Timer_A
功    能：定时器A的中断服务函数，在这里驱动
          蜂鸣器发声
参    数：无
返回值  ：无
********************************************/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  P5OUT ^= BIT0;                            // Toggle P6.7
//flag=1;
}

/*******************************************
函数名称：PORT1_ISR 
功    能：P1端口的中断服务函数，在这里接收来
          自键盘的字符
参    数：无
返回值  ：无
********************************************/
 void  PORT(void)
{
    if(!(P5IN & BIT2))            //如果是clock的中断
    {
     // flag=1;//  P1IFG &=~ BIT7;         //清除中断标志 
        
        if(bitcount == 11)        //接收第1位
        {
            if(SIDval)          //如果不是起始位
                return;
            else  
                bitcount--;
        } 
        else if(bitcount == 2)    //接收奇偶校验位
        {   
            if(SIDval)          //如果校验位等于1
                pebit = 1;
            else
                pebit = 0;
            bitcount--;
        }
        else if(bitcount == 1)    //接收停止位
        {
            if(SIDval)          //若停止位正确
            {
                bitcount = 11;    //复位位计数变量
                if( Decode(recdata) )    //解码获得此键值的ASCII值并保存
                   // LPM3_EXIT;           //退出低功耗模式
                recdata = 0;          //清除接收数据
                flag=0;
            }
            else                //如果出错
            {
                bitcount = 11;
                recdata = 0;   
                flag=1;
            }
        }
        else                    //接收8个数据位
        {
            recdata >>= 1;
            if(SIDval)  recdata |= 0x80;
            bitcount--;
        }
   }
  /// else
   //  flag=0;
}




