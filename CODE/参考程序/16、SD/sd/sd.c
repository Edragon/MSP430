// ********************************************************
//
//            MSP430F5438A                 MMC Card 
//         -----------------          -----------------
//     /|\|              XIN|-   /|\ |                 |
//      | |                 |     |  |                 |
//      --|RST          XOUT|-    |--|/Vcc             |
//        |                 |        |                 |
//        |                 |        |                 |
//        |            P8.1 |------->|/CS              |
//        |                 |        |                 |
//        |      P3.2/MISO  |<-------|DIN              |
//        |      P3.1/MOSI  |------->|DOUT             |
//        |      P3.3/CLK   |------->|CLK              |
//        |                 |        |                 |

/*
  SD卡初始化流程：
  1、上电后主机应发送至少74个时钟，在这期间应保持CS为高电平，然后SD卡进入idle模式。
  2、发送CMD0命令时，CS为低电平，则SD卡进入SPI模式。CMD0命令没有参数，CMD0的应答R1格式，收到应答为01。
*/

#include "sd.h"
#include <msp430f5438a.h>

#define SD_CS_HIGH    P8OUT |= BIT1
#define SD_CS_LOW     P8OUT &= ~BIT1

#define SD_CLK_HIGH   P3OUT |= BIT3
#define SD_CLK_LOW    P3OUT &= ~BIT3

#define SD_MOSI_HIGH  P3OUT |= BIT1
#define SD_MOSI_LOW   P3OUT &= ~BIT1

#define SD_MISO       P3IN & BIT2

void Init_SPI_Port()
{
  P8OUT |= BIT1 + BIT2;                     // SD_CS 配置失效, Flash_CS 配置失效
  P8DIR |= BIT1 + BIT2;                     // SD_CS、Flash_CS 配置输出
  P3DIR |= BIT1 + BIT3;                     // MOSI、CLK 配置为输出
  
  P3SEL |= BIT1 + BIT2 + BIT3;              // P3.5,4,0 option select
}

void Init_SPI()
{
  Init_SPI_Port();      	            // 初始化SPI端口
  
  UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 0x02;                           // /2
  UCB0BR1 = 0;                              //
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCB0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  __delay_cycles(100);                      // Wait for slave to initialize  
}

unsigned char Init_SD(void)
{
  unsigned char i;
  unsigned char response=0x01;
  
  SD_CS_HIGH;
  for(i=0; i<=9; i++)
    SdWrite(0xff);
  SD_CS_LOW;
  
  //Send Command 0 to put MMC in SPI mode
  SdCommand(0x00,0,0x95);
  response=SdResponse(); 	
  if(response!=0x01)
  {
    return 0;
  } 
  
  while(response==0x01)
  {
    SD_CS_HIGH;
    SdWrite(0xff);
    SD_CS_LOW;
    SdCommand(0x01,0x00ffc000,0xff);
    response=SdResponse();
  } 
  
  SD_CS_HIGH;
  SdWrite(0xff);
  
  return 1; 
}

//写一字节到SD卡,模拟SPI总线方式
unsigned char SdWrite(unsigned char n)
{
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = n;                     // Transmit first character
  while ((UCB0IFG&UCRXIFG) == 0);   // wait for RX buffer (full)
  return UCB0RXBUF;
} 

//发命令到SD卡
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC)
{	
  SdWrite(command|0x40);
  SdWrite(((unsigned char *)&argument)[0]);
  SdWrite(((unsigned char *)&argument)[1]);
  SdWrite(((unsigned char *)&argument)[2]);
  SdWrite(((unsigned char *)&argument)[3]);
  SdWrite(CRC);
}

//检测SD卡的响应，应该范围0x01为正常
unsigned char SdResponse()
{
  unsigned char i=0,response;
  
  while(i<=8)
  {
    response = SdWrite(0xff);
    if(response==0x00)
      break;
    if(response==0x01)
      break;
    i++;
  }
  
  __delay_cycles(5);
  return response;
} 

//往SD卡指定地址写数据,一次最多512字节
unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len)
{
  unsigned int count;
  unsigned char dataResp;
  //Block size is 512 bytes exactly
  //First Lower SS
  
  SD_CS_LOW;
  //Then send write command
  SdCommand(0x18,address,0xff);
  
  if(SdResponse()==00)
  {
    SdWrite(0xff);
    SdWrite(0xff);
    SdWrite(0xff);
    //command was a success - now send data
    //start with DATA TOKEN = 0xFE
    SdWrite(0xfe);
    //now send data
    for(count=0;count<len;count++) SdWrite(*Block++);
    
    for(;count<512;count++) SdWrite(0);
    //data block sent - now send checksum
    SdWrite(0xff); //两字节CRC校验, 为0XFFFF 表示不考虑CRC
    SdWrite(0xff);
    //Now read in the DATA RESPONSE token
    dataResp=SdWrite(0xff);
    //Following the DATA RESPONSE token
    //are a number of BUSY bytes
    //a zero byte indicates the MMC is busy
    
    while(SdWrite(0xff)==0);
    
    dataResp=dataResp&0x0f; //mask the high byte of the DATA RESPONSE token
    SD_CS_HIGH;
    SdWrite(0xff);
    if(dataResp==0x0b)
    {
      //printf("DATA WAS NOT ACCEPTED BY CARD -- CRC ERROR\n");
      return 0;
    }
    if(dataResp==0x05)
      return 1;
    
    //printf("Invalid data Response token.\n");
    return 0;
  }
  //printf("Command 0x18 (Write) was not received by the MMC.\n");
  
  return 0;
}


//从SD卡指定地址读取数据,一次最多512字节
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len)
{
  unsigned int count;
  //Block size is 512 bytes exactly
  //First Lower SS
  
  //printf("MMC_read_block\n");
  
  SD_CS_LOW;
  //Then send write command
  SdCommand(0x11,address,0xff);
  
  if(SdResponse()==00)
  {
    //command was a success - now send data
    //start with DATA TOKEN = 0xFE
    while(SdWrite(0xff)!=0xfe);
    
    for(count=0;count<len;count++) *Block++=SdWrite(0xff); 
    
    for(;count<512;count++) SdWrite(0xff);
    
    //data block sent - now send checksum
    SdWrite(0xff);
    SdWrite(0xff);
    //Now read in the DATA RESPONSE token
    SD_CS_HIGH;
    SdWrite(0xff);
    
    return 1;
  }
  //printf("Command 0x11 (Read) was not received by the MMC.\n");
  return 0;
} 
