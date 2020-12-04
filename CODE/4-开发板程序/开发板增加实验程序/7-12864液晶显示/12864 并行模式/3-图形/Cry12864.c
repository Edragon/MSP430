#include <msp430x14x.h>
typedef unsigned char uchar;
typedef unsigned int  uint;

extern const unsigned char shuzi_table[];

#define LCD_DataIn    P4DIR=0x00    //数据口方向设置为输入
#define LCD_DataOut   P4DIR=0xff    //数据口方向设置为输出
#define LCD2MCU_Data  P4IN
#define MCU2LCD_Data  P4OUT
#define LCD_CMDOut    P3DIR|=0x07     //P3口的低三位设置为输出
#define LCD_RS_H      P3OUT|=BIT0      //P3.0
#define LCD_RS_L      P3OUT&=~BIT0     //P3.0
#define LCD_RW_H      P3OUT|=BIT1      //P3.1
#define LCD_RW_L      P3OUT&=~BIT1     //P3.1
#define LCD_EN_H      P3OUT|=BIT2      //P3.2
#define LCD_EN_L      P3OUT&=~BIT2     //P3.2

/*******************************************
函数名称：Delay_1ms
功    能：延时约1ms的时间
参    数：无
返回值  ：无
********************************************/
void Delay_1ms(void)
{
	uchar i;
    
	for(i = 150;i > 0;i--)  _NOP();
} 
/*******************************************
函数名称：Delay_Nms
功    能：延时N个1ms的时间
参    数：n--延时长度
返回值  ：无
********************************************/
void Delay_Nms(uint n)
{
    uint i;
    
    for(i = n;i > 0;i--)    Delay_1ms();
}
/*******************************************
函数名称：Write_Cmd
功    能：向液晶中写控制命令
参    数：cmd--控制命令
返回值  ：无
********************************************/
void Write_Cmd(uchar cmd)
{
    uchar lcdtemp = 0;
			
    LCD_RS_L;
    LCD_RW_H;
    LCD_DataIn;  
    do                       //判忙
    {    
        LCD_EN_H;
       	_NOP();					
       	lcdtemp = LCD2MCU_Data; 
       	LCD_EN_L;
        
    }
    while(lcdtemp & 0x80); 
    
    LCD_DataOut;    
    LCD_RW_L;  		
    MCU2LCD_Data = cmd; 
    LCD_EN_H;
    _NOP();						  			
    LCD_EN_L;
}
/*******************************************
函数名称：Write_Data
功    能：向液晶中写显示数据
参    数：dat--显示数据
返回值  ：无
********************************************/
void  Write_Data(uchar dat)
{
    uchar lcdtemp = 0;   
        
    LCD_RS_L;
    LCD_RW_H;  
    LCD_DataIn;   
    do                       //判忙
    {    
        LCD_EN_H;
        _NOP();						
        lcdtemp = LCD2MCU_Data; 
        LCD_EN_L;      
    }
    while(lcdtemp & 0x80);  
    
    LCD_DataOut; 
    LCD_RS_H;
    LCD_RW_L;  
        
    MCU2LCD_Data = dat;
    LCD_EN_H;
    _NOP();
    LCD_EN_L;
}  
/*******************************************
函数名称：Ini_Lcd
功    能：初始化液晶模块
参    数：无
返回值  ：无
********************************************/
void Ini_Lcd(void)
{                  
    LCD_CMDOut;    //液晶控制端口设置为输出
    
    Delay_Nms(500);
    Write_Cmd(0x30);   //基本指令集
    Delay_1ms();
    Write_Cmd(0x02);   // 地址归位
    Delay_1ms();
	Write_Cmd(0x0c);   //整体显示打开,游标关闭
    Delay_1ms();
	Write_Cmd(0x01);   //清除显示
    Delay_1ms();
	Write_Cmd(0x06);   //游标右移
    Delay_1ms();
	Write_Cmd(0x80);   //设定显示的起始地址
}
/*******************************************
函数名称:Clear_GDRAM
功    能:清除液晶GDRAM中的随机数据
参    数:无
返回值  :无
********************************************/
void Clear_GDRAM(void)
{
    uchar i,j,k;
    
	Write_Cmd(0x34);        //打开扩展指令集
	i = 0x80;            
	for(j = 0;j < 32;j++)
	{
        Write_Cmd(i++);
        Write_Cmd(0x80);
  		for(k = 0;k < 16;k++)
  		{
  		    Write_Data(0x00);
  		}
	}
	i = 0x80;
 	for(j = 0;j < 32;j++)
	{
 	    Write_Cmd(i++);
        Write_Cmd(0x88);	   
  		for(k = 0;k < 16;k++)
  		{
   		    Write_Data(0x00);
   		} 
	}   
	Write_Cmd(0x30);        //回到基本指令集
}
/*******************************************
函数名称:Draw_PM
功    能:在整个液晶屏幕上画图
参    数:无
返回值  :无
********************************************/
void Draw_PM(const uchar *ptr)
{
    uchar i,j,k;
    
	Write_Cmd(0x34);        //打开扩展指令集
	i = 0x80;            
	for(j = 0;j < 32;j++)
	{
        Write_Cmd(i++);
        Write_Cmd(0x80);
  		for(k = 0;k < 16;k++)
  		{
  		    Write_Data(*ptr++);
  		}
	}
	i = 0x80;
 	for(j = 0;j < 32;j++)
	{
 	    Write_Cmd(i++);
        Write_Cmd(0x88);	   
  		for(k = 0;k < 16;k++)
  		{
   		    Write_Data(*ptr++);
   		} 
	}  
    Write_Cmd(0x36);        //打开绘图显示
	Write_Cmd(0x30);        //回到基本指令集
}
