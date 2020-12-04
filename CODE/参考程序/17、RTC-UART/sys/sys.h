#ifndef __SYS_H__
#define __SYS_H__

#include <msp430F5438A.h>

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long int

#define CPU_F ((double)24000000UL)                                              //XT2 --> 24MHZ
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))           //—” ±1us
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))              //—” ±1ms

void Init_System();
void Init_Clock();
void Set_Vcore(uint level);

#endif