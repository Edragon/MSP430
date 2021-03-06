#ifndef __LCD_H__
#define __LCD_H__
typedef unsigned short     int u16;
typedef unsigned          char u8;
typedef unsigned           int u32;

#include "../sys/sys.h"

/********************2.8��Һ���ܽŶ��� Start***********************************/
#define LCD_Data_Port           PEOUT                    //P9��P10���PE��Ϊ���ݿ�

#define LCD_RS_CLR	        P8OUT &= ~BIT7           //RS�õ�
#define LCD_RS_SET	        P8OUT |=  BIT7           //RS�ø�

#define LCD_RW_CLR	        P8OUT &= ~BIT4           //RW�õ�
#define LCD_RW_SET	        P8OUT |=  BIT4           //RW�ø�

#define LCD_RD_CLR	        P8OUT &= ~BIT5           //E�õ�
#define LCD_RD_SET	        P8OUT |=  BIT5           //E�ø�

#define LCD_CS_CLR	        P8OUT &= ~BIT6            //CS�õ�
#define LCD_CS_SET	        P8OUT |=  BIT6            //CS�ø�

#define LCD_RST_CLR	        P8OUT &= ~BIT3            //RST�õ�
#define LCD_RST_SET	        P8OUT |=  BIT3            //RST�ø�

#define LCD_BL_CLR	        P8OUT &= ~BIT0            //LE�õ�
#define LCD_BL_SET	        P8OUT |=  BIT0            //LE�ø�
/*********************2.8��Һ���ܽŶ��� END************************************/
#define LCD_W 240
#define LCD_H 320
#define White          0xFFFF           //��ʾ��ɫ���룺��
#define Black          0x0000           //��ʾ��ɫ���룺�� 
#define Blue           0x001F           //��ʾ��ɫ���룺��
#define Blue2          0x051F           //��ʾ��ɫ���룺��2
#define Red            0xF800           //��ʾ��ɫ���룺��
#define Magenta        0xF81F           //��ʾ��ɫ���룺Ʒ��
#define Green          0x07E0           //��ʾ��ɫ���룺��
#define Cyan           0x7FFF           //��ʾ��ɫ���룺����ɫ
#define Yellow         0xFFE0           //��ʾ��ɫ���룺��
	

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

void LCD_Clear(u16 Color);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

#endif