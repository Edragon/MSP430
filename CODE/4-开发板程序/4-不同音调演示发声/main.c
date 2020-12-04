/*********************************************************
程序功能：用固定频率的方波驱动蜂鸣器，共16种音调；在蜂鸣器
          发出不同音调的同时，LED发光以二进制数字形式指示
          当前音调的编号（1~16）
----------------------------------------------------------
测试说明：聆听蜂鸣器发声的音调变化。
**********************************************************/
#include <msp430x14x.h>
#define uchar unsigned char
uchar step = 0xff;

/************************主函数************************/
void main( void )
{
    uchar i;
    
    WDTCTL = WDTPW + WDTHOLD;           //关狗
   // BoardConfig(0xb0);                  //关数码管、流水灯和电平转换
    
    /*------选择系统主时钟为8MHz-------*/
    BCSCTL1 &= ~XT2OFF;                 //打开XT2高频晶体振荡器
    do
    {
        IFG1 &= ~OFIFG;                 //清除晶振失败标志
        for (i = 0xFF; i > 0; i--);     //等待8MHz晶体起振
    }
    while ((IFG1 & OFIFG));             //晶振失效标志仍然存在？
    BCSCTL2 |= SELM_2 + SELS;           //MCLK和SMCLK选择高频晶振
    
    TACCTL0 |= CCIE;                    //使能比较中断
    TACTL |= TASSEL_2 + ID_3 ;          //计数时钟选择SMLK=8MHz，1/8分频后为1MHz
    
    TBCCR0 = 4096*2 - 1;                //周期两秒
    TBCCTL0 |= CCIE;
    TBCTL |= TBSSEL_1 + ID_3 + MC_1;    //时钟源ACLK/8，up mode
    
    P6DIR |= BIT7;                      //蜂鸣器对应IO设置为输出
    P2DIR = 0xff;
    P2OUT = 0xff;
    
    _EINT();
    
    LPM1;
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
  P6OUT ^= BIT7;                            // Toggle P67
}
/*******************************************
函数名称：Timer_B
功    能：定时器B的中断服务函数，在这里更改
          蜂鸣器发声频率
参    数：无
返回值  ：无
********************************************/
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
    if(step == 0xff)                        //up mode
      TACTL |= MC_1;   
    step++;
    switch(step)
    {  
    case    0:  TACCR0 = 5000;  P2OUT = ~1;   break;      // 100Hz
    case    1:  TACCR0 = 2500;  P2OUT = ~2;   break;      // 200Hz
    case    2:  TACCR0 = 1250;  P2OUT = ~3;   break;      // 400Hz
    case    3:  TACCR0 = 625;   P2OUT = ~4;   break;      // 800Hz
    case    4:  TACCR0 = 500;   P2OUT = ~5;   break;      // 1KHz
    case    5:  TACCR0 = 250;   P2OUT = ~6;   break;      // 2KHz
    case    6:  TACCR0 = 167;   P2OUT = ~7;   break;      // 3KHz
    case    7:  TACCR0 = 125;   P2OUT = ~8;   break;      // 4KHz
    case    8:  TACCR0 = 100;   P2OUT = ~9;   break;      // 5KHz
    case    9:  TACCR0 = 83;    P2OUT = ~10;   break;      // 6KHz
    case    10:  TACCR0 = 71;   P2OUT = ~11;   break;      // 7KHz
    case    11:  TACCR0 = 63;   P2OUT = ~12;   break;      // 8KHz
    case    12:  TACCR0 = 56;   P2OUT = ~13;   break;      // 9KHz
    case    13:  TACCR0 = 50;   P2OUT = ~14;   break;      // 10KHz
    case    14:  TACCR0 = 33;   P2OUT = ~15;   break;      // 15KHz
    case    15:  TACCR0 = 25;   P2OUT = ~16;   break;      // 20KHz
    case    16:  step = 0xff;               // 循环播放
    }
}
