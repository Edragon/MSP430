#ifndef _I2C_H
#define _I2C_H

#define SDA_1       P5OUT|=0x04               //SDA = 1
#define SDA_0       P5OUT&=~0x04              //SDA = 0
#define SCL_1       P5OUT|=0x02               //SCL = 1
#define SCL_0       P5OUT&=~0x02              //SCL = 0
#define SDA_IN      P5DIR&=~0x04             //I/O口为输入
#define SDA_OUT     P5DIR|=0x04               //I/0口为输出
#define J_SDA_1_or_0   (P5IN&0x04)==0x04
//--------------------------------------------
//--------24CXX读写子函数
//DELAY 函数程序//功能:延时子程序
//==========================================
static void Delay(unsigned int n)
{
while(n!=0)
{n--;}
}
//=========================================
//START 函数程序
//功能:开始闪存操作
//==========================================
void Start(void)
{
  SDA_OUT;
  SDA_1;
  Delay(10);
  SCL_1;
  Delay(10);
  SDA_0;
  Delay(10);
  SCL_0;
  Delay(10);
}

//=========================================
//STOP 函数程序
//功能:停止闪存操作
//==========================================
void Stop(void)
{
  SDA_0;
  Delay(10);
  SCL_1;
  Delay(10);
  SDA_1;
  Delay(10);
}



void TX_Byte(unsigned char WriteData)
{
  unsigned char i,j;
  j=WriteData;
  SDA_OUT;
  for (i=0; i<8; i++)
  {
    if ((j&0x80)==0)
    { SDA_0;}
    else
    { SDA_1;}
    j <<=1;
    Delay(10);
    SCL_1;
    Delay(10);
    SCL_0;
    Delay(10);
  }
}



unsigned char RX_Byte(void)
{
  unsigned char i;
  unsigned char TempData = 0;
  SDA_IN;
  for (i=0; i<8; i++)
  {
    Delay(10);
    SCL_1;
    Delay(10);
    TempData <<= 1;
    if (J_SDA_1_or_0)
    { TempData++; }
    SCL_0;
    Delay(10);
  }
  SDA_OUT;
  return(TempData);
}
//=========================================
//ACK 函数程序
//功能:挂高电平表示闪存时钟信号
//==========================================
unsigned char ReceiveAck(void)
{
  SDA_IN;
  SCL_1;
  Delay(10);
  while (J_SDA_1_or_0)
  {  }
  SCL_0;
  SDA_OUT;
  Delay(10);
  return 1;
}



unsigned char ReadWord(unsigned long int unit/*address*/)
{
  unsigned char LowAdd  = 0;
  unsigned char HighAdd = 0;
  unsigned char  TempData = 0;
  LowAdd  = (unsigned char)unit;
  HighAdd = (unsigned char)(unit >> 8);
  SDA_OUT;
  Start();
  TX_Byte(0xa0);
  ReceiveAck();
  TX_Byte(HighAdd);
  ReceiveAck();
  TX_Byte(LowAdd);
  ReceiveAck();
  Start();
  TX_Byte(0xa1);
  ReceiveAck();
  TempData  = RX_Byte();  
  Stop();
  Delay(8000);  
  return(TempData);
}


unsigned char WriteWord(unsigned long int unit/*address*/, unsigned char WriteData)
{
  unsigned char LowAdd  = 0;
  unsigned char HighAdd = 0;
  LowAdd  = (unsigned char)unit;
  HighAdd = (unsigned char)(unit >> 8);
  Start();
  TX_Byte(0xa0);
  ReceiveAck();
  TX_Byte(HighAdd);
  ReceiveAck();
  TX_Byte(LowAdd);
  ReceiveAck();
  TX_Byte(WriteData);
  ReceiveAck();
  Stop();
  Delay(8000);
  return 1;
}
#endif
