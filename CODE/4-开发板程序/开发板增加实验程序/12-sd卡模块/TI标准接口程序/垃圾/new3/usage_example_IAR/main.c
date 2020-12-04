//*******************************************************************************
//  MSP-MMC Demo - Demo to show how to use the MMC library
//
//  Description; This example shows the correct setup and usage of the MMC
//  Library
//
//
//  S. Schauer
//  Texas Instruments, Inc
//  June 2005
//  Updated for IAR Embedded Workbench Version: 3.31B
//******************************************************************************

/* ***********************************************************
* THIS PROGRAM IS PROVIDED "AS IS". TI MAKES NO WARRANTIES OR
* REPRESENTATIONS, EITHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
* FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR 
* COMPLETENESS OF RESPONSES, RESULTS AND LACK OF NEGLIGENCE. 
* TI DISCLAIMS ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET 
* POSSESSION, AND NON-INFRINGEMENT OF ANY THIRD PARTY 
* INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO THE PROGRAM OR 
* YOUR USE OF THE PROGRAM.
*
* IN NO EVENT SHALL TI BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
* CONSEQUENTIAL OR INDIRECT DAMAGES, HOWEVER CAUSED, ON ANY 
* THEORY OF LIABILITY AND WHETHER OR NOT TI HAS BEEN ADVISED 
* OF THE POSSIBILITY OF SUCH DAMAGES, ARISING IN ANY WAY OUT 
* OF THIS AGREEMENT, THE PROGRAM, OR YOUR USE OF THE PROGRAM. 
* EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF 
* REMOVAL OR REINSTALLATION, COMPUTER TIME, LABOR COSTS, LOSS 
* OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF 
* USE OR INTERRUPTION OF BUSINESS. IN NO EVENT WILL TI'S 
* AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF 
* YOUR USE OF THE PROGRAM EXCEED FIVE HUNDRED DOLLARS 
* (U.S.$500).
*
* Unless otherwise stated, the Program written and copyrighted 
* by Texas Instruments is distributed as "freeware".  You may, 
* only under TI's copyright in the Program, use and modify the 
* Program without any charge or restriction.  You may 
* distribute to third parties, provided that you transfer a 
* copy of this license to the third party and the third party 
* agrees to these terms by its first use of the Program. You 
* must reproduce the copyright notice and any other legend of 
* ownership on each copy or partial copy, of the Program.
*
* You acknowledge and agree that the Program contains 
* copyrighted material, trade secrets and other TI proprietary 
* information and is protected by copyright laws, 
* international copyright treaties, and trade secret laws, as 
* well as other intellectual property laws.  To protect TI's 
* rights in the Program, you agree not to decompile, reverse 
* engineer, disassemble or otherwise translate any object code 
* versions of the Program to a human-readable form.  You agree 
* that in no event will you alter, remove or destroy any 
* copyright notice included in the Program.  TI reserves all 
* rights not specifically granted under this license. Except 
* as specifically provided herein, nothing in this agreement 
* shall be construed as conferring by implication, estoppel, 
* or otherwise, upon you, any license or other right under any 
* TI patents, copyrights or trade secrets.
*
* You may not use the Program in non-TI devices.
* ********************************************************* */



#include <msp430x14x.h>
#include "mmc.h"
#include "fAT16.h"
#include <stdio.h>
#include <string.h>

unsigned long cardSize = 0;
unsigned char status = 1;
unsigned int timeout = 0;
int i = 0;


unsigned char buffer[512];

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;

  //Initialisation of the MMC/SD-card
  while (status != 0)                       // if return in not NULL an error did occur and the
                                            // MMC/SD-card will be initialized again 
  {
    status = initMMC();
    timeout++;
    if (timeout == 50)                      // Try 50 times till error
    {
      printf ("No MMC/SD-card found!! %x\n", status);
      break;
    }
  }

  while ((mmc_ping() != MMC_SUCCESS));      // Wait till card is inserted
//P2DIR=0XFF;
//P2OUT=0X0;while(1);
  // Read the Card Size from the CSD Register
  cardSize =  MMC_ReadCardSize();
  
 //P2DIR=0XFF;
//P2OUT=0X0;while(1); 
  ReadBPB();
  CreateFile("TEST0009TXT");

  //OperateFile
  for (i = 0; i <= 512; i++) buffer[i] = 'A';
  OperateFile(1,"TEST0009TXT",0,512,buffer);
  for (i = 0; i <= 512; i++) buffer[i] = 'B';  
  OperateFile(1,"TEST0009TXT",512,512,buffer);
  for (i = 0; i <= 512; i++) buffer[i] = 'C';  
  OperateFile(1,"TEST0009TXT",1024,512,buffer);
  for (i = 0; i <= 512; i++) buffer[i] = 'D';  
  OperateFile(1,"TEST0009TXT",1536,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'E';  
  OperateFile(1,"TEST0009TXT",2048,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'F';  
  OperateFile(1,"TEST0009TXT",2560,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'G';  
  OperateFile(1,"TEST0009TXT",3072,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'H';  
  OperateFile(1,"TEST0009TXT",3584,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'I';  
  OperateFile(1,"TEST0009TXT",4096,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'J';  
  OperateFile(1,"TEST0009TXT",4608,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'K';  
  OperateFile(1,"TEST0009TXT",5120,5,buffer);  
    
   

  
  //读FAT表
  ReadBlock(FATStartSec());
  
  //读根目录
  ReadBlock(DirStartSec());
  
  //读数据开始区
  ReadBlock(DataStartSec());
    
  //DelFile("TEST0005TXT");

  //读根目录区
  //ReadBlock(DirStartSec());
  
  

  
  mmc_GoIdle();                             // set MMC in Idle mode
  

}
