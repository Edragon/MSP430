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
˵��������ļ���Ĵ����Ƕ�Chipcon�ṩ��PIC��Ƭ������CC1000��main����
      ��ֲ��ԭ�����ļ��������ͺ궨�壬�Լ���������������main()ǰ����
      �˸о��ܲ�����������λ�ý����˵������������С���ֲʱ������ԭ
      ���������й��ܣ�������������������ư�����
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*
����ָ�ϣ��ϵ��ѧϰ��ͨ��������PC����״̬��Ϣ������û������б��
���ò�������ʹ��Ĭ�����ý���RFͨ�ţ�Ĭ��������Ϣ����--
�ز�Ƶ�ʣ�433.916400MHz���������ʣ�2.4Kbps�����ݸ�ʽ��NRZ��
���书�ʣ�0 dBm��
������ϵ�֮ǰ���û���סK4���򽫻���PC����Ļ����ʾ���ò˵����û�����
������ʾ��Ϣ���������ʺͷ��书�ʽ������ã������Բ鿴CC1000�ڲ��ļĴ�
����ֵ����������ú�ֻҪά��ѧϰ���Դ��һֱ���ֵ�ǰ���ã��������
����Ϣ��ʧ���û�ֻ�����ϵ��Ƕ�CC1000����һ�����á�
��Ƭ����ɶ�CC1000�����úͳ�ʼ�������Ժ󣬻Ὣ״̬��Ϣ���͸�PC��������
�Ʒ���������һ������ʾ�û���ʼ��������ɡ�
��ʱ���û����԰������������Ӧ���ַ�����ͨ����������CC1000���͵���Ϣ��
���CC1000�յ���ȷ����֡���Ὣ���յ�������ͨ��������ʾ��PC����Ļ�ϡ�
*/
/*--------------------------------------------------------------------
�����Է��֣���76.8Kbps��NRZ��ʽ��38.4Kbps��manchester��ʽ��������ʱ��
            ���ն˵Ĵ����ʼ��ߣ���������ʹ�á�����ԭ��һ������MCU��
            �����������ޣ����ʱ��8MHz����һ����������ǵ��жϴ�����
            ��C���Ա�д�ģ�����Ƚ�ռ���жϴ���ʱ��ģ���������û���
            ʹ�����������������Ҫ�������ܸ�Ϊǿ���Ĵ���������MSP430F2XXX��
            �����ǽ��жϴ�������û���������±�д��
            �����������£�����ͨ��������ֻ��ż�������λ������������
            ���󣬽����û��ڱ������������ÿ֡��β����16bitѭ������У��
            �������в��;�������������Ч�����λ��������⡣
---------------------------------------------------------------------*/
/*************************************************************************
�����û��������������ݣ�
            1.CC1000��RSSI�����Ѿ����ӵ���MCU��P6.2���ţ��û���������MCU
              ����ADC��1602Һ������һ�����ֻ���ʾ�����ź�ǿ�ȵ��ź�ָʾ����
            2.���ð��ϵ�EEPROM����MCU�ڲ�FLASH�����ܹ����浱ǰ���õ�ϵͳ��
              Ҳ�����û�����Ƶϵͳ��������Ϣ�ڵ���󱣴���EEPROM��FLASH�У�
              �ϵ���ٴ�����������CC1000��
            3.��֡��β����16CRC�������;����˼��;����ܣ�Ҳ�������ǿ
              ��֡����Ŀɿ��ԡ�
            4.�����ϲ������Э�飬����ʹ�ô���Ӧ�����ݼ��ܡ���Ȩ���ʵȻ�
              ����ʵ�ָ���׳ʵ�õ�����ͨ�š�
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
//*****************������**************/
void main(void)
{
    uchar i,dummy;   
    
    WDTCTL = WDTPW + WDTHOLD;		      // �رտ��Ź�
    
    BCSCTL1 &= ~XT2OFF;                       // XT2on 
    do
    {
    IFG1 &= ~OFIFG;                           // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);               // Time for flag to set
    }
    while ((IFG1 & OFIFG));                   // OSCFault flag still set?
    BCSCTL2 |= SELM_2 + SELS;                 // MCLK = SMCLK = XT2 (safe)
    
    //��ʼ��CC1000���ƶ˿�
    Ctrl_DIR |= PDATA + PALE + PCLK;	
    Ctrl_Port |= PDATA + PALE + PCLK;
    DIO_IN;				      // DIO����Ϊ���뷽��
    //�����˿�����
    P1DIR = 0x80;	                      //P1.0~P1.3�˿����ڰ�������
    P1OUT = 0x00;
    DCLK_INT_EN;                              //��DCLK�˿��ж�
    DCLK_INT_UP;                              
    //�շ�ָʾ������
    P2OUT = 0xff;
    P2DIR = 0xff;
    //UARTͨ�Ŷ˿�����
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL1;                          // UCLK = SMCLK
    UBR00 = 0xA0;                             // 8Mhz/19200 ~ 417
    UBR10 = 0x01;                             //
    UMCTL0 = 0x00;                            // no modulation                           // 8MHz 115200 modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    IE1 &= ~(URXIE0 + UTXIE0);                // Disable USART0 RX and TX interrupt
    //��ʱ��A������ʱ��ⰴ���Ƿ񱻰���
    TACTL =  TASSEL_1 + MC_1; 			//ѡ��327868Hzʱ��Դ
    CCR0  = 3277;				//�趨100ms�ж�һ��
    //��ʱ��B�������յ�UART�����Ժ�ʱ��״̬�л�������ģʽ
    TBCTL = TBSSEL_1;				// ѡ��32768Hzʱ��Դ
    TBCCR0 = 328;				// 10ms�ж�һ��
    
    //��ʼ��UART�շ�����ָ��
    RXBufferReadIndex=0;
    RXBufferWriteIndex=0;
  
    //����CC1000����Power Downģʽ
    SetupCC1000PD();
    //��λCC1000
    ResetCC1000();
    //����CC1000�ڲ����п��ƼĴ���
    SetupCC1000All();
    //����CC1000���뷢��ģʽ
    WakeUpCC1000ToTX(TXCurrent,TXPLL);
    //���÷���ģʽ����
    SetupCC1000TX(TXCurrent,TXPLL);
    //����ģʽƵ��У��
    if (!CalibrateCC1000())// Calibration data is stored in the chip indexed by the selected frequency register 
    {
        writeln("TX Calibration failed");
    }
    //����CC1000�������ģʽ
    SetupCC1000RX(RXCurrent,RXPLL);	
    //����ģʽ����У��
    if (!CalibrateCC1000())
    {
        writeln("RX Calibration failed");
    }	
  // Now the CC1000 is calibrated for both RX and TX, we do not need to recalibrate
  // unless the frequency is changed, the temperature changes by 40 degrees C
  // or if the supply voltage changes by more than 0.5V 
  
    // ����״̬����ʼ״̬��ǿ�Ʒ���һ��״̬ת��
    State=TX_STATE;
    NextState=IDLE_STATE;
    //��ʼ��RF���ͻ���
    InitializeTXBuffer();
    //�жϰ����������Ƿ�������ò˵�
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
    if(BUTTON4==0)	//�������ò˵�
    {           
        ConfigurationMode();
    }
    
  // ��Ϊ�ڲ��������Ƿ��֣�ֱ��ʹ��ChipCon�ṩ�ĺ������е����ú���д
  // CC1000�ļĴ���ʱ������PA_POW����Ĵ�����д�����Ǵ��ڴ���������
  // ����ͨ����дPA_POW�Ĵ����Ĳ������Ա�֤��ȷ����CC1000�����������
  WriteToCC1000Register(CC1000_PA_POW,PA_Power);
 
  //��ʼ�������ͽ���Average Filter����
  LockAverage=0;
  UnlockAverage=0;
  
  //��ʾ������Ϣ
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
  
  //����������һ������ʾ��Ƶģ�鿪ʼ����
  P5DIR |= BIT0;
  P5OUT &= ~BIT0;
  for(i = 255;i > 0;i--)
  {
        dummy = 255;
        while(dummy > 0)dummy--;
  }
  P5OUT |= BIT0;
  
  AverageFreeRunCC1000();  
  
  //��ȫ���ж�
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
        else   		//����һ���ƶ������������жϱ�־
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
      
      //��ʼ������״̬��Ҫ�ı���
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
      ME1 &= ~URXE0;			// ��ֹUART����
        
      BytesToSend=TXBufferIndex;        // Number of bytes to send
      TXBuffer[PREAMBLE_LENGTH+3]=BytesToSend-HEADER_SIZE-PREAMBLE_LENGTH;
      //��ʼ������״̬��Ҫ�ı���
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
      
      ME1 |= URXE0;		         // ʹ��UART����
      
      //��ʼ������״̬��Ҫ�ı��� 
      TXBufferIndex=HEADER_SIZE+PREAMBLE_LENGTH;
      PreambleNextbit = 1;
      BitCounter = 0;
      UnlockAverage=1;
      break;
  }
}

