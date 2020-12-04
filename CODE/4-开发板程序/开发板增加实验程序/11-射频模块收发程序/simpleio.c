/****************************************************************************/
/* Application note AN015                                                   */
/* Reference design : CC1000 RF Modem                                       */
/*                                                                          */
/* File:      simpleio.c                                              */
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
/* This file contains routines for simple output of strings, integers and   */
/* hexadecimal values. Using these routines instead of printf() saves large */
/* amounts of program memory.                                               */
/****************************************************************************/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
说明：这个文件里的代码是Chipcon提供在PIC单片机上尽心串口通信的函数，
      这里是上层代码不需要做任何变更即可直接应用了，我们只是移植了
      getchar()和putchar()两个最底层的驱动函数
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "mstdio.h"
#include <stdlib.h>
#include <ctype.h>
#include "simpleio.h"

/****************************************************************************/
/* This routine outputs a string                                            */
/****************************************************************************/

void writestr(const unsigned char *str)
{
  while (*str!='\0')
  {
    putchar(*str++);
  }
}

/****************************************************************************/
/* This routine outputs a string, and also outputs trailing new-line and    */
/* carrage return characters                                                */
/****************************************************************************/

void writeln(const unsigned char *str)
{
  while (*str!='\0') 
  {
    putchar(*str++);
  }
  putchar('\n');
  putchar('\r');
}

/****************************************************************************/
/* This routine converts an ASCII hexadecimal digit to the corresponding    */
/* integer value                                                            */
/****************************************************************************/

unsigned char hexdigit2int(unsigned char val)
{
  if ((val>=0x30)&&(val<=0x39))
    return val-0x30;
  if ((toupper(val)>=0x41)&&(toupper(val)<=0x46))
    return val-0x41+10;
    
  return 0x00;
}

/****************************************************************************/
/* This routine outputs a long value in decimal                             */
/****************************************************************************/


void writelong(long longval)
{
  unsigned char i;
  long divider;
  
  divider=1000000000;
  
  for (i=10;i>0;i--) 
  {
    putchar((longval/divider)+0x30);
    divider/=10;
  }
    
}

/****************************************************************************/
/* This routine outputs an int (in hexadecimal)                             */
/****************************************************************************/

void writehex(unsigned int hexval)
{
  unsigned char i;
  
  int temp;
  
  unsigned char val;
  
  putchar('0');
  putchar('x');
  
  temp=hexval;
  
  for(i=0;i<4;i++) 
  {
    val=(temp&0xF000)>>12;
    if (val<=9) 
    {
      putchar(val+0x30);
    } 
    else 
    {
      putchar(val+0x41-0xA);
    }
    temp=(temp<<4);
  }
}   


