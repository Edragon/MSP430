//接收函数，收到数据后，通过串口显示，串口拨特率设置9600
#include <msp430x14x.h>
//==============================================================================
#define  RF24L01_CE_0        P6OUT &=~BIT2         
#define  RF24L01_CE_1        P6OUT |= BIT2        
//=============================RF24L01_CSN端口==================================
#define  RF24L01_CSN_0       P6OUT &=~BIT3         
#define  RF24L01_CSN_1       P6OUT |= BIT3     
//=============================RF24L01_SCK======================================
#define  RF24L01_SCK_0       P6OUT &=~BIT4      
#define  RF24L01_SCK_1       P6OUT |= BIT4   
//============================= RF24L01_MOSI端口================================
#define  RF24L01_MOSI_0      P6OUT &=~BIT5
#define  RF24L01_MOSI_1      P6OUT |= BIT5
//=============================MISO端口=========================================
#define  RF24L01_MISO_0      P1OUT &=~BIT3
#define  RF24L01_MISO_1      P1OUT |= BIT3
//==========================IRQ状态============================================
#define  RF24L01_IRQ_0       P1OUT &=~BIT4       
#define  RF24L01_IRQ_1       P1OUT |= BIT4
//===================================LCD========================================
#define  DataDir             P4DIR              
#define  DataPort            P4OUT	
//==============================================================================
#define  Busy	             0x80
//==============================================================================
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  32  	// 32 TX payload
#define RX_PLOAD_WIDTH  32  	// 32 uints TX payload
//=========================NRF24L01寄存器指令===================================
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP1            0xFF  	// 保留
//========================SPI(nRF24L01)寄存器地址===============================
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置
//=============================RF24l01状态=====================================
char  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
char  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址
char  sta;
char  RxBuf[32]=
      { 0x01,0x02,0x03,0x4,0x05,0x06,0x07,0x08,
        0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
        0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,
        0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32};
//==============================================================================
void LED_IO_set(void);
void RF24L01_IO_set(void);
void LCD_IO_set();
void ms_delay(void);
void InitSys();
void Delay(int s);
char SPI_RW(char data);
char SPI_Read(char reg);
char SPI_RW_Reg(char reg, char value);
char SPI_Read_Buf(char reg, char *pBuf, char uchars);
char SPI_Write_Buf(char reg, char *pBuf, char uchars);
void SetRX_Mode(void);
char nRF24L01_RxPacket(char* rx_buf);
void nRF24L01_TxPacket(char * tx_buf);
void init_NRF24L01(void);
void init_uart0(void);
void   R_S_Byte(char R_Byte);
//===========================RF24L01端口设置==========================================
void RF24L01_IO_set(void)
{
      P1DIR &= 0xE7;  P1DIR |= 0xE7;  
      P6DIR &= 0xFF;  P6DIR |= 0xFF; 
}

//系统初始化
//******************************************************************************
void InitSys()
{
   unsigned int iq0;
   _DINT();
   BCSCTL1 &=~XT2OFF;
   do
   {
      IFG1 &= ~OFIFG;							// 清除振荡器失效标志
      for (iq0 = 0xFF; iq0 > 0; iq0--);	// 延时，等待XT2起振
   }
   while ((IFG1 & OFIFG) != 0);		// 判断XT2是否起振		
   BCSCTL2 =SELM1+SELS;                // MCLK,SMCLK时钟为XT2	
}
//========================延时约5ms=============================================
void ms_delay(void)
{ 
   unsigned int i=40000;
    while (i != 0)
    {
        i--;
    }
}

//========================================长延时================================
void Delay(int s)
{
	unsigned int i,j;
	for(i=0; i<s; i++);
	for(j=0; j<s; j++);
}
//==============================================================================
//函数：uint SPI_RW(uint uchar)
//功能：NRF24L01的SPI写时序
//******************************************************************************
char SPI_RW(char data)
{	
        char i,temp=0;
   	for(i=0;i<8;i++) // output 8-bit
   	{
	if((data & 0x80)==0x80)
	{
		RF24L01_MOSI_1;         // output 'uchar', MSB to MOSI
	}
	else
	{
	 	RF24L01_MOSI_0; 
	}	
//==============================================================================
		data = (data << 1);            // shift next bit into MSB..
		temp<<=1;
		RF24L01_SCK_1;                // Set SCK high..
		if((P1IN&0x08)==0x08)temp++;         // capture current MISO bit
		RF24L01_SCK_0;              // ..then set SCK low again
   	}
    return(temp);           		  // return read uchar  		
}
//****************************************************************************************************
//函数：uchar SPI_Read(uchar reg)
//功能：NRF24L01的SPI时序
//****************************************************************************************************
char SPI_Read(char reg)
{
	char reg_val;
	RF24L01_CSN_0;           // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	RF24L01_CSN_1;         // CSN high, terminate SPI communication
	return(reg_val);       // return register value
}
//****************************************************************************************************/
//功能：NRF24L01读写寄存器函数
//****************************************************************************************************/
char SPI_RW_Reg(char reg, char value)
{
	char status1;
	RF24L01_CSN_0;                   // CSN low, init SPI transaction
	status1 = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	RF24L01_CSN_1;                   // CSN high again
	return(status1);            // return nRF24L01 status uchar
}
//****************************************************************************************************/
//函数：uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
//功能: 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
//****************************************************************************************************/
char SPI_Read_Buf(char reg, char *pBuf, char chars)
{
	char status2,uchar_ctr;
	RF24L01_CSN_0;                    		// Set CSN low, init SPI tranaction
	status2 = SPI_RW(reg);       		// Select register to write to and read status uchar
	for(uchar_ctr=0;uchar_ctr<chars;uchar_ctr++)
        {
	pBuf[uchar_ctr] = SPI_RW(0);    // 
        }
	RF24L01_CSN_1;                           
	return(status2);   
}
//*********************************************************************************************************
//函数：uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
//功能: 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uchars：写入数据的个数
//*********************************************************************************************************/
char SPI_Write_Buf(char reg, char *pBuf, char chars)
{
	char status1,uchar_ctr;
	RF24L01_CSN_0;             //SPI使能       
	status1 = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<chars; uchar_ctr++) //
        {
	SPI_RW(*pBuf++);
        }
	RF24L01_CSN_1;           //关闭SPI
	return(status1);    		  // 
}
//****************************************************************************************************/
//函数：void SetRX_Mode(void)
//功能：数据接收配置 
//****************************************************************************************************/
void SetRX_Mode(void)
{
	RF24L01_CE_0 ;
        Delay(1);
	RF24L01_CE_1; 
	Delay(1000);//注意不能太小
}
//******************************************************************************************************/
//函数：unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//功能：数据读取后放如rx_buf接收缓冲区中
//******************************************************************************************************/
char nRF24L01_RxPacket(char* rx_buf)
{
    char revale=0;
	sta=SPI_Read(STATUS);	     // 读取状态寄存其来判断数据接收状况		
	if(sta&0x40)                 // 判断是否接收到数据
	{
	    RF24L01_CE_0 ; 			//SPI使能
	    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
	    revale =1;			//读取数据完成标志
	}
        else 
        {
              revale =0;	
        }
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	return revale;
}
//***********************************************************************************************************
//函数：void nRF24L01_TxPacket(char * tx_buf)
//功能：发送 tx_buf中数据
//**********************************************************************************************************/
void nRF24L01_TxPacket(char * tx_buf)
{
	RF24L01_CE_0 ;			//StandBy I模式	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据	
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
	RF24L01_CE_1;		 //置高CE，激发数据发送
	Delay(600);
}
//****************************************************************************************
//NRF24L01初始化
//***************************************************************************************/
void init_NRF24L01(void)
{

 	RF24L01_CE_0 ;    // chip enable
 	RF24L01_CSN_1;   // Spi disable 
 	RF24L01_SCK_0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//设置发射速率为1MHZ，发射功率为最大值0dB
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收}
        Delay(1000);
}
//===============================串口初始化=====================================
void init_uart0(void)
  {
//====================串口工作模式设置========================================
    U0CTL=0x00;      // U0CTL包含串口0通信协议、通信模式、校验位等设置，允许UART0
    U0CTL +=CHAR;    //(CHAR=0x10)当CHAR=0时位7位数据，当 CHAR=1时为8位数据 
                     //不需要校验，数据位为8位，无反馈，异步UART通信，UART被允许
//====================串口发送操作设置========================================
    U0TCTL=0x00;     //U0TCTL包含串口0发送功能操作
    U0TCTL +=SSEL0;  //波特率发生器选择ACLK 
                     //#define SSEL1  (0x20)       #define SSEL0  (0x10)
//====================串口拨特率设置9600====================================
//===================拨特率计算公式：拨特率=BRCLK/（UBR+（M7+M6+。。。。+M0）/8）
    UBR0_0=0x03;     //UBR0_0为串口0波特率整数部分低地址 
    UBR1_0=0x00;     //UBR1_0为串口0波特率整数部分高地址 
    UMCTL_0=0x4A;    //UBR1_0为串口0波特率小数部分高地址 
//===================串口收发使能控制=========================================
    ME1 |= UTXE0;    //串口0发送功能使能
    ME1 |= URXE0;    //串口0接收功能使能    
//===================串口中断使能控制=========================================
//  IE1 |= URXIE0;   //串口0接收中断使能
//   IE1 |= UTXIE0;   //串口0发送中断使能
//===================端口第二功能使能=========================================
    P3SEL |=BIT4;    //设置P3。4为UART0 的TXD
    P3SEL |=BIT5;    //设置P3。5为UART0 的RXD
    P3DIR |=BIT4;    //设置P3。4为输出
  }
//==============================================================================
void   R_S_Byte(char R_Byte)
{
    while((IFG1&UTXIFG0)==0);	//
    TXBUF0=R_Byte;
}

//=============================================================================
main()
{
      char i;
      WDTCTL = WDTPW + WDTHOLD; //禁止看门狗
      RF24L01_IO_set();    
      InitSys();
      init_uart0();
      init_NRF24L01() ;
      while(1)
      {
     SetRX_Mode();
  if(nRF24L01_RxPacket(RxBuf))   //判断是否收到数据
        {
             for(i=0;i<32;i++)        //收到数据后，串口显示
	      {
		R_S_Byte(RxBuf[i]);
		Delay(30);
	      }	
        }
        ms_delay();
      }
}