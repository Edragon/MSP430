#ifndef __LCD_H__
#define __LCD_H__
typedef unsigned short     int u16;
typedef unsigned          char u8;
typedef unsigned           int u32;
#include "../sys/sys.h"
#define White          0xFFFF           //显示颜色代码：白
#define Black          0x0000           //显示颜色代码：黑 
#define Blue           0x001F           //显示颜色代码：蓝
#define Blue2          0x051F           //显示颜色代码：蓝2
#define Red            0xF800           //显示颜色代码：红
#define Magenta        0xF81F           //显示颜色代码：品红
#define Green          0x07E0           //显示颜色代码：绿
#define Cyan           0x7FFF           //显示颜色代码：蓝绿色
#define Yellow         0xFFE0           //显示颜色代码：黄
typedef struct  
{										    
	u16 width;			//LCD ?í?è
	u16 height;			//LCD ???è
	u16 id;				//LCD ID
	u8  dir;			//oá?á?1ê?êú?á????￡o0￡?êú?á￡?1￡?oá?á?￡	
	u16	 wramcmd;		//?aê?D′gram??á?
	u16  setxcmd;		//éè??x×?±ê??á?
	u16  setycmd;		//éè??y×?±ê??á?	 
}_lcd_dev; 	

void Init_LCD();
void Write_Cmd_Data(uint LCD_Cmd, uint LCD_Data);
void Write_Cmd(uint LCD_Cmd);
void Write_Data(uint LCD_Data);
void CLR_Screen(unsigned int bColor);
void LCD_SetPos(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_PutString_Chinese24(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor);
void LCD_PutString_Chinese16(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor);
void LCD_PutChar(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor);
void PutGB2424(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor);
void Put_pixel(unsigned int x,unsigned int y,unsigned int color);
void Put16x16(unsigned short x, unsigned short  y, unsigned char g[2], unsigned int fColor,unsigned int bColor);
void Show_RGB (unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1,unsigned int Color);
void LCD_PutString_English(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor);
void LCD_SetParam(void);
void LCD_WR_REG(u8 Reg);
void LCD_WriteRAM_Prepare(void);
#endif