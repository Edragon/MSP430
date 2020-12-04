/***********************************************
程序功能：通过RS485端口不停地发送数据0xaa
------------------------------------------------
测试说明：用示波器观察RS485端口A、B信号线上的波形
          或者用RS485接收器接收发送的字符。
************************************************/
#include  <msp430x14x.h>

#define DRE_out     P5DIR |= BIT3     //连接485芯片的DE，RE端口的IO设置为输出状态
#define DE          P5OUT |= BIT3     //设置485芯片处于发送状态 
#define RE          P5OUT &= ~BIT3    //设置485芯片处于接收状态

void Set_UART1(void);

/****************主函数****************/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 //关闭看门狗
    
    DRE_out;
    DE;
    Set_UART1();
    
    while (1)
    {
        TXBUF1 = 0xaa; 
        while(!(IFG2 & UTXIFG1));               //等待发送结束 
        IFG2 &= ~UTXIFG1;
    }
}
/*******************************************
函数名称：Set_UART1
功    能：设置MCU的UART1对应的寄存器
参    数：无
返回值  ：无
********************************************/
void Set_UART1(void)
{
    P3SEL |= 0xC0;                            //将P3.6，P3.7选做UART1的通信端口
    ME2 |= UTXE1 + URXE1;                     //使能USART1的TXD和RXD
    UCTL1 |= CHAR;                            //选择8-bit字符
    UTCTL1 |= SSEL0;                          //驱动时钟选择ACLK
    UBR01 = 0x0D;                             //波特率2400
    UBR11 = 0x00;
    UMCTL1 = 0x6B;                            //调整
    UCTL1 &= ~SWRST;                          //初始化UART状态机
}
