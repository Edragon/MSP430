#include <msp430x14x.h>
#include <math.h>
#define CPU_F ((double)1000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsignded int
#define   res1 P4OUT|=BIT6;  //复位,0复位
#define   res0 P4OUT&=~BIT6;
#define   sce1 P4OUT|=BIT5;  //片选
#define   sce0 P4OUT&=~BIT5;  //片选
#define   dc1  P4OUT|=BIT4;  //1写数据，0写指令
#define   dc0  P4OUT&=~BIT4;
#define   sdin1 P4OUT|=BIT3;  //数据
#define   sdin0 P4OUT&=~BIT3;
#define   sclk1 P4OUT|=BIT2;  //时钟
#define   sclk0 P4OUT&=~BIT2;

const unsigned char tuxiang[]={

/*--  宽度x高度=84x48  --*/
0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0x80,0xC0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,
0x00,0x00,0x80,0x80,0xC0,0x60,0x60,0x60,0x30,0xB0,0xB0,0x90,0x90,0x98,0xD8,0x58,
0x48,0x28,0xA8,0x48,0x08,0xD0,0x50,0x50,0xD8,0xD8,0xFC,0xFE,0xFF,0xFF,0xF0,0xC0,
0xC0,0x80,0xE0,0xF8,0xF8,0xFC,0x7E,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x80,0xC0,0x70,0xFC,0xFF,0x07,0xFD,0xFC,0x86,0x87,0x85,0x85,
0x84,0xFC,0xFC,0x00,0x00,0x00,0x00,0x20,0x7E,0xFE,0xA2,0x22,0x23,0xFF,0xFD,0x21,
0x21,0x21,0x21,0x20,0x00,0x00,0xFD,0xFC,0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFC,0xFE,0xFF,0xFC,0xFC,0xFC,0xFC,0xF8,0xF9,0xFF,
0xD4,0x80,0xC3,0xBF,0xFF,0xFF,0xFF,0x7F,0x7F,0x79,0x30,0xF0,0xC0,0xC0,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x00,0x3F,0x3F,
0x10,0x10,0x10,0x10,0x10,0x3F,0x3F,0x00,0x00,0x00,0x10,0x1C,0x0E,0x07,0x0B,0x19,
0x30,0x3F,0x1F,0x00,0x01,0x07,0x1E,0x1C,0x18,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x7D,0xDD,0x91,0x99,0x99,0x9B,0xFF,0xFF,0x7C,0x38,0x70,0xF0,0xC4,
0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xE0,
0x20,0x20,0x20,0xFC,0xFC,0x20,0x20,0x20,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,0x08,
0x08,0x08,0x08,0x08,0x88,0xC8,0x68,0x38,0x18,0x08,0x00,0x00,0x00,0x00,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0xF0,0xF0,0xF3,0xC3,0xCF,0xDF,0xFF,0xFF,0xD0,
0x80,0x00,0x06,0x07,0x0F,0x40,0x03,0x0F,0x70,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x3F,0x3F,0x11,0x11,0x11,0xFF,0xFF,0x11,0x11,0x11,0x3F,0x3F,0xC0,0xC0,
0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0xFF,0xFF,0x04,0x04,0x04,0x04,0x06,0x06,
0x04,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,
0x0F,0x07,0x01,0x01,0x03,0x0F,0x0F,0x1F,0x1F,0x3F,0x3F,0x3F,0x3F,0x7F,0x7F,0xFD,
0xF8,0xE1,0xE1,0x40,0x00,0x13,0x34,0x60,0xE0,0xE0,0x60,0x00,0x00,0x00,0x02,0x08,
0x10,0x20,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,
0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0F,0x3E,0xFF,0xFC,0xFC,0xF1,0xDF,0x9C,0x36,
0x97,0x9D,0xF1,0xF0,0xF8,0x3F,0x07,0x00,

};

/*--------------------------------------------
LCD_write_byte: 使用SPI接口写数据到LCD
输入参数：dt：写入的数据；
command ：写数据/命令选择；
编写日期：20080918 
----------------------------------------------*/
void LCD_write_byte(unsigned char dt, unsigned char command)
{
	unsigned char i; 
	sce0;	
	if(command==1)
        {dc1;	}
        else {dc0;}
	for(i=0;i<8;i++)
	{ 
		if(dt&0x80)
                {sdin1;}
		else
                {sdin0;}
		dt=dt<<1;		
		sclk0; 		
		sclk1; 
	}	
	dc1;	
	sce1;	
	sdin1;
}
/*---------------------------------------
LCD_init: 3310LCD初始化
编写日期：20080918 
-----------------------------------------  */
void LCD_init(void)
{
	res0;  	
  	delay_ms(1);
  	res1;  
	LCD_write_byte(0x21,0);//初始化Lcd,功能设定使用扩充指令
	LCD_write_byte(0xd0,0);//设定液晶偏置电压
	LCD_write_byte(0x20,0);//使用基本指令
	LCD_write_byte(0x0C,0);//设定显示模式，正常显示
}
/*-------------------------------------------
LCD_set_XY: 设置LCD坐标函数
输入参数：X：0－83  Y：0－5
编写日期：20080918 
---------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x40 | Y, 0);// column
	LCD_write_byte(0x80 | X, 0);// row
} 
/*------------------------------------------
LCD_clear: LCD清屏函数
编写日期：20080918 
--------------------------------------------*/
void LCD_clear(void)
{
	unsigned char t;
	unsigned char k;
	LCD_set_XY(0,0);
	for(t=0;t<6;t++)
	{ 
		for(k=0;k<84;k++)
		{ 
			LCD_write_byte(0x00,1);
				
		} 
	}
}

/*---------------------------------------------
LCD_write_hanzi: 显示16（宽）*16（高）点阵列汉字等半角类
输入参数：c：显示的字符；
编写日期：20080918 
-----------------------------------------------*/
void LCD_write_tuxiang() //row:列 page:页 dd:字符
{
	unsigned char i,page,c=0;  	
	for(page=0;page<6;page++)
        {
	  LCD_set_XY(0, page);// 列，页 
	  for(i=0; i<84;i++) 
	  {
	   LCD_write_byte(tuxiang[c*84+i],1); 
	  }
          c++;
        }
}
main()
{	
        WDTCTL = WDTPW + WDTHOLD;
      //  DCOCTL=CALDCO_1MHZ;
      //  BCSCTL1=CALBC1_1MHZ;
        P4DIR=0XFF;
  	res0;
	delay_ms(200);
	res1;	  	
  	LCD_init();  //初始化LCD模块 
	LCD_clear(); //清屏幕	
	LCD_write_tuxiang(); 
         while(1);
}