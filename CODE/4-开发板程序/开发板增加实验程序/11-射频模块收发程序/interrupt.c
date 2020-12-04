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
说明：这个文件里的代码是对Chipcon提供在PIC单片机的中断中处理发射和接收
      字符的程序的移植，并在其基础上做了改进和提高，保证MSP430单片机
      能够更好的处理寻找帧前导码、接收字符和发送字符的工作
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
      if(PreambleEnd)  //已经找到Preamble的结尾？
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
                  case 0 :         // 判断UI1(帧标示1)是否正确
                    if (ShiftReg != UI1)
                    {  
                      writeln("UI1 Error");
                      NextState=IDLE_STATE;
                    }
                    break;
                 case 1 :           // 判断UI2(帧标示2)是否正确
                    if (ShiftReg != UI2)
                    {  
                      writeln("UI2 Error");
                      NextState=IDLE_STATE;
                    }
                    break;
                  case 2 :          // 判断模块地址是否匹配
                    if (ShiftReg != UnitAddress)
                    {  
                      writeln("UnitAddress Error");
                      NextState=IDLE_STATE;
                    }
                    break;
                  case 3 :          // 判断接收字符长度是否符合要求
                    BytesToReceive=ShiftReg;
                    if (BytesToReceive > TX_BUFFER_SIZE) // If invalid length, ignore message
                    {
                        BytesToReceive=0;
                        writeln("Payload Error");
                    }            
                    break;
                  default :         // 接收数据，并保存在UART发送缓存中  
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
      else  //寻找并等待Preamble结束
      {
            if(PreambleNextbit)     //next bit shoule be 1
            {	
                    if(DIO_Value)   //正确，准备接收0
                    {
                        PreambleNextbit=0;
                    }
                    else            //找到了Preamble的尾巴
                    {
                        PreambleEnd=1;
                       goto Start2Receive; //前导码接收完成，跳到接收帧标识和数据   
                    }
            }
            else                    // next bit shoule be 0
            {
                    if(DIO_Value)   //错误，退出RX状态
                    {
                         NextState=IDLE_STATE;	
                    }
                    else            //正确，准备接收1
                    {
                         PreambleNextbit=1;           
                    }	
            }	
      }
      break;
      
    case IDLE_STATE:  //寻找帧的Preamble
      if(PreambleNextbit)   //next bit should be 1
      {	
            if(DIO_Value)   //正确，准备接收0
            {
                PreambleNextbit=0;
                BitCounter++;
            }
            else            //错误，重新寻找
            {
                BitCounter=0;   
            }
      }
      else                  //next bit should be 0
      {
            PreambleNextbit=1; 
            if(DIO_Value)   //错误，重新寻找
            {                       
                BitCounter=0;
            }
            else            //正确
            {
                BitCounter++;
                if(BitCounter >= PREAMBLE_REQ*8)  //已经找到足够多的10101010....
                {
                    LockAverage=1;
                    NextState=RX_STATE;   //进入接收状态 
                }
            }	
      }	                  
     break;
     
     default:   //如果出现意外，回到确知状态
      NextState=IDLE_STATE;
      break;
  }  
}
