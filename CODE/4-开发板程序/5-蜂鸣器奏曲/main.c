/*********************************************
程序功能：MCU控制蜂鸣器演奏歌曲《祝你平安》
*********************************************/
#include  <msp430x14x.h>
#include  "music.h"
#define   uchar  unsigned char
#define   Buzzer        BIT7
#define   Buzzer_Port   P6OUT
#define   Buzzer_DIR    P6DIR

uchar counter;
void Play_Song(void);

/***************主函数****************/
void main(void)
{
    uchar i;
     
    WDTCTL = WDTPW + WDTHOLD;                 //关闭看门狗
    /*------选择系统主时钟为8MHz-------*/
    BCSCTL1 &= ~XT2OFF;                       // 打开XT2高频晶体振荡器
    do
    {
        IFG1 &= ~OFIFG;                       //清除晶振失败标志
        for (i = 0xFF; i > 0; i--);           // 等待8MHz晶体起振
    }
    while ((IFG1 & OFIFG));                   // 晶振失效标志仍然存在？
    BCSCTL2 |= SELM_2 + SELS;                 //主时钟和从时钟都选择高频晶振

    
    //设置定时器A每10ms中断一次
    CCTL0 = CCIE;
    CCR0 = 10000;
    TACTL |= TASSEL_2 + ID_3; 
    //设置控制蜂鸣器的IO方向为输出
    Buzzer_DIR |= Buzzer;
    //打开全局中断
    _EINT();
    //循环演奏歌曲
    while(1)
    {
        Play_Song();
    }
}

/*******************************************
函数名称：TimerA_ISR
功    能：定时器A的中断服务函数
参    数：无
返回值  ：无
********************************************/
#pragma vector = TIMERA0_VECTOR
__interrupt void TimerA_ISR(void)
{
    counter++;
}
/*******************************************
函数名称：Delay_Nms
功    能：延时N个ms的函数
参    数：n--延时长度
返回值  ：无
********************************************/
void Delay_Nms(uchar n)
{
    uchar i,j;
    
    for( i = 0;i < n; i++ )
    {
        for( j = 0;j < 3;j++ )
          _NOP();
    }
}
/*******************************************
函数名称：Play_Song
功    能：播放《祝你平安》的乐曲
参    数：无
返回值  ：无
********************************************/
void Play_Song(void)
{
    uchar Temp1,Temp2;
    uchar addr = 0;
    
    counter = 0; //中断计数器清0
    while(1)
    {
        Temp1 = SONG[addr++];
        if ( Temp1 == 0xFF )        //休止符
        {
            TACTL &=~MC_1;          //停止计数
            Delay_Nms(100);
        }
        else if ( Temp1 == 0x00 )   //歌曲结束符
        {
            return;
        }
        else
        {
            Temp2 = SONG[addr++];
            TACTL |=MC_1;           //开始计数
            while(1)
            {
                Buzzer_Port ^= Buzzer;
                Delay_Nms(Temp1);
                if ( Temp2 == counter )
                {
                    counter = 0;
                    break;
                }
            }
        }
    }
}
