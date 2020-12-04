/****************************************************************************/
/* Application note AN015                                                   */
/* Reference design : CC1000 RF Modem                                       */
/*                                                                          */
/* File:      interrupt.c                                                   */
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
/* This file contains the interrupt handler, which handles the external,    */
/* UART, timer 2 and timer 1 interrupts. Communication with the main        */
/* program occurs via global variables.                                     */
/****************************************************************************/


/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: interrupt.c,v $
 * Revision 2.1  2003/04/24 12:43:27  tos
 * Cosmetic change: added revision history for CVS.
 *
 *                                                                           *
 ****************************************************************************/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
˵��������ļ���Ĵ����Ƕ�Chipcon�ṩ��PIC��Ƭ�����ж��д�����ͽ���
      �ַ��ĳ������ֲ����������������˸Ľ�����ߣ���֤MSP430��Ƭ��
      �ܹ����õĴ���Ѱ��֡ǰ���롢�����ַ��ͷ����ַ��Ĺ���
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <msp430x14x.h>
#include "IO_Define.h"
#include "main.h"
#include "configure.h"
#include "simpleio.h"
//#include <stdio.h>

#pragma vector = PORT1_VECTOR 
__interrupt void InterruptHandler(void)
{
   
  P1IFG = 0; 
  /* This function must execute as fast as possible. Therefore, as much 
  processing as possible is left to the main program */ 
  switch (State) 
  {
    case TX_STATE:      /* Write data to CC1000 */ 
      if(ShiftReg&0x80)	DIO_1;
      else		DIO_0;
      
      ShiftReg <<= 1;
      BitCounter++;
      
      if(BitCounter==1)
      {
            if(TXBufferIndex > BytesToSend) 
            {
                  NextState=IDLE_STATE;
                  break;
            }
      }      
      else if(BitCounter==8) 
      {
            BitCounter=0;
            ShiftReg=TXBuffer[TXBufferIndex++];
      }        
      break;
      
    case RX_STATE:      /* Read data from CC1000 */        
      if(PreambleEnd)  //�Ѿ��ҵ�Preamble�Ľ�β��
      {      
Start2Receive:          
            ShiftReg <<= 1;
            if(DIO_Value) ShiftReg |= 0x01;
            BitCounter++;
        
          /* Byte received? */
          if (BitCounter==8) 
          {
                BitCounter=0;
                           
                switch (ByteCounter) 
                {
                  case 0 :         // �ж�UI1(֡��ʾ1)�Ƿ���ȷ
                    if (ShiftReg != UI1)
                    {  
                      writeln("UI1 Error");
                      NextState=IDLE_STATE;
                    }
                    break;
                 case 1 :           // �ж�UI2(֡��ʾ2)�Ƿ���ȷ
                    if (ShiftReg != UI2)
                    {  
                      writeln("UI2 Error");
                      NextState=IDLE_STATE;
                    }
                    break;
                  case 2 :          // �ж�ģ���ַ�Ƿ�ƥ��
                    if (ShiftReg != UnitAddress)
                    {  
                      writeln("UnitAddress Error");
                      NextState=IDLE_STATE;
                    }
                    break;
                  case 3 :          // �жϽ����ַ������Ƿ����Ҫ��
                    BytesToReceive=ShiftReg;
                    if (BytesToReceive > TX_BUFFER_SIZE) // If invalid length, ignore message
                    {
                        BytesToReceive=0;
                        writeln("Payload Error");
                    }            
                    break;
                  default :         // �������ݣ���������UART���ͻ�����  
                    RXBuffer[RXBufferWriteIndex++]=ShiftReg;     
                    RXBufferWriteIndex &= 0x3F;
            }
            
            ByteCounter++;
            if (ByteCounter >= BytesToReceive+4)
            {
              UnlockAverage=1;
              NextState=IDLE_STATE;
            }
         }
      }
      else  //Ѱ�Ҳ��ȴ�Preamble����
      {
            if(PreambleNextbit)     //next bit shoule be 1
            {	
                    if(DIO_Value)   //��ȷ��׼������0
                    {
                        PreambleNextbit=0;
                    }
                    else            //�ҵ���Preamble��β��
                    {
                        PreambleEnd=1;
                       goto Start2Receive; //ǰ���������ɣ���������֡��ʶ������   
                    }
            }
            else                    // next bit shoule be 0
            {
                    if(DIO_Value)   //�����˳�RX״̬
                    {
                         NextState=IDLE_STATE;	
                    }
                    else            //��ȷ��׼������1
                    {
                         PreambleNextbit=1;           
                    }	
            }	
      }
      break;
      
    case IDLE_STATE:  //Ѱ��֡��Preamble
      if(PreambleNextbit)   //next bit should be 1
      {	
            if(DIO_Value)   //��ȷ��׼������0
            {
                PreambleNextbit=0;
                BitCounter++;
            }
            else            //��������Ѱ��
            {
                BitCounter=0;   
            }
      }
      else                  //next bit should be 0
      {
            PreambleNextbit=1; 
            if(DIO_Value)   //��������Ѱ��
            {                       
                BitCounter=0;
            }
            else            //��ȷ
            {
                BitCounter++;
                if(BitCounter >= PREAMBLE_REQ*8)  //�Ѿ��ҵ��㹻���10101010....
                {
                    LockAverage=1;
                    NextState=RX_STATE;   //�������״̬ 
                }
            }	
      }	                  
     break;
     
     default:   //����������⣬�ص�ȷ֪״̬
      NextState=IDLE_STATE;
      break;
  }  
}
