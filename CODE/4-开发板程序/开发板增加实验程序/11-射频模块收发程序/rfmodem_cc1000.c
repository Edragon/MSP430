/****************************************************************************/
/* Application note AN015                                                   */
/* Reference design : CC1000 RF Modem                                       */
/*                                                                          */
/* File:      rfmodem_cc1000.c                                              */
/* Revision:  1.0                                                           */
/*                                                                          */
/* Microcontroller:                                                         */
/*          Microchip PIC16F876                                             */
/*                                                                          */
/* Author:  Karl H. Torvmark, Field Applications Engineer, Chipcon          */
/*                                                                          */
/* Contact: Chipcon AS +47 22 95 85 44                                      */
/*          wireless@chipcon.com                                            */
/****************************************************************************/


/****************************************************************************/
/* Description:                                                             */
/*                                                                          */
/* This application note presents a software and hardware reference design  */
/* for the CC1000. This software can serve as a starting point for          */
/* developing custom software. A full protocol is implemented, with         */
/* provisions for addressing, error checking etc.                           */
/*                                                                          */
/* This software contains routines for detecting the preamble, searching    */
/* for the start-of-frame (SOF) marker and doing byte synchronisation,      */
/* and reading and handling header data such as the packet length.          */
/*                                                                          */
/* The software also contains all necessary routines to configure the       */
/* CC1000, and demonstrates how to read and write data to the data          */
/* interface. Configuration data is stored in non-volatile EEPROM memory,   */
/* so that changes are preserved even when power is removed. Easy           */
/* modification of parameters such as RF frequency and data rate is         */
/* supported through a configuration menu.                                  */
/*                                                                          */
/* Please see the main text of application note AN015 for a more detailed   */
/* description.                                                             */
/****************************************************************************/

/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: rfmodem_cc1000.c,v $
 * Revision 2.5  2003/05/07 15:02:31  tos
 * Modified LOCK monitor in RX/TX setup: wait for LOCK before turning PA on.
 *
 * Revision 2.4  2003/04/25 13:30:06  tos
 * Removed UART baudrate test (baudrate=2.4 kBaud).
 *
 * Revision 2.3  2003/04/25 13:12:47  tos
 * Corrected inconsistent [UART interrupt request flag] monitor.
 *
 * Revision 2.2  2003/04/24 12:44:59  tos
 * Corrected inconsistent monitoring of CC1000: [calibration complete] + [lock].
 *
 * Revision 2.1  2003/03/04 10:43:18  tos
 * Corrected inconsistent AND instruction in CalibrateCC1000().
 *
 *                                                                           *
 ****************************************************************************/

#include <msp430x14x.h>
#include "BoardConfig.h"
#include "CC1000.h"
#include "IO_Define.h"
#include "configure.h"
#include "simpleio.h"
#include "mstdio.h"

// #define INTERRUPT_VECTOR 0x04

#define HEADER_SIZE 4           // 4 bytes header 

// The wrap-around functionality has been optimised by ANDing with a bit mask. 
// Please note that if RX_BUFFER_SIZE is to be changed, the lines of code which
// do this must also be changed. They are located in the main loop of the program
// and in 

#define TX_BUFFER_SIZE 64       // Size (in bytes) of transmit buffer
#define RX_BUFFER_SIZE 64       // Size (in bytes) of receive ring-buffer

#define PREAMBLE_LENGTH 10       // Number of bytes of preamble to send */
#define PREAMBLE_REQ 6          /* Number of bits required in addition to */
                                /* the initial 8 bits for the preamble to be */
                                /* accepted */

#define UI1     0x33		/* First byte of unique identifier */
#define UI2     0xCC		/* Second byte of unique identifier */

#define FALSE   0
#define TRUE    1

#define ON      TRUE
#define OFF     FALSE


// Macros for turning on and off the LEDs

#define SET_RXLED(STATE) \
  if (STATE)    P2OUT &= ~BIT0;  \
  else          P2OUT |= BIT0        

#define SET_TXLED(STATE) \
  if (STATE)    P2OUT &= ~BIT7;  \
  else          P2OUT |= BIT7        
    
//定义键盘按键
#define BUTTON1 (P1IN & BIT0)
#define BUTTON2 (P1IN & BIT1)
#define BUTTON3 (P1IN & BIT2)
#define BUTTON4 (P1IN & BIT3)

// Variables

// Unit address is not used
extern  unsigned char UnitAddress;

unsigned char ShiftReg;

// Buffers for transmitted and received data
// These are put into different banks so that they can be as large as possible
// The TX buffer is filled up with data to be sent in the next data packet
// The RX buffer is a ring buffer in which received data is stored waiting to be
// sent to the UART
                                
volatile  unsigned char TXBuffer[TX_BUFFER_SIZE]; 
volatile  unsigned char RXBuffer[RX_BUFFER_SIZE];

// Index pointers for use with buffers 
 unsigned char TXBufferIndex;            
 unsigned char RXBufferReadIndex;
 unsigned char RXBufferWriteIndex;

// Counter variables
unsigned char PreambleEnd;
unsigned char PreambleNextbit;
//unsigned char PreambleCount;
unsigned char ByteCounter;
unsigned char BitCounter;

// Contains the total number of bytes to send in TX, including preamble and header
unsigned char BytesToSend;
// The number of bytes of data to receive in RX
unsigned char BytesToReceive;


// State variable stores the current state of the state machine
enum StateType {IDLE_STATE=0,RX_STATE=1,TX_STATE=2} State;

// This variable stores the state to switch to
// It is updated by the interrupt routine, while the main program
// does the actual state switch
volatile enum StateType NextState;

unsigned char LockAverage;
unsigned char UnlockAverage;

// This routine initialises the TX buffer at startup
void InitializeTXBuffer(void)
{
  unsigned char i;
  
  // Put preamble into buffer
  for (i=0;i<PREAMBLE_LENGTH;i++)
  {
    TXBuffer[i]=0xAA;
  }
 
  TXBuffer[PREAMBLE_LENGTH]=UI1;              // First byte of unique identifier
  TXBuffer[PREAMBLE_LENGTH+1]=UI2;            // Second byte of unique identifier
  TXBuffer[PREAMBLE_LENGTH+2]=UnitAddress;    // Unit address
  TXBuffer[PREAMBLE_LENGTH+3]=0x00;           // Default : no data  
}




// This routine handles setup needed when changing states
void ChangeState(void) 
{
  switch (NextState) 
  {
    case RX_STATE:
      if(State == TX_STATE) 
      {
       DCLK_INT_UP;			// OPTION=0xC0;        /* INT on rising edge */
       DIO_IN;				//  TRISC|=0x02;        /* Set DIO as input */
       SetupCC1000RX(RXCurrent,RXPLL);
      }
    
      State=RX_STATE;
      
      SET_RXLED(ON); 
      SET_TXLED(OFF);
   
      PreambleEnd = 0;
      PreambleNextbit = 1;
      BitCounter=0;
      ByteCounter=0;
      break;
    case TX_STATE:
      if(State != TX_STATE) 
      {
        DCLK_INT_DN;		// OPTION=0x80;        /* INT on falling edge */
        DIO_OUT;					// TRISC&=~(0x02);     /* Set DIO as output */
        SetupCC1000TX(TXCurrent,TXPLL);
      }

      State=TX_STATE;

      SET_RXLED(OFF);
      SET_TXLED(ON);
     // READY=1;  // HW Handshake : Not Ready
    // writeln("S2T");
      ME1 &= ~URXE0;			 //RCIE=0;   // Disable UART Interrupts  禁止UART接收
        
      BytesToSend=TXBufferIndex; // Number of bytes to send
      TXBuffer[PREAMBLE_LENGTH+3]=BytesToSend-HEADER_SIZE-PREAMBLE_LENGTH;

      TXBufferIndex=0;
      BitCounter=0;
      ShiftReg=TXBuffer[TXBufferIndex++];
      break;
    case IDLE_STATE:
      if(State==TX_STATE) 
      {
        DCLK_INT_UP;				// OPTION=0xC0;        /* INT on rising edge */
        DIO_IN;							// TRISC|=0x02;        /* Set DIO as input */
        SetupCC1000RX(RXCurrent,RXPLL);
      }

      State=IDLE_STATE;

      SET_RXLED(OFF);
      SET_TXLED(OFF);
      
      ME1 |= URXE0;		 // RCIE=1;   // Enable UART Interrupts  使能UART接收
      
      TXBufferIndex=HEADER_SIZE+PREAMBLE_LENGTH;
      PreambleNextbit = 1;
      BitCounter = 0;
      UnlockAverage=1;
      break;
  }
}

// Main program
void main(void)
{
    uchar i;
    uchar dummy;   
    
    WDTCTL = WDTPW + WDTHOLD;		//关闭看门狗
    BoardConfig(0xb0);                  //打开流水灯
    
    BCSCTL1 &= ~XT2OFF;                       // XT2on 
    do
    {
    IFG1 &= ~OFIFG;                           // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);               // Time for flag to set
    }
    while ((IFG1 & OFIFG));                   // OSCFault flag still set?
    BCSCTL2 |= SELM_2 + SELS;                // MCLK = SMCLK = XT2 (safe)
    
    //初始化CC1000控制端口
    Ctrl_DIR |= PDATA + PALE + PCLK;	
    Ctrl_Port |= PDATA + PALE + PCLK;
    DIO_IN;					//DIO设置为输入方向
    //打开DCLK端口中断
    P1DIR = 0x80;		//P1端口设置为输入，用于按键检测,P1.7输出
    P1OUT = 0x00;
    DCLK_INT_EN;
    DCLK_INT_UP;
    //设置P2口为输出，用于指示灯
    P2OUT = 0xff;
    P2DIR = 0xff;
    //设置UART端口
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL1;                          // UCLK = SMCLK
    UBR00 = 0xA0;                             // 8Mhz/19200 ~ 417
    UBR10 = 0x01;                             //
    UMCTL0 = 0x00;                            // no modulation                           // 8MHz 115200 modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    IE1 &= ~(URXIE0 + UTXIE0);                // Disable USART0 RX and TX interrupt


  //定时器A用来定时检测按键是否被按下
    TACTL =  TASSEL_1 + MC_1; 			//选择327868Hz时钟源
    CCR0  = 3277;				//设定100ms中断一次
  //定时器B用来在收到UART数据以后定时将状态切换到发送模式
    TBCTL = TBSSEL_1;				// 选择32768Hz时钟源
    TBCCR0 = 328;				// 10ms中断一次
    
 //   PALE_1;					//PALE=1;
  
    RXBufferReadIndex=0;
    RXBufferWriteIndex=0;
  
    SetupCC1000PD();
    ResetCC1000();
  
    SetupCC1000All();
 
    WakeUpCC1000ToTX(TXCurrent,TXPLL);
    SetupCC1000TX(TXCurrent,TXPLL);
    if (!CalibrateCC1000())
    {
        writeln("TX Calibration failed");
    }
    // Calibration data is stored in the chip indexed by the selected frequency register 
    SetupCC1000RX(RXCurrent,RXPLL);						// TRISC|=0x02;        // Set DIO as input
    if (!CalibrateCC1000())
    {
        writeln("RX Calibration failed");
    }
 	
  // Now the CC1000 is calibrated for both RX and TX, we do not need to recalibrate
  // unless the frequency is changed, the temperature changes by 40 degrees C
  // or if the supply voltage changes by more than 0.5V 
  
    // Force update 强制发生一次状态转换
    State=TX_STATE;
    NextState=IDLE_STATE;
  
    InitializeTXBuffer();
    if(BUTTON1==0) 
    {
        _NOP();
    }
    if(BUTTON2==0) 
    {
        _NOP();
    }
    if(BUTTON3==0) 
    {
        _NOP();
    }
    if(BUTTON4==0)		//调用配置菜单
    {           
        ConfigurationMode();
    }
  //补写PA_POW寄存器，保证以正确的功率输出
  WriteToCC1000Register(CC1000_PA_POW,PA_Power);
 
  LockAverage=0;
  UnlockAverage=0;
  
  /* Startup message */ 
  writeln("RF Modem ready");
  writestr("Compiled ");
  writestr(__DATE__);
  writestr(" ");
  writestr(__TIME__);
  writeln("");
  
  dummy=ReadFromCC1000Register(CC1000_MODEM0); 
  switch(dummy) 
  {
    case 0x07:
      writestr("0.6kbit/s Manchester");
      break;
    case 0x03:
      writestr("0.6kbit/s NRZ");
      break;
    case 0x17:
      writestr("1.2kbit/s Manchester");
      break;
    case 0x13:
      writestr("1.2kbit/s NRZ");
      break;
    case 0x27:
      writestr("2.4kbit/s Manchester");
      break;
    case 0x23:
      writestr("2.4kbit/s NRZ");
      break;
    case 0x37:
      writestr("4.8kbit/s Manchester");
      break;
    case 0x33:
      writestr("4.8kbit/s NRZ");
      break;
    case 0x47:
      writestr("9.6kbit/s Manchester");
      break;
    case 0x43:
      writestr("9.6kbit/s NRZ");
      break;
    case 0x57:
      writestr("19.2kbit/s Manchester");
      break;
    case 0x53:
      writestr("19.2kbit/s NRZ");
      break;
    case 0x55:
      writestr("38.4kbit/s Manchester");
      break;
    case 0x51:
      writestr("38.4kbit/s NRZ");
      break;
    case 0x50:
      writestr("76.8kbit/s NRZ");
      break;
    default:
      writestr("Invalid data rate");
      break;
  } 
  writeln("");
  dummy=ReadFromCC1000Register(CC1000_PA_POW);
  switch(dummy) 
  {
    case 0x01:
      writestr("-20/-19 dBm");
      break;
    case 0x02:
      writestr("-18/-17/-16 dBm");
      break;
    case 0x03:
      writestr("-15/-14/-13 dBm");
      break;
    case 0x04:
      writestr("-12/-11 dBm");
      break;
    case 0x05:
      writestr("-10/-9/-8 dBm");
      break;
    case 0x06:
      writestr("-8 dBm");
      break;
    case 0x07:
      writestr("-7 dBm");
      break;
    case 0x08:
      writestr("-6 dBm");
      break;
    case 0x09:
      writestr("-5 dBm");
      break;
    case 0x0A:
      writestr("-4 dBm");
      break;
    case 0x0B:
      writestr("-3 dBm");
      break;
    case 0x0C:
      writestr("-2 dBm");
      break;
    case 0x0E:
      writestr("-1 dBm");
      break;
    case 0x0F:
      writestr(" 0 dBm");
      break;
    case 0x40:
      writestr(" 1 dBm");
      break;
    case 0x50:
      writestr(" 2/3 dBm");
      break;
    case 0x60:
      writestr(" 4 dBm");
      break;
    case 0x70:
      writestr(" 5 dBm");
      break;
    case 0x80:
      writestr(" 6 dBm");
      break;
    case 0x90:
      writestr(" 7 dBm");
      break;
    case 0xc0:
      writestr(" 8 dBm");
      break;
    case 0xe0:
      writestr(" 9 dBm");
      break;
    case 0xff:
      writestr("10 dBm");
      break;
    default:
      writestr("Not typical setting!");
      break;
  } 
  writeln("");
  //蜂鸣器鸣叫一声，提示开始运行
  P6DIR |= BIT7;
  P6OUT &= ~BIT7;
  for(i = 255;i > 0;i--)
  {
        dummy = 255;
        while(dummy > 0)dummy--;
  }
  P6OUT |= BIT7;

 // DumpCC1000Regs();

  AverageFreeRunCC1000();  
  
  _EINT(); 			//INTCON=0x90; 
  
  while(1)
  {    
    /* If state change, handle it */
    if (State!=NextState)
    {
     // _DINT();    
      ChangeState();  
     // _EINT();
    }      
      
    if (UnlockAverage) 
    {
      UnlockAverage=0;
      AverageFreeRunCC1000();
    }
    if (LockAverage) 
    {
      LockAverage=0;
      AverageManualLockCC1000();
    }
      
    /* If data in receive buffer, write it to the serial port */
    if (RXBufferReadIndex!=RXBufferWriteIndex)
    {
      putchar(RXBuffer[RXBufferReadIndex++]);  
      RXBufferReadIndex &= 0x3F;
    }
    
    if (IFG1 & URXIFG0) 
    {
    // UART receive interrupt. PC has sent data to us.
        if (State==IDLE_STATE)			// Only handle data if idle
        {  
            TBCTL |= TBCLR; 			// 复位定时器// TMR2=0;   // Reset time-out timer
            TBCTL |= TBSSEL_1 + MC_1; 		// TMR2ON=1; // Start time-out timer
            TBCCR0 = 328;
            
            TXBuffer[TXBufferIndex++]=RXBUF0;
            
            // IMPORTANT : We may have another interrupt or two occur before we can change mode
            // Therefore, leave safety margin!
            if (TXBufferIndex>=(TX_BUFFER_SIZE-3)) // Change mode early to have safety margin
            {  
              NextState=TX_STATE;
            }
        }
        else   		//进行一次哑读操作，消除中断标志
        {
            dummy = RXBUF0;
        }
    }
  
    if (TBCTL & TBIFG) 
    {
        // Timer 2 interrupt. TX timer has timed out.
        TBCTL = 0x00;    
        NextState=TX_STATE;    
    }
  
    if(TACTL & TAIFG)
    {    
        if(BUTTON1==0)
        {
            TXBuffer[TXBufferIndex++]=Button1Char;
            NextState=TX_STATE;
        }
        if(BUTTON2==0) 
        {
            TXBuffer[TXBufferIndex++]=Button2Char;
            NextState=TX_STATE;
        }
        if(BUTTON3==0) 
        {
            TXBuffer[TXBufferIndex++]=Button3Char;
            NextState=TX_STATE;
        }
        if(BUTTON4==0) 
        {
            TXBuffer[TXBufferIndex++]=Button4Char;
            NextState=TX_STATE;
        }
    
        TACTL &= ~TAIFG;				//  TMR1IF=0;
    }   
 }

}
