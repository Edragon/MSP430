/******************************************************************************
	         中景园电子   MSP430F5438A 

功能：测试MSP430F5438A- 启用外部晶振，配置时钟系统

//                MSP430F5438A
//             -----------------

//            |            P11.0|--> ACLK  = 32.768kHz Crystal Output
//            |                 |
//            |            P11.2|--> SMCLK = 24MHz/4 Crystal Output
//            |                 |
//            |            P11.1|--> MCLK  = 24MHz/2 Crystal Output

说明：启用XT1--外部32.768Khz，XT2--外部24Mhz

测试工具：逻辑分析仪

中景园电子
淘宝连接：https://shop73023976.taobao.com
******************************************************************************/

#include <msp430F5438A.h>  

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long int

#define CPU_F ((double)24000000UL)            //XT2 --> 24MHZ
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) //延时1us
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))    //延时1ms

void Set_Vcore(uint level);
void Init_Clock();

void main(void)                             //主函数 
{ 
  WDTCTL = WDTPW + WDTHOLD;                 //关闭看门狗 
  Init_Clock();                             //初始化系统时钟
  
  UCSCTL5 |= DIVS__4 + DIVM__2;             //SMCLK 4分频、MCLK 2分频
  
  P1DIR |= BIT3 + BIT4 + BIT5 + BIT6;       //LED IO设置为输出
  P1OUT &= ~(BIT3 + BIT4 + BIT5 + BIT6);    //LED灭
   
  P11DIR = BIT2 + BIT1 + BIT0;              // P11.2,1,0 to output direction
  P11SEL = BIT2 + BIT1 + BIT0;              // P11.2 to output SMCLK, P11.1 to output MCLK and P11.0 to output ACLK
  
  while(1)
  {
      P1OUT |= (BIT3 + BIT4 + BIT5 + BIT6);   //LED亮  
      Delay_ms(500);
      P1OUT &= ~(BIT3 + BIT4 + BIT5 + BIT6);  //LED灭
      Delay_ms(500);
  }
} 

/******************************************************************************
功能：初始化时钟
详细：使用外部晶振24M作为：MCLK、SMLK
      使用外部晶振32768HZ作为：ACLK
******************************************************************************/
void Init_Clock()
{
  uchar i;
  
  P5SEL |= BIT2 + BIT3;                   //P5.2和P5.3选择为晶振XT2输入  
  P7SEL |= BIT0 + BIT1;                   //P7.0和P7.1选择为晶振XT1输入  
  Set_Vcore(PMMCOREV_3);                  // Set frequency up to 25MHz
  UCSCTL6 &= ~(XT1OFF + XT2OFF);          // Set XT1 & XT2 On 
  UCSCTL6 |= XCAP_3;                      // Internal load XT1 cap 12pF，MSP430F5438A V4.0最小系统XT1未接外部晶振  
  do                                      // Loop until XT1,XT2 & DCO stabilizes
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
    SFRIFG1 &= ~OFIFG;                    // 清除振荡器失效标志
    for (i = 0xFF; i > 0; i--);           // 延时，等待XT2起振
  } while (SFRIFG1 & OFIFG);              // 判断XT2是否起振
  Delay_ms(50);
  UCSCTL4 |= SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK;    //选择MCLK、SMCLK为XT2,  
}


/******************************************************************************
功能：设置内核电压
详细：Power Management Module (PMM).The PMM manages all functions related to the power supply and its supervision for the device. Its primary
functions are first to generate a supply voltage for the core logic, and second, provide several
mechanisms for the supervision and monitoring of both the voltage applied to the device (DVCC) and thevoltage generated for the core (VCORE).
The PMM uses an integrated low-dropout voltage regulator (LDO) to produce a secondary core voltage(VCORE) from the primary one applied to the device (DVCC).
In general, VCOREsupplies the CPU, memories(flash and RAM), and the digital modules, while DVCCsupplies the I/Os and all analog modules (includingthe 
oscillators). The VCOREoutput is maintained using a dedicated voltage reference. VCORE is programmable up to four steps, 
to provide only as much power as is needed for the speed that has been selected for the CPU. 
This enhances power efficiency of the system. The input or primary side of the regulator is referred to in this chapter as its high side. 
The output or secondary side is referred to in this chapter as its low side.
******************************************************************************/
void Set_Vcore(uint level)
{  
  PMMCTL0_H = PMMPW_H;                    // Open PMM registers for write  
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;  // Set SVS/SVM high side new level  
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;   // Set SVM low side to new level  
  while ((PMMIFG & SVSMLDLYIFG) == 0);    // Wait till SVM is settled  
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);      // Clear already set flags  
  PMMCTL0_L = PMMCOREV0 * level;          // Set VCore to new level  
  if ((PMMIFG & SVMLIFG))                 // Wait till new level reached
  while ((PMMIFG & SVMLVLRIFG) == 0);
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;  // Set SVS/SVM low side to new level  
  PMMCTL0_H = 0x00;                       // Lock PMM registers for write access
}