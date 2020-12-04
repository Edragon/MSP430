/******************************************************************************
	            MSP430F5438A 
功能：测试MSP430F5438A- PWM

//                MSP430F5438A

//            -------------------

//        /|\|                   |

//         | |                   |

//         --|RST                |

//           |                   |

//           |         P2.2/TA1.1|--> CCR1 - 75% PWM

//           |         P2.3/TA1.2|--> CCR2 - 25% PWM

中景园电子
淘宝连接：https://shop73023976.taobao.com
******************************************************************************/

#include <msp430F5438A.h>  

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long int

#define CPU_F ((double)24000000UL)                              //XT2 --> 24MHZ
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) //延时1us
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))    //延时1ms

uchar num = 0;

void Set_Vcore(uint level);
void Init_Clock();

void main(void)                             //主函数 
{ 
  WDTCTL = WDTPW + WDTHOLD;                 //关闭看门狗 
  Init_Clock();                             //初始化系统时钟
  
  P2DIR |= 0x0C;                            // P2.2 and P2.3 output
  P2SEL |= 0x0C;                            // P2.2 and P2.3 options select
  TA1CCR0 = 512-1;                          // PWM Period
  TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
  TA1CCR1 = 384;                            // CCR1 PWM duty cycle
  TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
  TA1CCR2 = 128;                            // CCR2 PWM duty cycle
  TA1CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR
  
  _EINT();
  
  while(1);
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