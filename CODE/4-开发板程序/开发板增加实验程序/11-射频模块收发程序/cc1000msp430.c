/****************************************************************************/
/* Application note AN009                                                   */
/* CC1000 interface library                                                 */
/*                                                                          */
/* File:      cc1000.c                                                      */
/* Revision:  2.1                                                           */
/*                                                                          */
/* Microcontroller:                                                         */
/*          Microchip PIC16F876                                             */
/* Written for the IAR PIC16 compiler                                       */
/*                                                                          */
/* Author:  Karl H. Torvmark, Field Applications Engineer, Chipcon          */
/*                                                                          */
/* Contact: Chipcon AS +47 22 95 85 44                                      */
/*          wireless@chipcon.com                                            */
/*                                                                          */
/* Changes:                                                                 */
/*      2.1 : Added change of PLL register between RX and TX                */
/*      2.0 : Changed file into library format, many other changes, added   */
/*            SPI code                                                      */
/*      1.0 : First version                                                 */
/****************************************************************************/

/****************************************************************************/
/* This library contains functions for configuring the CC1000. These        */
/* routines use bit-banging to program the CC1000, faster configuration is  */
/* possible by using a synchronous serial port such as a SPI interface.     */
/* The header file "modemhw.h" contains definitions for the various I/O     */
/* pins, the user should make a similar file to name the pins used to       */
/* communicate with the CC1000. Routines to read and write the calibration  */
/* values in the CC1000 are provided, they are not used in this reference   */
/* application, but are useful in other applications, most notably          */
/* frequency-agile and frequency hopping applications. See application      */
/* note AN009 for more information.                                         */
/* The routines in this file will have to be adapted depending on the MCU   */
/* and compiler used. The method used for shifting data in and out may have */
/* to be changed if the bit ordering for bitfields is different from the    */
/* IAR PIC compiler.                                                        */
/*                                                                          */
/* Configuration routines are included in two versions: one using general   */
/* I/O ports ("bit-banging"), and one using the built-in SPI interface of   */
/* the PIC16F876. If possible, the SPI version should be used, as this is   */
/* much faster. The SPI versions are used if the symbol "SPI" is defined,   */
/* otherwise the general I/O-based version is used.                         */
/****************************************************************************/

/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: cc1000pic.c,v $
 * Revision 2.7  2003/05/08 08:17:46  tos
 * Corrected freq.synth.reset: toggle MODEM1 reset back to 1 from 0.
 *
 * Revision 2.6  2003/05/07 15:51:06  tos
 * Modified setup RX/TX (ref.: Errata Note 01): reset synth if unable to lock PLL.
 *
 * Revision 2.5  2003/05/07 15:43:44  tos
 * Modified calibrate: moved LOCK monitor before turning PA on.
 *
 * Revision 2.4  2003/05/07 15:02:29  tos
 * Modified LOCK monitor in RX/TX setup: wait for LOCK before turning PA on.
 *
 * Revision 2.3  2003/04/25 13:13:19  tos
 * Removed obsolete timeout definition.
 *
 * Revision 2.2.2.1  2003/05/07 14:34:49  tos
 * Modified LOCK monitor in RX/TX setup: wait for LOCK before turning PA on.
 *
 * Revision 2.2  2003/04/24 12:41:36  tos
 * Corrected inconsistent monitoring of CC1000: [calibration complete] + [lock].
 *
 *                                                                           *
 ****************************************************************************/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
说明：这个文件里的代码是对Chipcon提供在PIC单片机驱动CC1000的底层源码
      在MSP430单片机上的移植，这里的函数是驱动CC1000的最基本的元函数
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include <msp430x14x.h>
#include "IO_Define.h"
#include "CC1000.h"

#define CAL_TIMEOUT     0x7FFE
#define LOCK_TIMEOUT    0x7FFE

#define PA_VALUE        0xF0

/****************************************************************************/
/*  This routine sends new configuration data to the CC1000                 */
/*  Based on bit bashing (general I/O pin use)                              */
/****************************************************************************/

void ConfigureCC1000(unsigned char Count, unsigned int Configuration[])
{
  unsigned char  BitCounter;
  unsigned char  WordCounter;
  unsigned int	     Data;
	
   
  PALE_1; 
  
  for (WordCounter=0;WordCounter<Count;WordCounter++)
  {
    Data=Configuration[WordCounter];
    PALE_0; 
    
    /* Send address bits */
    for (BitCounter=0;BitCounter<7;BitCounter++)
    {
      PCLK_1; 
      if ((Data&0x8000)==0) 
      {
        PDATA_0; 
      }
      else
      {
        PDATA_1; 
      }
      Data=Data<<1;
      PCLK_0; 
    }
    
    /* Send read/write bit */
    /* Ignore bit in data, always use 1 */
    
    PCLK_1; 
    PDATA_1;  
    PCLK_0; 
    Data=Data<<1;
    PCLK_1; 
    PALE_1; 
    
    /* Send data bits */
    
    for (BitCounter=0;BitCounter<8;BitCounter++)
    {
      PCLK_1; 
      if ((Data&0x8000)==0) 
      {
        PDATA_0;
      }
      else
      {
       PDATA_1;
      }
      Data=Data<<1;
      PCLK_0; 
     }
  PCLK_1; 
  }   /* Finished with word */   
}

/****************************************************************************/
/*  This routine writes to a single CC1000 register                         */
/****************************************************************************/

void WriteToCC1000Register(unsigned char addr, unsigned char data)
{
  unsigned int val;
  
  val=(addr&0x7F)<<9 | (data&0xFF);
  ConfigureCC1000(1,&val);
}

/****************************************************************************/
/*  This routine writes to a single CC1000 register, with data and address  */
/*  given in the same variable                                              */
/****************************************************************************/

void WriteToCC1000RegisterWord(unsigned int addranddata)
{
 
  ConfigureCC1000(1,&addranddata);
}

/****************************************************************************/
/*  This routine reads from a single CC1000 register                        */
/****************************************************************************/

unsigned char ReadFromCC1000Register(unsigned char addr)
{
  unsigned char BitCounter;
  unsigned char Data;
  
    
  PALE_1;
  
  Data=addr<<1;
  PALE_0;  
    
  /* Send address bits */
  for (BitCounter=0;BitCounter<7;BitCounter++)
  {
    PCLK_1; 
    if ((Data&0x80)==0) 
    {
     PDATA_0; 
    }
    else
    {
      PDATA_1; 
    }
    Data=Data<<1;
    PCLK_0; 
  }
  
  /* Send read/write bit */
  /* Ignore bit in data, always use 0 */
  
  PCLK_1;
  PDATA_0; 
  PCLK_0; 
 
 
  PCLK_1; 
  PALE_1; 
    
  /* Receive data bits */ 
  PDATA_1;  
  PDATA_IN; /* Set up PDATA as an input */
    
  for (BitCounter=0;BitCounter<8;BitCounter++)
  {
    PCLK_0; 
    Data=Data<<1;
    if ((PDATA_Value)==0)
    {
      Data&=0xFE;
    } 
     else
    {
      Data|=0x01;
    }
    PCLK_1; 
  }

  PDATA_OUT; /* Set up PDATA as an output again */
  
  return Data;
}
  
/****************************************************************************/
/*  This routine resets the CC1000, clearing all registers.                 */
/****************************************************************************/  

void ResetCC1000(void)
{
  char MainValue;
  
  MainValue=ReadFromCC1000Register(CC1000_MAIN);
  WriteToCC1000Register(CC1000_MAIN,MainValue & 0xFE);         // Reset CC1000
  WriteToCC1000Register(CC1000_MAIN,MainValue | 0x01);         // Bring CC1000 out of reset
}

/****************************************************************************/
/*  This routine calibrates the CC1000                                      */
/*  Returns 0 if calibration fails, non-zero otherwise. Checks the LOCK     */
/*  to check for success.                                                   */
/****************************************************************************/

unsigned char CalibrateCC1000(void)
{  
  unsigned int TimeOutCounter;

  WriteToCC1000Register(CC1000_PA_POW,0x00); // Turn off PA to avoid spurs
                                             // during calibration in TX mode
  WriteToCC1000Register(CC1000_CAL,0xA6); // Start calibration

  // Wait for calibration complete
  for(TimeOutCounter=CAL_TIMEOUT; ((ReadFromCC1000Register(CC1000_CAL)&0x08)==0)&&(TimeOutCounter>0); TimeOutCounter--);

  // Wait for lock
  for(TimeOutCounter=LOCK_TIMEOUT; ((ReadFromCC1000Register(CC1000_LOCK)&0x01)==0)&&(TimeOutCounter>0); TimeOutCounter--);
  
  WriteToCC1000Register(CC1000_CAL,0x26); /* End calibration */
  WriteToCC1000Register(CC1000_PA_POW,PA_VALUE); /* Restore PA setting */

  return ((ReadFromCC1000Register(CC1000_LOCK)&0x01)==1);
}

/****************************************************************************/
/*  This routine puts the CC1000 into RX mode (from TX). When switching to  */
/*  RX from PD, use WakeupC1000ToRX first                                   */
/****************************************************************************/


unsigned char SetupCC1000RX(unsigned char RXCurrent,unsigned char RX_PLL)
{
  unsigned int i;
  unsigned char lock_status;

  WriteToCC1000Register(CC1000_MAIN,0x11);    // Switch into RX, switch to freq. reg A
  WriteToCC1000Register(CC1000_PLL,RX_PLL);   // Use RX refdiv setting
  WriteToCC1000Register(CC1000_CURRENT,RXCurrent); // Program VCO current for RX

  // Wait 250us before monitoring LOCK
  for (i=0;i<0x1000;i++);

  // Wait for lock
  for(i=LOCK_TIMEOUT; ((ReadFromCC1000Register(CC1000_LOCK)&0x01)==0)&&(i>0); i--);

  // If PLL in lock
  if ((ReadFromCC1000Register(CC1000_LOCK)&0x01)==0x01)
 {
    // Indicate PLL in LOCK
    lock_status = LOCK_OK;
  // Else (PLL out of LOCK)
  }
 else
 {
    // If recalibration ok
    if(CalibrateCC1000())
	{
      // Indicate PLL in LOCK
      lock_status = LOCK_RECAL_OK;
    // Else (recalibration failed)
    }
	else
	{
      // Reset frequency syncthesizer (ref.: Errata Note 01)
      ResetFreqSynth();
      // Indicate PLL out of LOCK
      lock_status = LOCK_NOK;
    }
  }

  // Return LOCK status to application
  return (lock_status);
}

/****************************************************************************/
/*  This routine puts the CC1000 into TX mode (from RX). When switching to  */
/*  TX from PD, use WakeupCC1000ToTX first                                  */
/****************************************************************************/

unsigned char SetupCC1000TX(unsigned char TXCurrent,unsigned char TX_PLL)
{
  unsigned int i;
  unsigned char lock_status;

  WriteToCC1000Register(CC1000_PA_POW,0x00);   // Turn off PA to avoid frequency splatter

  WriteToCC1000Register(CC1000_MAIN,0xE1);    // Switch into TX, switch to freq. reg B
  WriteToCC1000Register(CC1000_PLL,TX_PLL);   // Use TX refdiv setting
  WriteToCC1000Register(CC1000_CURRENT,TXCurrent); // Program VCO current for TX

  // Wait 250us before monitoring LOCK
  for (i=0;i<0x1000;i++);

  // Wait for lock
  for(i=LOCK_TIMEOUT; ((ReadFromCC1000Register(CC1000_LOCK)&0x01)==0)&&(i>0); i--);

  // If PLL in lock
  if ((ReadFromCC1000Register(CC1000_LOCK)&0x01)==0x01)
  {
    // Indicate PLL in LOCK
    lock_status = LOCK_OK;
  // Else (PLL out of LOCK)
  }
  else
  {
    // If recalibration ok
    if(CalibrateCC1000())
	{
      // Indicate PLL in LOCK
      lock_status = LOCK_RECAL_OK;
    // Else (recalibration failed)
    }
	else
	{
      // Reset frequency syncthesizer (ref.: Errata Note 01)
      ResetFreqSynth();
      // Indicate PLL out of LOCK
      lock_status = LOCK_NOK;
    }
  }

  // Increase output power
  WriteToCC1000Register(CC1000_PA_POW,PA_VALUE); // Restore PA setting
  
  // Return LOCK status to application
  return (lock_status);
}

/****************************************************************************/
/*  This routine puts the CC1000 into power down mode. Use WakeUpCC1000ToRX */
/*  followed by SetupCC1000RX or WakeupCC1000ToTX followed by SetupCC1000TX */
/*  to wake up from power down                                              */
/****************************************************************************/

void SetupCC1000PD(void)
{
  WriteToCC1000Register(CC1000_MAIN,0x3F);    // Put CC1000 into power-down
  WriteToCC1000Register(CC1000_PA_POW,0x00);  // Turn off PA to minimise current draw
}

/****************************************************************************/
/*  This routine wakes the CC1000 up from PD mode to RX mode, call          */
/*  SetupCC1000RX after this routine is finished.                           */
/****************************************************************************/

void WakeUpCC1000ToRX(unsigned char RXCurrent,unsigned char RX_PLL)
{
  unsigned int i;
  
  
  WriteToCC1000Register(CC1000_MAIN,0x3B);  // Turn on xtal oscillator core
  WriteToCC1000Register(CC1000_CURRENT,RXCurrent); // Program VCO current for RX 
  WriteToCC1000Register(CC1000_PLL,RX_PLL); // Use RX refdiv setting
  
  // Insert wait routine here, must wait for xtal oscillator to stabilise, 
  // typically takes 2-5ms.
  for (i=0;i<0x7FFE;i++);
  
  WriteToCC1000Register(CC1000_MAIN,0x39);  // Turn on bias generator
  // Wait for 250us, insert wait loop here
  WriteToCC1000Register(CC1000_MAIN,0x31);  // Turn on frequency synthesiser
}

/****************************************************************************/
/*  This routine wakes the CC1000 up from PD mode to TX mode, call          */
/*  SetupCC1000TX after this routine is finished.                           */
/****************************************************************************/

void WakeUpCC1000ToTX(unsigned char TXCurrent,unsigned char TX_PLL)
{
  unsigned int i;
  
  
  WriteToCC1000Register(CC1000_MAIN,0xFB);  // Turn on xtal oscillator core
  WriteToCC1000Register(CC1000_CURRENT,TXCurrent); // Program VCO current for TX
  WriteToCC1000Register(CC1000_PLL,TX_PLL); // Use TX refdiv setting
  
  // Insert wait routine here, must wait for xtal oscillator to stabilise, 
  // typically takes 2-5ms. 
  for (i=0;i<0x7FFE;i++);
  
  WriteToCC1000Register(CC1000_MAIN,0xF9);  // Turn on bias generator
  // Wait for 250us, insert wait loop here
  WriteToCC1000Register(CC1000_PA_POW,PA_VALUE); // Turn on PA
  WriteToCC1000Register(CC1000_MAIN,0xF1);  // Turn on frequency synthesiser
}

/****************************************************************************/
/*  This routine locks the averaging filter of the CC1000                   */
/****************************************************************************/

void AverageManualLockCC1000(void)
{
  WriteToCC1000Register(CC1000_MODEM1,0x19);
}

/****************************************************************************/
/*  This routine unlocks the averaging filter of the CC1000                 */
/****************************************************************************/

void AverageFreeRunCC1000(void)
{
  WriteToCC1000Register(CC1000_MODEM1,0x09);
}

/****************************************************************************/
/*  This routine sets up the averaging filter of the CC1000 for automatic   */
/*  lock. This can be used in polled receivers.                             */
/****************************************************************************/

void AverageAutoLockCC1000(void)
{
  WriteToCC1000Register(CC1000_MODEM1,0x01);
}

/****************************************************************************/
/*  This routine reads the current calibration values from the CC1000       */
/****************************************************************************/

void ReadCurrentCalibration(unsigned char *val1, unsigned char *val2)
{
  *val1=ReadFromCC1000Register(CC1000_TEST0);
  *val2=ReadFromCC1000Register(CC1000_TEST2);
}

/****************************************************************************/
/*  This routine overrides the current calibration of the CC1000            */
/****************************************************************************/

void OverrideCurrentCalibration(unsigned char val1, unsigned char val2)
{
  WriteToCC1000Register(CC1000_TEST5,(val1&0x0F)|0x10);
  WriteToCC1000Register(CC1000_TEST6,(val2&0x1F)|0x20);
}

/****************************************************************************/
/*  This routine stops override of the CC1000 calibration values            */
/****************************************************************************/

void StopOverridingCalibration(void)
{
  WriteToCC1000Register(CC1000_TEST5,0x00);
  WriteToCC1000Register(CC1000_TEST6,0x00);
}
  

/****************************************************************************/
/*  This CC1000 frequency synthesizer                                       */
/****************************************************************************/
void ResetFreqSynth(void)
{
  char modem1_value;
	
  modem1_value = ReadFromCC1000Register(CC1000_MODEM1)&~0x01;
  WriteToCC1000Register(CC1000_MODEM1,modem1_value);
  WriteToCC1000Register(CC1000_MODEM1,modem1_value|0x01);
}
