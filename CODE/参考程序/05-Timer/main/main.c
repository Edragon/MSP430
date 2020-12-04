/******************************************************************************
	            MSP430F5438A 
�о�԰����
�Ա����ӣ�https://shop73023976.taobao.com

���ܣ�����MSP430F5438A - Timer
      ����500ms��תһ��LED״̬

˵��������XT1--�ⲿ32.768Khz��XT2--�ⲿ24Mhz
      ��24M����24��Ƶ=1Mhz

���Թ��ߣ��߼�������
�о�԰����
�Ա����ӣ�https://shop73023976.taobao.com
******************************************************************************/

#include <msp430F5438A.h>  

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long int

#define CPU_F ((double)24000000UL)            //XT2 --> 24MHZ
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) //��ʱ1us
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))    //��ʱ1ms

uchar num = 0;

void Set_Vcore(uint level);
void Init_Clock();

void main(void)                             //������ 
{ 
  WDTCTL = WDTPW + WDTHOLD;                 //�رտ��Ź� 
  Init_Clock();                             //��ʼ��ϵͳʱ��
  
  P1DIR |= BIT3;                            // P1.0 output
  TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TA1CCR0 = 50000;
  TA1CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, upmode, clear TAR
  TA1CTL |= ID_3;                           // ��SMCK����8��Ƶ��Ȼ����ΪTACLKʹ��
  TA1EX0 |= TAIDEX_2;                       // ��SMCK8��Ƶ���ٽ���3��Ƶ��Ȼ����ΪTACLKʹ�ã��ܹ�Ϊ24��Ƶ�õ�1Mhz
  
  _EINT();
  
  while(1);
} 

// Timer A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  num++;
  if(num == 10)
  {
    P1OUT ^= BIT3;                            // Toggle P1.3
    num = 0;
  }  
}


/******************************************************************************
���ܣ���ʼ��ʱ��
��ϸ��ʹ���ⲿ����24M��Ϊ��MCLK��SMLK
      ʹ���ⲿ����32768HZ��Ϊ��ACLK
******************************************************************************/
void Init_Clock()
{
  uchar i;
  
  P5SEL |= BIT2 + BIT3;                   //P5.2��P5.3ѡ��Ϊ����XT2����  
  P7SEL |= BIT0 + BIT1;                   //P7.0��P7.1ѡ��Ϊ����XT1����  
  Set_Vcore(PMMCOREV_3);                  // Set frequency up to 25MHz
  UCSCTL6 &= ~(XT1OFF + XT2OFF);          // Set XT1 & XT2 On 
  UCSCTL6 |= XCAP_3;                      // Internal load XT1 cap 12pF��MSP430F5438A V4.0��СϵͳXT1δ���ⲿ����  
  do                                      // Loop until XT1,XT2 & DCO stabilizes
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
    SFRIFG1 &= ~OFIFG;                    // �������ʧЧ��־
    for (i = 0xFF; i > 0; i--);           // ��ʱ���ȴ�XT2����
  } while (SFRIFG1 & OFIFG);              // �ж�XT2�Ƿ�����
  Delay_ms(50);
  UCSCTL4 |= SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK;    //ѡ��MCLK��SMCLKΪXT2,  
}


/******************************************************************************
���ܣ������ں˵�ѹ
��ϸ��Power Management Module (PMM).The PMM manages all functions related to the power supply and its supervision for the device. Its primary
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