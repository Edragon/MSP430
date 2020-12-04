#ifndef __LCD_H__
#define __LCD_H__
typedef unsigned short     int u16;
typedef unsigned          char u8;
typedef unsigned           int u32;
#include "../sys/sys.h"
#define White          0xFFFF           //��ʾ��ɫ���룺��
#define Black          0x0000           //��ʾ��ɫ���룺�� 
#define Blue           0x001F           //��ʾ��ɫ���룺��
#define Blue2          0x051F           //��ʾ��ɫ���룺��2
#define Red            0xF800           //��ʾ��ɫ���룺��
#define Magenta        0xF81F           //��ʾ��ɫ���룺Ʒ��
#define Green          0x07E0           //��ʾ��ɫ���룺��
#define Cyan           0x7FFF           //��ʾ��ɫ���룺����ɫ
#define Yellow         0xFFE0           //��ʾ��ɫ���룺��
typedef struct  
{										    
	u16 width;			//LCD ?��?��
	u16 height;			//LCD ???��
	u16 id;				//LCD ID
	u8  dir;			//o��?��?1��?����?��????��o0��?����?����?1��?o��?��?��	
	u16	 wramcmd;		//?a��?D��gram??��?
	u16  setxcmd;		//����??x��?����??��?
	u16  setycmd;		//����??y��?����??��?	 
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