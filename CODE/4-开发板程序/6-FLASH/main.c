//****************************************************************************
//  MSP-FET430P140 Demo - Flash In-System Programming, Copy SegA to SegB
//
//  Description: This program first erases flash seg A, then it increments all
//  values in seg A, then it erases seg B, then  copies seg A to seg B.
//  Assumed MCLK 550kHz - 900kHz.
//  //* Set Breakpoint on NOP in the Mainloop to avoid Stressing Flash *//
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//
//  M. Mitchell
//  Texas Instruments Inc.
//  Feb 2005
//  Built with IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include  <msp430x14x.h>
#define uchar unsigned char
#define uint  unsigned int
uchar  value;                                // 8-bit value to write to segment A
uchar  DataBuffer[128];

// Function prototypes
void  write_SegA (uchar value);
void  copy_A2B (void);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  FCTL2 = FWKEY + FSSEL0 + FN0;             // MCLK/2 for Flash Timing Generator
  value = 0;                                // Initialize value

  while(1)                                  // Repeat forever
  {
    write_SegA(value++);                    // Write segment A, increment value
    copy_A2B();                             // Copy segment A to B
    _NOP();                                 // SET BREAKPOINT HERE
  }
}

void write_SegA (uchar value)
{
  uchar *Flash_ptr;                          // Flash pointer
  uint i;

  Flash_ptr = (uchar *) 0x1080;              // Initialize Flash pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash segment

  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i=0; i<128; i++)
  {
    *Flash_ptr++ = value;                   // Write value to flash
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}


void copy_A2B (void)
{
  uchar *Flash_ptrA;                         // Segment A pointer
  uchar *Flash_ptrB;                         // Segment B pointer
  uint i;

  Flash_ptrA = (uchar *) 0x1080;             // Initialize Flash segment A pointer
  Flash_ptrB = (uchar *) 0x1000;             // Initialize Flash segment B pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *Flash_ptrB = 0;                          // Dummy write to erase Flash segment B
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i=0; i<128; i++)
  {
    DataBuffer[i] = *Flash_ptrA++;
    *Flash_ptrB++ = DataBuffer[i];           // Copy value segment A to segment B
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}
