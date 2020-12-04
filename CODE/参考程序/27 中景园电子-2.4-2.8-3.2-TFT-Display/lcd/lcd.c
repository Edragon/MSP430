/******************************************************************************
MSP430F5438A最小系统V4.0

//                TFT2.8寸液晶
//             -----------------

//            |            P9       |-->  LCD 数据IO低8位
//            |            P10      |-->  LCD 数据IO高8位
//            |            LCD_RS   |-->  P8.7
//            |            LCD_CS   |-->  P8.6
//            |            LCD_RD   |-->  P8.5
//            |            LCD_WR   |-->  P8.4
//            |            LCD_RST  |-->  P8.3
//            |            LCD_BL   |-->  P8.0

说明：P9+P10=PE  |-->  LCD 数据IO16位
******************************************************************************/

#include "lcd.h"
#include "GB2424.h"
#include "Ascii_8x16.h"
#include "Chinese.h"
_lcd_dev lcddev;
uint Device_Code = 0x9341;                       //TFT控制IC型号，配套2.8寸液晶为ILI9320

void DATAOUT(unsigned short int X)
{
    P9DIR = 0xFF;
    P10DIR = 0XFF;
    P9OUT = X;
    P10OUT = (X >> 8);  
}
unsigned short int  DATAIN(void)
{
    unsigned short int datainH;
    P9DIR = 0X00;
    P10DIR = 0X00;
    datainH = P10IN;
    return (datainH << 8 | P9IN);
}

//写寄存器函数
void LCD_WR_REG(u8 data)
{ 
	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
         //LCD_Data_Port = data;
	LCD_RW_CLR; 
	LCD_RW_SET; 
 	LCD_CS_SET;   
}

void LCD_WR_DATA(data)
{ 
LCD_RS_SET;
LCD_CS_CLR;
  LCD_Data_Port = data;
LCD_RW_CLR;
LCD_RW_SET;
LCD_CS_SET;

}




void Init_LCD_PORT()
{
//  PEDIR = 0xFFFF;   //LCD 数据IO，设置为输出
        P9OUT = 0XFF;
        P10OUT = 0XFF;
  P8DIR|= BIT0 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;  //LCD 控制位，设置为输出 
  
  LCD_BL_SET;       //LCD背光打开
}

void Init_LCD()
{  
  Init_LCD_PORT();
  LCD_RD_SET;
  LCD_RST_SET;    
  Delay_ms(1);                    // Delay 1ms 
  LCD_RST_CLR;  
  Delay_ms(10);                   // Delay 10ms            
  LCD_RST_SET;  
  Delay_ms(50);                   // Delay 50 ms  
  LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC1); 
	LCD_WR_DATA(0X30); 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x08); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
  Delay_ms(50);
	LCD_WR_REG(0x29); //display on	
LCD_SetParam();//éè??LCD2?êy	

}

//==============================================================================
//	实现功能：	向LCD_Cmd寄存器写入LCD_Data数据
//	输入参数：      LCD_Cmd   需要输入的命令 16位
//                      LCD_Data  需要输入的数据 16位
//==============================================================================
void Write_Cmd_Data(uint LCD_Cmd, uint LCD_Data)
{
  LCD_WR_REG(LCD_Cmd);
  Write_Data(LCD_Data);
}

//==============================================================================
//	实现功能：	写命令
//	输入参数：      LCD_Cmd
//==============================================================================
void Write_Cmd(uint LCD_Cmd)
{
  LCD_CS_CLR;
  LCD_RS_CLR;
  LCD_RD_SET;
  LCD_RW_CLR;
  LCD_Data_Port = LCD_Cmd;
  LCD_RW_SET;
  LCD_CS_SET;
}

//==============================================================================
//	实现功能：	写数据
//	输入参数：      LCD_Data
//==============================================================================
void Write_Data(uint LCD_Data)
{
  LCD_CS_CLR;
  LCD_RS_SET;
  LCD_Data_Port = LCD_Data;
  LCD_RW_CLR;
  LCD_RW_SET;
  LCD_CS_SET;
}

//================================================================================================
//	实现功能：	TFT清屏
//	输入参数：	bColor 清屏所使用的背景色
//================================================================================================
void CLR_Screen(unsigned int bColor)
{
  unsigned int i,j;
  
  LCD_SetPos(0,0,240,320);//320x240
  for (i=0;i<320;i++)
  {
    for (j=0;j<240;j++)
      LCD_WR_DATA(bColor);
  }
}
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }

	  }
}
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x2a);
   LCD_WR_DATA(x1>>8);
   LCD_WR_DATA(x1);
   LCD_WR_DATA(x2>>8);
   LCD_WR_DATA(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA(y1>>8);
   LCD_WR_DATA(y1);
   LCD_WR_DATA(y2>>8);
   LCD_WR_DATA(y2);

   LCD_WR_REG(0x2C);					 						 
}
//================================================================================================
//	实现功能：	设置坐标
//      输入参数：      x0，y0 起始坐标
//                      x1，y1 结束坐标
//================================================================================================
void LCD_SetPos(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//?aê?D′è?GRAM			
}

/**********************************************************************
函数功能:显示字符串,可以中英文同时显示
输入参数：x 横坐标
y 纵坐标
*s 需要显示的字符串
fColor 字符颜色
bColor 字符背景颜色
***********************************************************************/

void LCD_PutString_Chinese24(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) 
{
  uchar l=0;
  
  while(*s) 
  {
    PutGB2424(x+l*12,y,(unsigned char*)(s),fColor,bColor);
    s+=2;l+=2;
  }  
}

void LCD_PutString_English(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) 
{
  unsigned char l=0;
  
  while(*s) 
  {
    LCD_PutChar(x+l*8,y,*s,fColor,bColor);
    s++;
    l++;
  }
}


void LCD_PutChar(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor)
{
  unsigned int i,j;
  LCD_SetPos(x,y,x+8-1,y+16-1);                   //设置字符显示位置
  for(i=0; i<16;i++) {                            //循环写入16字节，一个字符为16字节
    unsigned char m=Font8x16[(c-0x20)*16+i];      //提取c字符的第i个字节以,c减去0x20是由于Ascii码库中的0~1f被去掉
    for(j=0;j<8;j++) {                            //循环写入8位，一个字节为8位
      if((m&0x80)==0x80) {                        //判断最高位是否为1
        Write_Data(fColor);                       //最高位为1，写入字符颜色
      }
      else {
        Write_Data(bColor);                       //最高位为0，写入背景颜色
      }
      m<<=1;                                      //左移1位，准备写下一位
    }
  }
}

/**********************************************************************
函数功能: 显示汉字24x24
输入参数：x 横坐标
y 纵坐标
c 需要显示的汉字码
fColor 字符颜色
bColor 字符背景颜色
***********************************************************************/
void PutGB2424(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor)
{
  unsigned int i,j,k;
  
  LCD_SetPos(x,  y,x+24-1, y+24-1);
  
  for (k=0;k<Chinese24_Number;k++)    //Chinese_Number表示自建汉字库中的个数，循环查询内码,必须为实际个数，否则会乱码
  { 
    if ((codeGB_24[k].Index[0]==c[0])&&(codeGB_24[k].Index[1]==c[1]))
    { 
      for(i=0;i<72;i++) 
      {
        unsigned short m=codeGB_24[k].Msk[i];
        for(j=0;j<8;j++) 
        {
          if((m&0x80)==0x80) 
          {
            Write_Data(fColor);
          }
          else 
          {
            Write_Data(bColor);
          }
          m<<=1;
        } 
      }
    }  
  }	
}

//================================================================================================
//	实现功能：	画点
//  输入参数：  x，y 需要画点坐标
//              color 点的颜色
//================================================================================================
void Put_pixel(unsigned int x,unsigned int y,unsigned int color)
{
  LCD_SetPos(x,y,x,y);        //设置待画点坐标
  Write_Data(color);      //在指定点写入颜色数据
}

//================================================================================================
//实现功能：	显示中英文字符串
//输入参数：  x 横坐标
//y 纵坐标
//*s 待显示的字符串,例如LCD_PutString(24,16,"123蓝芯",White,Blue);即把"123蓝芯"的第一个字符地址赋给指针变量s.
//bColor 字符背景颜色
//================================================================================================
void LCD_PutString_Chinese16(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) 
{
  unsigned char l=0;                            //显示屏位置增量
  
  while(*s) 
  {
    Put16x16(x+l*8,y,(unsigned char*)s,fColor,bColor);//显示该汉字
    s+=2;l+=2;                                        //因为汉字为编码为2字节，指针加2,显示16x16所以位置加2
  }
}

//================================================================================================
//	实现功能：	显示16x16汉字
//      输入参数：      x 横坐标
//                      y 纵坐标
//		        g 需要显示的字符编码
//		        fColor 字符颜色
//		        bColor 字符背景颜色
//================================================================================================
void Put16x16(unsigned short x, unsigned short  y, unsigned char g[2], unsigned int fColor,unsigned int bColor)
{
  unsigned int i,j,k;
  
  LCD_SetPos(x,  x+16-1,y, y+16-1);                       //设置汉字显示位置
  
  for (k=0;k<Chinese16_Number;k++)                                      //循环64次，查询汉字字模位置
  { 
    if ((ch16[k].GBK[0]==g[0])&&(ch16[k].GBK[1]==g[1]))   //判断第k个汉字的编码是否与输入汉字g[2]相等
    { 
      for(i=0;i<32;i++)                                   //如果相等，既已找到待显示字模位置，循环写入32字节
      {
        unsigned short m=ch16[k].hz16[i];                 //读取32字节中的第i字节
        for(j=0;j<8;j++)                                  //循环写入8位数据
        {                                                
          if((m&0x80)==0x80) Write_Data(fColor);     //判断最高位是否为1,最高位为1，写入字符颜色
          else              Write_Data(bColor);      //最高位为0，写入背景颜色
          m<<=1;                                         //左移1位，准备写下一位
        } 
      }
    }  
  }	
}

//================================================================================================
//	实现功能：	指定位置显示RGB颜色
//  输入参数：  x0，y0 起始坐标
//              x1，y1 结束坐标
//		        Color  背景颜色
//================================================================================================
void Show_RGB (unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int Color)
{
  unsigned int i,j;
  
  LCD_SetPos(x0,x0,x1,y1);      //设置显示位置
  for (i=y0;i<=y1;i++)
  {
    for (j=x0;j<=x1;j++)
      LCD_WR_DATA(Color);
  }
}

void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x2C;

	lcddev.dir=0;//êú?á				 	 		
	lcddev.width=240;
	lcddev.height=320;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;	
//LCD_WriteReg(0x36,0xC9);

}	

void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}