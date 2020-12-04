
#define HEADER_SIZE 4           // 4 bytes header 

// The wrap-around functionality has been optimised by ANDing with a bit mask. 
// Please note that if RX_BUFFER_SIZE is to be changed, the lines of code which
// do this must also be changed. They are located in the main loop of the program
// and in 

#define TX_BUFFER_SIZE 64       // Size (in bytes) of transmit buffer
#define RX_BUFFER_SIZE 64       // Size (in bytes) of receive ring-buffer

#define PREAMBLE_LENGTH 10       // Number of bytes of preamble to send */
#define PREAMBLE_REQ 6          /* Number of bits required in addition to */
                                /* the initial 8 bits for the preamble to be */
                                /* accepted */

#define UI1     0x33		/* First byte of unique identifier */
#define UI2     0xCC		/* Second byte of unique identifier */

#define FALSE   0
#define TRUE    1

#define ON      TRUE
#define OFF     FALSE


// Macros for turning on and off the LEDs
#define SET_RXLED(STATE) \
  if (STATE)    P2OUT &= ~BIT0;  \
  else          P2OUT |= BIT0        

#define SET_TXLED(STATE) \
  if (STATE)    P2OUT &= ~BIT7;  \
  else          P2OUT |= BIT7        
    
//定义键盘按键
#define BUTTON1 (P1IN & BIT0)
#define BUTTON2 (P1IN & BIT1)
#define BUTTON3 (P1IN & BIT2)
#define BUTTON4 (P1IN & BIT3)


// 模块单元地址
extern const unsigned char UnitAddress;

// 发送和接收的移位变量
unsigned char ShiftReg;

// Buffers for transmitted and received data
// These are put into different banks so that they can be as large as possible
// The TX buffer is filled up with data to be sent in the next data packet
// The RX buffer is a ring buffer in which received data is stored waiting to be
// sent to the UART                                
volatile  unsigned char TXBuffer[TX_BUFFER_SIZE]; 
volatile  unsigned char RXBuffer[RX_BUFFER_SIZE];
// Index pointers for use with buffers 
 unsigned char TXBufferIndex;            
 unsigned char RXBufferReadIndex;
 unsigned char RXBufferWriteIndex;

// Counter variables
unsigned char PreambleEnd;
unsigned char PreambleNextbit;
unsigned char ByteCounter;
unsigned char BitCounter;

// Contains the total number of bytes to send in TX, including preamble and header
unsigned char BytesToSend;
// The number of bytes of data to receive in RX
unsigned char BytesToReceive;

// State variable stores the current state of the state machine
enum StateType {IDLE_STATE=0,RX_STATE=1,TX_STATE=2} State;

// This variable stores the state to switch to
// It is updated by the interrupt routine, while the main program
// does the actual state switch
volatile enum StateType NextState;

unsigned char LockAverage;
unsigned char UnlockAverage;
