
/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: main.h,v $
 * Revision 2.1  2003/04/24 12:43:49  tos
 * Cosmetic change: added revision history for CVS.
 *
 *                                                                           *
 ****************************************************************************/


#define UI1 0x33		/* First byte of unique identifier */
#define UI2 0xcc		/* Second byte of unique identifier */

#define HEADER_SIZE 4           /* 4 bytes header */
#define TX_BUFFER_SIZE 64       /* Size (in bytes) of transmit buffer */
#define RX_BUFFER_SIZE 64       /* Size (in bytes) of receive ring-buffer */

#define PREAMBLE_LENGTH 10       /* Number of bytes of preamble to send */

#define PREAMBLE_REQ 6          /* Number of bits required in addition to */
                                /* the initial 8 bits for the preamble to be */
                                /* accepted */
                                
#define FALSE 0
#define TRUE (!FALSE)

/* This struct stores various flags in a space-efficient manner */
//extern volatile struct
//  {
//    unsigned char PreambleFound:1;
//    unsigned char UI1Found:1;
//    unsigned char LastPreambleBit:1;
//    unsigned char LockAverage:1;
//    unsigned char UnlockAverage:1;
//  };


extern enum StateType {IDLE_STATE,RX_STATE,TX_STATE} State;
extern volatile enum StateType NextState;
extern volatile unsigned char TXBuffer[TX_BUFFER_SIZE]; 
extern volatile unsigned char RXBuffer[RX_BUFFER_SIZE];
extern unsigned char ShiftReg;
/*
union {
  unsigned char ShiftReg;
  struct {
    unsigned char ShiftRegLSB :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char ShiftRegMSB :1;
  };
};
*/

extern unsigned char BitCounter;

extern unsigned char TXBufferIndex;            
extern unsigned char RXBufferReadIndex;
extern unsigned char RXBufferWriteIndex;

extern unsigned char BytesToSend;
extern unsigned char BytesToReceive;

//extern unsigned char PreambleCount;
extern unsigned char PreambleError;
extern unsigned char PreambleEnd;
extern unsigned char PreambleNextbit;

extern unsigned char ByteCounter;
extern const unsigned char UnitAddress;
extern unsigned char LockAverage;
extern unsigned char UnlockAverage;
//extern unsigned char EEPROMCC1000Config[33];
