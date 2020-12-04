/*

硬件原理图：  DS1302_SCLK --> P7.5       
              DS1302_IO   --> P7.6
              DS1302_RST  --> P7.7

*/


#include "ds1302.h"
#include "../sys/sys.h"
#include "../uart/uart.h"


//复位脚
#define RST_CLR	P7OUT &= ~BIT7    //电平置低
#define RST_SET	P7OUT |=  BIT7    //电平置高

//双向数据
#define DS1302_IO_OUT P7DIR |= BIT6
#define DS1302_IO_IN  P7DIR &= ~BIT6
#define IO_CLR	P7OUT &= ~BIT6    //电平置低
#define IO_SET	P7OUT |=  BIT6    //电平置高
#define IO_R	P7IN  &  BIT6     //电平读取

//时钟信号
#define SCK_CLR	P7OUT &= ~BIT5    //时钟信号
#define SCK_SET	P7OUT |=  BIT5    //电平置高


#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

extern unsigned char time_buf1[];
unsigned char time_buf[8] ;                         //空年月日时分秒周

extern uchar Uart_Array[];              //接收缓存数组
extern int   Uart_Array_Num;   			//接收数据长度
extern uchar Key_Value[];     
extern uint  Key_Value_Num;      

uchar DS1302_Time[20];

/*------------------------------------------------
向DS1302写入时钟数据
------------------------------------------------*/
void Ds1302_Write_Time(void) 
{
  Ds1302_Write_Byte(ds1302_control_add,0x00);			//关闭写保护 
  Ds1302_Write_Byte(ds1302_sec_add,0x80);				//暂停 
  Ds1302_Write_Byte(ds1302_charger_add,0xa9);		//涓流充电 
  
  Ds1302_Write_Byte(ds1302_year_add,time_buf1[1]);		//年 
  Ds1302_Write_Byte(ds1302_month_add,time_buf1[2]);	//月 
  Ds1302_Write_Byte(ds1302_date_add,time_buf1[3]);		//日 
  Ds1302_Write_Byte(ds1302_day_add,time_buf1[7]);		//周 
  Ds1302_Write_Byte(ds1302_hr_add,time_buf1[4]);		//时 
  Ds1302_Write_Byte(ds1302_min_add,time_buf1[5]);		//分
  Ds1302_Write_Byte(ds1302_sec_add,time_buf1[6]);		//秒
  Ds1302_Write_Byte(ds1302_day_add,time_buf1[7]);		//周 
  Ds1302_Write_Byte(ds1302_control_add,0x80);			//打开写保护 
  
  Send_DS1302();
}

/*------------------------------------------------
从DS1302读出时钟数据
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
  time_buf[1]=Ds1302_Read_Byte(ds1302_year_add);		//年 
  time_buf[2]=Ds1302_Read_Byte(ds1302_month_add);		//月 
  time_buf[3]=Ds1302_Read_Byte(ds1302_date_add);		//日 
  time_buf[4]=Ds1302_Read_Byte(ds1302_hr_add);		//时 
  time_buf[5]=Ds1302_Read_Byte(ds1302_min_add);		//分 
  time_buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;//秒 
  time_buf[7]=Ds1302_Read_Byte(ds1302_day_add);		//周 
}

void Init_DS1302_Port()
{
  P7DIR |= BIT5 + BIT6 + BIT7;
}

/*------------------------------------------------
DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void)
{
  Init_DS1302_Port();
  
  RST_CLR;			//RST脚置低
  SCK_CLR;			//SCK脚置低
  Ds1302_Write_Byte(ds1302_sec_add,0x00);				 
}

/**************************************************
函数: Send_DS1302
描述: 串口显示当前DS1302时间
**************************************************/
void Send_DS1302(void)
{ 	
  Ds1302_Read_Time();  //读取时间    
  
  DS1302_Time[0] = '2';   //分离出年千位
  DS1302_Time[1] = '0'; //分离出年百位 
  DS1302_Time[2] = (time_buf[1]>>4) + '0';   //分离出年十位
  DS1302_Time[3] = (time_buf[1]&0x0F) + '0'; //分离出年个位 
  DS1302_Time[4] = '-';
  DS1302_Time[5] = (time_buf[2]>>4) + '0';   //分离出月十位
  DS1302_Time[6] = (time_buf[2]&0x0F) + '0'; //分离出月个位 
  DS1302_Time[7] = '-';
  DS1302_Time[8] = (time_buf[3]>>4) + '0';   //分离出日十位
  DS1302_Time[9] = (time_buf[3]&0x0F) + '0'; //分离出日个位 
  DS1302_Time[10] = ' ';
  DS1302_Time[11] = (time_buf[4]>>4) + '0';   //分离出小时十位
  DS1302_Time[12] = (time_buf[4]&0x0F) + '0'; //分离出小时个位 
  DS1302_Time[13] = ':';
  DS1302_Time[14] = (time_buf[5]>>4) + '0';   //分离出分钟十位
  DS1302_Time[15] = (time_buf[5]&0x0F) + '0'; //分离出分钟个位
  DS1302_Time[16] = ' ';
  DS1302_Time[17] = (time_buf[6]>>4) + '0';   //分离出分钟十位
  DS1302_Time[18] = (time_buf[6]&0x0F) + '0'; //分离出分钟个位
  DS1302_Time[19] = '\0';
  
  UART_SendString(DS1302_Time);
  UART_SendString("\r\n");
}


/*------------------------------------------------
向DS1302写入一字节数据
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{
  
  unsigned char i;
  RST_SET;	
  
  //写入目标地址：addr
  addr = addr & 0xFE;     //最低位置零
  for (i = 0; i < 8; i ++) 
  { 
    if (addr & 0x01) 
    {
      IO_SET;
    }
    else 
    {
      IO_CLR;
    }
    SCK_SET;
    SCK_CLR;
    addr = addr >> 1;
  }
  
  //写入数据：d
  for (i = 0; i < 8; i ++) 
  {
    if (d & 0x01) 
    {
      IO_SET;
    }
    else 
    {
      IO_CLR;
    }
    SCK_SET;
    SCK_CLR;
    d = d >> 1;
  }
  RST_CLR;					//停止DS1302总线
}

/*------------------------------------------------
从DS1302读出一字节数据
------------------------------------------------*/
unsigned char Ds1302_Read_Byte(unsigned char addr) 
{
  
  unsigned char i;
  unsigned char temp;
  RST_SET;	
  
  //写入目标地址：addr
  addr = addr | 0x01;//最低位置高
  for (i = 0; i < 8; i ++) 
  {
    if (addr & 0x01) 
    {
      IO_SET;
    }
    else 
    {
      IO_CLR;
    }
    SCK_SET;
    SCK_CLR;
    addr = addr >> 1;
  }
  
  DS1302_IO_IN;
  //输出数据：temp
  for (i = 0; i < 8; i ++) 
  {
    temp = temp >> 1;
    if (IO_R) 
    {
      temp |= 0x80;
    }
    else 
    {
      temp &= 0x7F;
    }
    SCK_SET;
    SCK_CLR;
  }
  DS1302_IO_OUT;
  
  RST_CLR;	//停止DS1302总线
  return temp;
}