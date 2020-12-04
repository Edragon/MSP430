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
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
说明：这个文件里的代码是对Chipcon提供在PIC单片机驱动CC1000的main函数
      移植，原来的文件将变量和宏定义，以及两个函数排列在main()前面让
      人感觉很不舒服，这里对位置进行了调整和重新排列。移植时保持了原
      函数的所有功能，并增加了两个发射控制按键。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*
操作指南：上电后，学习板通过串口向PC发送状态信息，如果用户不进行变更
配置操作，则使用默认设置进行RF通信，默认设置信息如下--
载波频率：433.916400MHz，数据速率：2.4Kbps，数据格式：NRZ，
发射功率：0 dBm，
如果在上电之前，用户按住K4间则将会在PC机屏幕上显示配置菜单，用户可以
根据提示信息对数据速率和发射功率进行配置，并可以查看CC1000内部的寄存
器数值，在完成设置后只要维持学习板电源则将一直保持当前设置，掉电后设
置信息丢失。用户只能在上电是对CC1000进行一次设置。
单片机完成对CC1000的设置和初始化工作以后，会将状态信息发送给PC机，并控
制蜂鸣器鸣响一声，提示用户初始化工作完成。
此时，用户可以按动按键发射对应的字符或者通过串口设置CC1000发送的信息。
如果CC1000收到正确数据帧，会将接收到的数据通过串口显示在PC机屏幕上。
*/
/*--------------------------------------------------------------------
经测试发现：按76.8Kbps的NRZ格式和38.4Kbps的manchester格式发送数据时，
            接收端的错误率极高，基本不能使用。分析原因一方面是MCU的
            处理能力有限（最高时钟8MHz）另一方面就是我们的中断代码是
            用C语言编写的，这个比较占用中断处理时间的！所以如果用户想
            使用上面的两个速率需要换用性能更为强劲的处理器（如MSP430F2XXX）
            或者是将中断处理程序用汇编语言重新编写。
            在其他速率下，基本通信正常，只是偶尔会出现位错误，这是正常
            现象，建议用户在本程序基础上在每帧结尾增加16bit循环冗余校验
            码来进行差错和纠错，这样可以有效解决用位错误的问题。
---------------------------------------------------------------------*/
/*************************************************************************
建议用户继续开发的内容：
            1.CC1000的RSSI引脚已经连接到了MCU的P6.2引脚，用户可以利用MCU
              内置ADC和1602液晶做成一个数字化显示接收信号强度的信号指示器。
            2.利用板上的EEPROM或者MCU内部FLASH做成能够保存当前配置的系统，
              也就是用户对射频系统的配置信息在掉电后保存在EEPROM或FLASH中，
              上电后再次载入以配置CC1000。
            3.在帧结尾增加16CRC，这样就具有了检错和纠错功能，也极大地增强
              了帧传输的可靠性。
            4.开发上层的网络协议，例如使用传输应答、数据加密、授权访问等机
              制来实现更健壮实用的无线通信。
*************************************************************************/
#include <msp430x14x.h>
#include "CC1000.h"
#include "IO_Define.h"
#include "configure.h"
#include "simpleio.h"
#include "mstdio.h"
#include "variable_define.h"
#define  uchar unsigned char 
void InitializeTXBuffer(void);
void ChangeState(void);
//*****************主程序**************/
void main(void)
{
    uchar i,dummy;   
    
    WDTCTL = WDTPW + WDTHOLD;		      // 关闭看门狗
    
    BCSCTL1 &= ~XT2OFF;                       // XT2on 
    do
    {
    IFG1 &= ~OFIFG;                           // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);               // Time for flag to set
    }
    while ((IFG1 & OFIFG));                   // OSCFault flag still set?
    BCSCTL2 |= SELM_2 + SELS;                 // MCLK = SMCLK = XT2 (safe)
    
    //初始化CC1000控制端口
    Ctrl_DIR |= PDATA + PALE + PCLK;	
    Ctrl_Port |= PDATA + PALE + PCLK;
    DIO_IN;				      // DIO设置为输入方向
    //按键端口设置
    P1DIR = 0x80;	                      //P1.0~P1.3端口用于按键输入
    P1OUT = 0x00;
    DCLK_INT_EN;                              //打开DCLK端口中断
    DCLK_INT_UP;                              
    //收发指示灯设置
    P2OUT = 0xff;
    P2DIR = 0xff;
    //UART通信端口设置
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
    
    //初始化UART收发缓存指针
    RXBufferReadIndex=0;
    RXBufferWriteIndex=0;
  
    //设置CC1000进入Power Down模式
    SetupCC1000PD();
    //复位CC1000
    ResetCC1000();
    //设置CC1000内部所有控制寄存器
    SetupCC1000All();
    //唤醒CC1000进入发射模式
    WakeUpCC1000ToTX(TXCurrent,TXPLL);
    //设置发射模式参数
    SetupCC1000TX(TXCurrent,TXPLL);
    //发射模式频率校正
    if (!CalibrateCC1000())// Calibration data is stored in the chip indexed by the selected frequency register 
    {
        writeln("TX Calibration failed");
    }
    //设置CC1000进入接收模式
    SetupCC1000RX(RXCurrent,RXPLL);	
    //接收模式参数校正
    if (!CalibrateCC1000())
    {
        writeln("RX Calibration failed");
    }	
  // Now the CC1000 is calibrated for both RX and TX, we do not need to recalibrate
  // unless the frequency is changed, the temperature changes by 40 degrees C
  // or if the supply voltage changes by more than 0.5V 
  
    // 设置状态机初始状态，强制发生一次状态转换
    State=TX_STATE;
    NextState=IDLE_STATE;
    //初始化RF发送缓存
    InitializeTXBuffer();
    //判断按键，决定是否调用配置菜单
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
    if(BUTTON4==0)	//调用配置菜单
    {           
        ConfigurationMode();
    }
    
  // 因为在测试中我们发现，直接使用ChipCon提供的函数库中的配置函数写
  // CC1000的寄存器时，对于PA_POW这个寄存器的写入总是存在错误，所以在
  // 这里通过补写PA_POW寄存器的操作足以保证正确配置CC1000的输出功率了
  WriteToCC1000Register(CC1000_PA_POW,PA_Power);
 
  //初始化锁定和解锁Average Filter变量
  LockAverage=0;
  UnlockAverage=0;
  
  //显示启动信息
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
  
  //蜂鸣器鸣叫一声，提示射频模块开始运行
  P5DIR |= BIT0;
  P5OUT &= ~BIT0;
  for(i = 255;i > 0;i--)
  {
        dummy = 255;
        while(dummy > 0)dummy--;
  }
  P5OUT |= BIT0;
  
  AverageFreeRunCC1000();  
  
  //打开全局中断
  _EINT(); 
  
  while(1)
  {    
    /* If state change, handle it */
    if (State!=NextState)
    {   
      ChangeState();  
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
    
    if (IFG1 & URXIFG0) // UART receive interrupt. PC has sent data to us.
    {
        if (State==IDLE_STATE)		 // Only handle data if idle
        {  
            TBCTL |= TBCLR; 		 // Reset time-out timer
            TBCTL |= TBSSEL_1 + MC_1; 	 // Start time-out timer
            TBCCR0 = 328;
            
            TXBuffer[TXBufferIndex++]=RXBUF0;
            
            // IMPORTANT : We may have another interrupt or two occur before we 
            // can change mode! Therefore, leave safety margin!
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
  
    if (TBCTL & TBIFG) // TimerB interrupt. TX timer has timed out.
    {
        TBCTL = 0x00;    
        NextState=TX_STATE;    
    }
  
    if(TACTL & TAIFG)  // TimerA interrut ,check the keys
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
    
        TACTL &= ~TAIFG;
    }   
 }

}

// This routine initialises the TX buffer at startup
void InitializeTXBuffer(void)
{
  uchar i;
  
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
       DCLK_INT_UP;		        /* INT on rising edge */
       DIO_IN;			        /* Set DIO as input */
       SetupCC1000RX(RXCurrent,RXPLL);
      }
    
      State=RX_STATE;
      
      SET_RXLED(ON); 
      SET_TXLED(OFF);
      
      //初始化接收状态需要的变量
      PreambleEnd = 0;
      PreambleNextbit = 1;
      BitCounter=0;
      ByteCounter=0;
      break;
    case TX_STATE:
      if(State != TX_STATE) 
      {
        DCLK_INT_DN;		       /* INT on falling edge */
        DIO_OUT;		       /* Set DIO as output */
        SetupCC1000TX(TXCurrent,TXPLL);
      }

      State=TX_STATE;

      SET_RXLED(OFF);
      SET_TXLED(ON);
      ME1 &= ~URXE0;			// 禁止UART接收
        
      BytesToSend=TXBufferIndex;        // Number of bytes to send
      TXBuffer[PREAMBLE_LENGTH+3]=BytesToSend-HEADER_SIZE-PREAMBLE_LENGTH;
      //初始化发射状态需要的变量
      TXBufferIndex=0;
      BitCounter=0;
      ShiftReg=TXBuffer[TXBufferIndex++];
      break;
    case IDLE_STATE:
      if(State==TX_STATE) 
      {
        DCLK_INT_UP;		        /* INT on rising edge */
        DIO_IN;		                /* Set DIO as input */
        SetupCC1000RX(RXCurrent,RXPLL);
      }

      State=IDLE_STATE;

      SET_RXLED(OFF);
      SET_TXLED(OFF);
      
      ME1 |= URXE0;		         // 使能UART接收
      
      //初始化空闲状态需要的变量 
      TXBufferIndex=HEADER_SIZE+PREAMBLE_LENGTH;
      PreambleNextbit = 1;
      BitCounter = 0;
      UnlockAverage=1;
      break;
  }
}

