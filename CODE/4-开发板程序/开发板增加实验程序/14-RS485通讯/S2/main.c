/*************************************************
程序功能：通过RS485端口接收字符并在1602液晶上显示
--------------------------------------------------
测试方法：用另一个RS485发射器，通过RS485总线向学习板
          发送数据（必须是可显示字符对应的ASCII码），
          观察液晶显示
**************************************************/
#include  <msp430x14x.h>
#include  "cry1602.h"

#define DRE_out     P5DIR |= BIT3     //连接485芯片的DE，RE端口的IO设置为输出状态
#define DE          P5OUT |= BIT3     //设置485芯片处于发送状态 
#define RE          P5OUT &= ~BIT3    //设置485芯片处于接收状态

unsigned char pX = 0,pY = 0;

void Set_UART1(void);

/****************主函数****************/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 //关闭看门狗   
    DRE_out;
    RE;
    Set_UART1();  
    LcdReset();             //初始化液晶
   
}
/*******************************************
函数名称：UART0_RX
功    能：UART0的中断服务函数
参    数：无
返回值  ：无
********************************************/
#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX (void)
{
    Disp1Char(pX++,pY,RXBUF0);
    if(pX == 16)
    {
      pX = 0;
      pY ^= 1;
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
    IE2 |= URXIE1;                            //使能UART1接收中断
}
