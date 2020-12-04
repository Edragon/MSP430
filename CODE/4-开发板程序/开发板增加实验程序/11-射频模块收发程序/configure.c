/****************************************************************************/
/* Application note AN015                                                   */
/* Reference design : CC1000 RF Modem                                       */
/*                                                                          */
/* File:      configure.c                                                   */
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
/* This file contains routines for CC1000 configuration. The configuration  */
/* menu is implemented here.                                                */
/****************************************************************************/

/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: configure.c,v $
 * Revision 2.1  2003/04/24 12:42:41  tos
 * Cosmetic change: added revision history for CVS.
 *
 *                                                                           *
 ****************************************************************************/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
说明：这个文件里的代码是对Chipcon提供在PIC单片机中对CC1000的配置函数
      的移植程序，源程序具有保存掉电后的配置信息和在线更新（串口）功能，
      在LT-1B开发板上同样可以实现这些功能，但是由于开发时间有限，我们将
      这部分功能留给广大用户去实现。这里只移植了原PIC程序中的除上面提到
      的两项功能以外的其他函数，并在此基础上增加了串口配置RF输出功率的
      功能。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "configure.h"
#include "cc1000.h"
#include "simpleio.h"
#include <msp430x14x.h>	 
#include <ctype.h>   
#include "mstdio.h"   
#include "IO_Define.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

/*************上电复位后执行此默认配置****************************/
// 默认配置，处于接收模式 433.916400 MHz, NRZ格式, 2.4K, Low LO
// FREQ_2A  --->   TEST2  顺序排列
const uchar EEPROMCC1000Config[33]=
 {0x58,0x00,0x00,0x41,0xfc,0x9c,0x02,0x80,0x44,0x12,
  0x0f,0x60,0x10,0x26,0x8e,0x69,0x23,0x70,0x01,0x1C,
  0x16,0x10,0x0A,0x06,0x03,0x01,0x02,0x00,0x10,0x08,
  0x25,0x04,0x00}; 
// 默认的发射和接收时的电流和PLL寄存器参数
const  uchar EEPROMTXCurrent = 0x81;
const  uchar EEPROMRXCurrent = 0x44; 
const  uchar EEPROMTXPLL     = 0x48;
const  uchar EEPROMRXPLL     = 0x60;
// 默认的四个按键对应的键值
const  uchar EEPROMButton1Char = 'L';
const  uchar EEPROMButton2Char = 'T';
const  uchar EEPROMButton3Char = 'D';
const  uchar EEPROMButton4Char = 'Z';
// 默认的模块单元地址
const uchar UnitAddress = 0xff;

// 保存在RAM中的发射功率控制变量
uchar PA_Power;
// 保存在RAM中的键值变量，用户可以配置的按键数值
uchar Button1Char, Button2Char, Button3Char, Button4Char; 
// RAM中的发射和接收参数变量
uchar TXCurrent;
uchar RXCurrent;
uchar TXPLL;
uchar RXPLL;

// Configures all the CC1000 registers according to the values stored in EEPROM
void SetupCC1000All(void)
{ 
  uchar counter;
  uchar value;
  
  for (counter=0x01;counter<=0x1C;counter++) //配置了28个寄存器
  {
    value=EEPROMCC1000Config[counter-1];
    WriteToCC1000Register(counter,value);
  }
  
  for (counter=0x40;counter<=0x44;counter++) //配置了5个寄存器
  {
    value=EEPROMCC1000Config[counter-0x40+0x1C];
    WriteToCC1000Register(counter,value);
  }
  
  PA_Power = EEPROMCC1000Config[0x0a];
  Button1Char=EEPROMButton1Char;
  Button2Char=EEPROMButton2Char;
  Button3Char=EEPROMButton3Char;
  Button4Char=EEPROMButton4Char;
  TXCurrent=EEPROMTXCurrent;
  RXCurrent=EEPROMRXCurrent;
  TXPLL=EEPROMTXPLL;
  RXPLL=EEPROMRXPLL;
}

// Dumps the values of all the registers of the CC1000 to the screen
void DumpCC1000Regs(void)
{
  uchar RegCounter;
  
  writestr("Register dump:\n\r\n\r");
  for(RegCounter=0x00;RegCounter<=0x1C;RegCounter++)
  {
    writestr("Addr. ");
    writehex(RegCounter);
    writestr(" : ");
    writehex(ReadFromCC1000Register(RegCounter));
    writeln("");
  }
  for(RegCounter=0x40;RegCounter<=0x46;RegCounter++) 
  {
    writestr("Addr. ");
    writehex(RegCounter);
    writestr(" : ");
    writehex(ReadFromCC1000Register(RegCounter));
    writeln("");
  }
}

// Lets the user select the RF data rate
void SetRFDataRate(void)
{
  unsigned int i;
  uchar regvalue;
  uchar PLLvalue;
   
  writeln("Select data rate (in kbit/s):\n\r");
  writeln("1 - 0.6kbit/s NRZ");
  writeln("2 - 0.6kbit/s Manchester");
  writeln("3 - 1.2kbit/s NRZ");
  writeln("4 - 1.2kbit/s Manchester");
  writeln("5 - 2.4kbit/s NRZ");
  writeln("6 - 2.4kbit/s Manchester");
  writeln("7 - 4.8kbit/s NRZ");
  writeln("8 - 4.8kbit/s Manchester");
  writeln("9 - 9.6kbit/s NRZ");
  writeln("A - 9.6kbit/s Manchester");
  writeln("B - 19.2kbit/s NRZ");
  writeln("C - 19.2kbit/s Manchester");
  writeln("D - 38.4kbit/s NRZ");
  writeln("E - 38.4kbit/s Manchester");
  writeln("F - 76.8kbit/s NRZ\n\r");
  writestr("Enter choice >");
  
  i=getchar();
  writeln("");
  
  switch(toupper(i)) 
  {
    case '1' :
      regvalue=0x03;
      PLLvalue=0x25;
      break;
    case '2' :
      regvalue=0x07;
      PLLvalue=0x25;
      break;
    case '3' :
      regvalue=0x13;
      PLLvalue=0x25;
      break;
    case '4' :
      regvalue=0x17;
      PLLvalue=0x25;
      break;
    case '5' :
      regvalue=0x23;
      PLLvalue=0x25;
      break;
    case '6' :
      regvalue=0x27;
      PLLvalue=0x25;
      break;
    case '7' :
      regvalue=0x33;
      PLLvalue=0x25;
      break;
    case '8' :
      regvalue=0x37;
      PLLvalue=0x25;
      break;
    case '9' :
      regvalue=0x43;
      PLLvalue=0x3f;
      break;
    case 'A' :
      regvalue=0x47;  
      PLLvalue=0x3f;
      break;
    case 'B' :
      regvalue=0x53;
      PLLvalue=0x3f;
      break;
    case 'C' :
      regvalue=0x57;
      PLLvalue=0x3f;
      break;
    case 'D' :
      regvalue=0x51;
      PLLvalue=0x3f;
      break;
    case 'E' :
      regvalue=0x55;
      PLLvalue=0x3f;
      break;
    case 'F' :
      regvalue=0x50;
      PLLvalue=0x3f;
      break;
  }
  WriteToCC1000Register(CC1000_MODEM0,regvalue);
  //EEPROMCC1000Config[0x10]=regvalue;
  
  WriteToCC1000Register(CC1000_TEST4,PLLvalue);
 // EEPROMCC1000Config[0x1e]=PLLvalue;
  writeln("---DONE---");
}

// Lets the user select the RF output power
void PowerSetting(void)
{
  uchar i;
  
  writeln("Select output power(in dbm):\n\r");
  writeln("0 - -20 dBm");
  writeln("1 - -19 dBm");
  writeln("2 - -18 dBm");
  writeln("3 - -17 dBm");
  writeln("4 - -16 dBm");
  writeln("5 - -15 dBm");
  writeln("6 - -14 dBm");
  writeln("7 - -13 dBm");
  writeln("8 - -12 dBm");
  writeln("9 - -11 dBm");
  writeln("A - -10 dBm");
  writeln("B -  -9 dBm");
  writeln("C -  -8 dBm");
  writeln("D -  -7 dBm");
  writeln("E -  -6 dBm");
  writeln("F -  -5 dBm");
  writeln("G -  -4 dBm");
  writeln("H -  -3 dBm");
  writeln("I -  -2 dBm");
  writeln("J -  -1 dBm");
  writeln("K -   0 dBm");
  writeln("L -   1 dBm");
  writeln("M -   2 dBm");
  writeln("N -   3 dBm");
  writeln("O -   4 dBm");
  writeln("P -   5 dBm");
  writeln("Q -   6 dBm");
  writeln("R -   7 dBm");
  writeln("S -   8 dBm");
  writeln("T -   9 dBm");
  writeln("U -  10 dBm");
  writestr("Enter choice >");
  
  i=getchar();
  writeln("");  
  
    switch (toupper(i)) 
    {
    case '0':
    case '1':
            PA_Power = 0x01;
            break;
    case '2':
    case '3':
    case '4':
            PA_Power = 0x02;
            break;
    case '5':
    case '6':
    case '7':
            PA_Power = 0x03;
            break;
    case '8':
    case '9':
            PA_Power = 0x04;
            break;
    case 'A':
    case 'B':
            PA_Power = 0x05;
            break;
    case 'C':
            PA_Power = 0x06;
            break;
    case 'D':
            PA_Power = 0x07;
            break;
    case 'E':
            PA_Power = 0x08;
            break;        
    case 'F':
            PA_Power = 0x09;
            break;      
    case 'G':
            PA_Power = 0x0A;
            break;  
    case 'H':
            PA_Power = 0x0B;
            break;
    case 'I':
            PA_Power = 0x0C;
            break;
    case 'J':
            PA_Power = 0x0E;
            break;
    case 'K':
            PA_Power = 0x0F;
            break;
    case 'L':
            PA_Power = 0x40;
            break;
    case 'M':
            PA_Power = 0x50;
            break;
    case 'N':
            PA_Power = 0x50;
            break;
    case 'O':
            PA_Power = 0x60;
            break;
    case 'P':
            PA_Power = 0x70;
            break;
    case 'Q':
            PA_Power = 0x80;
            break; 
    case 'R':
            PA_Power = 0x90;
            break;  
    case 'S':
            PA_Power = 0xA0;
            break;              
    case 'T':
            PA_Power = 0xC0;
            break;   
    case 'U':
            PA_Power = 0xFF;
            break;        
    }
    
  WriteToCC1000Register(CC1000_PA_POW,PA_Power);
  writeln("---DONE---");
}


// Lets the user set up which characters are transmitted when the buttons are pressed
void ButtonConfig(void)
{
  writestr("Enter character to be sent when button 1 is pressed: ");
  Button1Char=getchar();
  writeln("");
  writestr("\n\rEnter character to be sent when button 2 is pressed: ");
  Button2Char=getchar(); 
  writeln("");
  writestr("\n\rEnter character to be sent when button 3 is pressed: ");
  Button3Char=getchar(); 
  writeln("");
  writestr("\n\rEnter character to be sent when button 4 is pressed: ");
  Button4Char=getchar(); 
  writeln("");
  writeln("---DONE---");
}  

// Displays the configuration menu
void DisplayConfigMenu(void)
{
  writeln("RF Modem Configuration Menu\n\r");
  writeln("D - Dump CC1000 registers");
  writeln("E - Set RF data rate");
  writeln("P - Set RF output power");
  writeln("C - Calibrate");
  writeln("R - RX mode");
  writeln("T - TX mode");
  writeln("L - PLL lock");
  writeln("B - Button configuration");
  writeln("X - Exit from configuration menu");
  writeln("* - Any other chars to redisplay the menu");
}

// Handles keystrokes for the configuration menu
void HandleConfigMenu(uchar inputvalue)
{
  switch(toupper(inputvalue)) 
  {
    case 'D' : 
      DumpCC1000Regs();
      break; 
    case 'E' :
      SetRFDataRate();
      break;
    case 'X' :
      break;
    case 'P' :
      PowerSetting();
      break;
    case 'C' :
      if (CalibrateCC1000())
        writeln("Calibrate OK");
      else
        writeln("Calibrate failed");
      break;
    case 'R' :
      SetupCC1000RX(RXCurrent,RXPLL);
      DCLK_INT_UP;           // INT on rising edge
      DIO_IN;	             // Set DIO as input
      writeln("---DONE---");
      break;
    case 'T' :
      SetupCC1000TX(TXCurrent,TXPLL);
      DCLK_INT_DN;	    // INT on falling edge
      DIO_OUT;		    // Set DIO as output
      writeln("---DONE---");
      break;      
    case 'L' :
      if (ReadFromCC1000Register(CC1000_LOCK)&0x01==1)
        writeln("PLL in lock");
      else
        writeln("PLL out of lock");
      break;
    case 'B' :
      ButtonConfig();
      break;
    default:
      DisplayConfigMenu();
      break;
  }
}

// Runs the configuration menu
void ConfigurationMode(void)
{
  unsigned int i;
  
  DisplayConfigMenu();
 
  while(1) 
  {
    writeln(">");
    i=getchar();
    writeln("");
    HandleConfigMenu(i);
    if (toupper(i)=='X') 
    {
      writeln("RF module is working......");				//SetupCC1000All(); 
      return;
    }
  } 
}
