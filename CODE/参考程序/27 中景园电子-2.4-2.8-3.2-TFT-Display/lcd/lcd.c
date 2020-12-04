/******************************************************************************
MSP430F5438A��СϵͳV4.0

//                TFT2.8��Һ��
//             -----------------

//            |            P9       |-->  LCD ����IO��8λ
//            |            P10      |-->  LCD ����IO��8λ
//            |            LCD_RS   |-->  P8.7
//            |            LCD_CS   |-->  P8.6
//            |            LCD_RD   |-->  P8.5
//            |            LCD_WR   |-->  P8.4
//            |            LCD_RST  |-->  P8.3
//            |            LCD_BL   |-->  P8.0

˵����P9+P10=PE  |-->  LCD ����IO16λ
******************************************************************************/

#include "lcd.h"
#include "GB2424.h"
#include "Ascii_8x16.h"
#include "Chinese.h"
_lcd_dev lcddev;
uint Device_Code = 0x9341;                       //TFT����IC�ͺţ�����2.8��Һ��ΪILI9320

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

//д�Ĵ�������
void LCD_WR_REG(u8 data)
{ 
	LCD_RS_CLR;//д��ַ  
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
//  PEDIR = 0xFFFF;   //LCD ����IO������Ϊ���
        P9OUT = 0XFF;
        P10OUT = 0XFF;
  P8DIR|= BIT0 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;  //LCD ����λ������Ϊ��� 
  
  LCD_BL_SET;       //LCD�����
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
LCD_SetParam();//����??LCD2?��y	

}

//==============================================================================
//	ʵ�ֹ��ܣ�	��LCD_Cmd�Ĵ���д��LCD_Data����
//	���������      LCD_Cmd   ��Ҫ��������� 16λ
//                      LCD_Data  ��Ҫ��������� 16λ
//==============================================================================
void Write_Cmd_Data(uint LCD_Cmd, uint LCD_Data)
{
  LCD_WR_REG(LCD_Cmd);
  Write_Data(LCD_Data);
}

//==============================================================================
//	ʵ�ֹ��ܣ�	д����
//	���������      LCD_Cmd
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
//	ʵ�ֹ��ܣ�	д����
//	���������      LCD_Data
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
//	ʵ�ֹ��ܣ�	TFT����
//	���������	bColor ������ʹ�õı���ɫ
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
//	ʵ�ֹ��ܣ�	��������
//      ���������      x0��y0 ��ʼ����
//                      x1��y1 ��������
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

	LCD_WriteRAM_Prepare();	//?a��?D�䨨?GRAM			
}

/**********************************************************************
��������:��ʾ�ַ���,������Ӣ��ͬʱ��ʾ
���������x ������
y ������
*s ��Ҫ��ʾ���ַ���
fColor �ַ���ɫ
bColor �ַ�������ɫ
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
  LCD_SetPos(x,y,x+8-1,y+16-1);                   //�����ַ���ʾλ��
  for(i=0; i<16;i++) {                            //ѭ��д��16�ֽڣ�һ���ַ�Ϊ16�ֽ�
    unsigned char m=Font8x16[(c-0x20)*16+i];      //��ȡc�ַ��ĵ�i���ֽ���,c��ȥ0x20������Ascii����е�0~1f��ȥ��
    for(j=0;j<8;j++) {                            //ѭ��д��8λ��һ���ֽ�Ϊ8λ
      if((m&0x80)==0x80) {                        //�ж����λ�Ƿ�Ϊ1
        Write_Data(fColor);                       //���λΪ1��д���ַ���ɫ
      }
      else {
        Write_Data(bColor);                       //���λΪ0��д�뱳����ɫ
      }
      m<<=1;                                      //����1λ��׼��д��һλ
    }
  }
}

/**********************************************************************
��������: ��ʾ����24x24
���������x ������
y ������
c ��Ҫ��ʾ�ĺ�����
fColor �ַ���ɫ
bColor �ַ�������ɫ
***********************************************************************/
void PutGB2424(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor)
{
  unsigned int i,j,k;
  
  LCD_SetPos(x,  y,x+24-1, y+24-1);
  
  for (k=0;k<Chinese24_Number;k++)    //Chinese_Number��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����,����Ϊʵ�ʸ��������������
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
//	ʵ�ֹ��ܣ�	����
//  ���������  x��y ��Ҫ��������
//              color �����ɫ
//================================================================================================
void Put_pixel(unsigned int x,unsigned int y,unsigned int color)
{
  LCD_SetPos(x,y,x,y);        //���ô���������
  Write_Data(color);      //��ָ����д����ɫ����
}

//================================================================================================
//ʵ�ֹ��ܣ�	��ʾ��Ӣ���ַ���
//���������  x ������
//y ������
//*s ����ʾ���ַ���,����LCD_PutString(24,16,"123��о",White,Blue);����"123��о"�ĵ�һ���ַ���ַ����ָ�����s.
//bColor �ַ�������ɫ
//================================================================================================
void LCD_PutString_Chinese16(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) 
{
  unsigned char l=0;                            //��ʾ��λ������
  
  while(*s) 
  {
    Put16x16(x+l*8,y,(unsigned char*)s,fColor,bColor);//��ʾ�ú���
    s+=2;l+=2;                                        //��Ϊ����Ϊ����Ϊ2�ֽڣ�ָ���2,��ʾ16x16����λ�ü�2
  }
}

//================================================================================================
//	ʵ�ֹ��ܣ�	��ʾ16x16����
//      ���������      x ������
//                      y ������
//		        g ��Ҫ��ʾ���ַ�����
//		        fColor �ַ���ɫ
//		        bColor �ַ�������ɫ
//================================================================================================
void Put16x16(unsigned short x, unsigned short  y, unsigned char g[2], unsigned int fColor,unsigned int bColor)
{
  unsigned int i,j,k;
  
  LCD_SetPos(x,  x+16-1,y, y+16-1);                       //���ú�����ʾλ��
  
  for (k=0;k<Chinese16_Number;k++)                                      //ѭ��64�Σ���ѯ������ģλ��
  { 
    if ((ch16[k].GBK[0]==g[0])&&(ch16[k].GBK[1]==g[1]))   //�жϵ�k�����ֵı����Ƿ������뺺��g[2]���
    { 
      for(i=0;i<32;i++)                                   //�����ȣ������ҵ�����ʾ��ģλ�ã�ѭ��д��32�ֽ�
      {
        unsigned short m=ch16[k].hz16[i];                 //��ȡ32�ֽ��еĵ�i�ֽ�
        for(j=0;j<8;j++)                                  //ѭ��д��8λ����
        {                                                
          if((m&0x80)==0x80) Write_Data(fColor);     //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
          else              Write_Data(bColor);      //���λΪ0��д�뱳����ɫ
          m<<=1;                                         //����1λ��׼��д��һλ
        } 
      }
    }  
  }	
}

//================================================================================================
//	ʵ�ֹ��ܣ�	ָ��λ����ʾRGB��ɫ
//  ���������  x0��y0 ��ʼ����
//              x1��y1 ��������
//		        Color  ������ɫ
//================================================================================================
void Show_RGB (unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int Color)
{
  unsigned int i,j;
  
  LCD_SetPos(x0,x0,x1,y1);      //������ʾλ��
  for (i=y0;i<=y1;i++)
  {
    for (j=x0;j<=x1;j++)
      LCD_WR_DATA(Color);
  }
}

void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x2C;

	lcddev.dir=0;//����?��				 	 		
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