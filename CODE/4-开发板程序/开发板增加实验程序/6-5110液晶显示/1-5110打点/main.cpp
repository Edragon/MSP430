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

unsigned char zimu[]={

/*--  文字:  a  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x40,0xA0,0xA0,0xC0,0x00,0x00,0x01,0x02,0x02,0x03,0x02,

/*--  文字:  b  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x04,0xFC,0x20,0x20,0xC0,0x00,0x00,0x03,0x02,0x02,0x01,0x00,

/*--  文字:  c  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0xC0,0x20,0x20,0x60,0x00,0x00,0x01,0x02,0x02,0x02,0x00,

/*--  文字:  d  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0xC0,0x20,0x24,0xFC,0x00,0x00,0x01,0x02,0x02,0x03,0x02,

/*--  文字:  e  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0xC0,0xA0,0xA0,0xC0,0x00,0x00,0x01,0x02,0x02,0x02,0x00,

/*--  文字:  f  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x20,0xF8,0x24,0x24,0x04,0x00,0x02,0x03,0x02,0x02,0x00,

/*--  文字:  g  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x40,0xA0,0xA0,0x60,0x20,0x00,0x07,0x0A,0x0A,0x0A,0x04,

/*--  文字:  h  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x04,0xFC,0x20,0x20,0xC0,0x00,0x02,0x03,0x02,0x00,0x03,0x02,

/*--  文字:  i  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x20,0xE4,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x00,0x00,

/*--  文字:  j  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x00,0x20,0xE4,0x00,0x00,0x08,0x08,0x08,0x07,0x00,0x00,

/*--  文字:  k  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x04,0xFC,0x80,0xE0,0x20,0x20,0x02,0x03,0x02,0x00,0x03,0x02,

/*--  文字:  l  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x04,0x04,0xFC,0x00,0x00,0x00,0x02,0x02,0x03,0x02,0x02,0x00,

/*--  文字:  m  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0xE0,0x20,0xE0,0x20,0xC0,0x00,0x03,0x00,0x03,0x00,0x03,0x00,

/*--  文字:  n  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x20,0xE0,0x20,0x20,0xC0,0x00,0x02,0x03,0x02,0x00,0x03,0x02,

/*--  文字:  o  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0xC0,0x20,0x20,0xC0,0x00,0x00,0x01,0x02,0x02,0x01,0x00,

/*--  文字:  p  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x20,0xE0,0x20,0x20,0xC0,0x00,0x08,0x0F,0x0A,0x02,0x01,0x00,

/*--  文字:  q  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0xC0,0x20,0x20,0xE0,0x00,0x00,0x01,0x02,0x0A,0x0F,0x08,

/*--  文字:  r  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x20,0xE0,0x40,0x20,0x20,0x00,0x02,0x03,0x02,0x00,0x00,0x00,

/*--  文字:  s  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x60,0xA0,0xA0,0x20,0x00,0x00,0x02,0x02,0x02,0x03,0x00,

/*--  文字:  t  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x20,0xF8,0x20,0x00,0x00,0x00,0x00,0x01,0x02,0x02,0x00,

/*--  文字:  u  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x20,0xE0,0x00,0x20,0xE0,0x00,0x00,0x01,0x02,0x02,0x03,0x02,

/*--  文字:  v  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x20,0xE0,0x20,0x80,0x60,0x20,0x00,0x00,0x03,0x01,0x00,0x00,

/*--  文字:  w  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x60,0x80,0xE0,0x80,0x60,0x00,0x00,0x03,0x00,0x03,0x00,0x00,

/*--  文字:  x  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x20,0x60,0x80,0x60,0x20,0x00,0x02,0x03,0x00,0x03,0x02,0x00,

/*--  文字:  y  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x20,0xE0,0x20,0x80,0x60,0x20,0x08,0x08,0x07,0x01,0x00,0x00,

/*--  文字:  z  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x20,0xA0,0x60,0x20,0x00,0x00,0x02,0x03,0x02,0x02,0x00,

/*--  文字:  .  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,



	
};

unsigned char maohao[]={
/*--  文字:  :  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00


};
unsigned char jiaohao[]={

/*--  文字:  + - --48*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x10,0x10,0x7C,0x10,0x10,0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x00,

/*--  文字:  +  --49*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x10,0x10,0x7C,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  -  --50*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x00
};

unsigned char hanzi[]=
{
  /*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  江  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x09,0xD2,0x38,0x02,0x02,0x02,0xFE,0x02,0x02,0x02,0x00,0x00,0x01,0x07,0x04,0x04,
0x04,0x04,0x07,0x04,0x04,0x04,0x04,0x00,

/*--  文字:  苏  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x02,0x92,0x52,0x17,0x92,0x7A,0x12,0x17,0xF2,0x42,0x82,0x00,0x00,0x04,0x04,0x02,
0x01,0x04,0x04,0x06,0x01,0x00,0x01,0x00,

/*--  文字:  科  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x8A,0x6A,0xFE,0x29,0x49,0x80,0x92,0xA4,0x80,0xFF,0x40,0x00,0x01,0x00,0x07,0x00,
0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,

/*--  文字:  技  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x44,0x44,0xFF,0x24,0x00,0x64,0xA4,0x3F,0xA4,0x64,0x04,0x00,0x04,0x04,0x07,0x00,
0x04,0x04,0x02,0x01,0x02,0x04,0x04,0x00,

/*--  文字:  大  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x08,0x08,0x08,0x08,0xC8,0x3F,0xC8,0x08,0x08,0x0C,0x08,0x00,0x04,0x04,0x02,0x01,
0x00,0x00,0x00,0x01,0x02,0x04,0x04,0x00,

/*--  文字:  学  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x9C,0x84,0x95,0x96,0x95,0xD6,0xB4,0x94,0x87,0x94,0x8C,0x00,0x00,0x00,0x00,0x04,
0x04,0x07,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  张  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x79,0x49,0x49,0xCF,0x20,0xFF,0x30,0xE8,0x24,0x23,0x20,0x00,0x04,0x04,0x06,0x01,
0x00,0x07,0x04,0x02,0x01,0x02,0x04,0x00,

/*--  文字:  家  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x44,0x46,0x2A,0x9A,0x4A,0xBB,0xCA,0x4A,0xAA,0x12,0x06,0x00,0x04,0x05,0x03,0x02,
0x05,0x04,0x07,0x00,0x00,0x01,0x02,0x00,

/*--  文字:  港  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x89,0x72,0x80,0x52,0xF2,0x5F,0x52,0x52,0xFF,0x52,0x92,0x00,0x07,0x00,0x00,0x00,
0x07,0x05,0x05,0x05,0x05,0x04,0x06,0x00,

/*--  文字:  校  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x44,0x24,0xFF,0x14,0x20,0x54,0x8D,0x06,0xEC,0x14,0x24,0x00,0x00,0x00,0x07,0x04,
0x04,0x04,0x02,0x01,0x02,0x04,0x04,0x00,

/*--  文字:  区  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0xFF,0x01,0x01,0x09,0x91,0x61,0x51,0x8D,0x01,0x01,0x00,0x00,0x07,0x04,0x05,0x05,
0x04,0x04,0x04,0x04,0x05,0x04,0x04,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
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
LCD_write_shu: 显示8（宽）*16（高）点阵列数字字母符号等半角类
输入参数：c：显示的字符；
编写日期：20080918 
-----------------------------------------------*/
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c) //row:列 page:页 dd:字符
{
	unsigned char i;  	
	
	LCD_set_XY(row*6, page);// 列，页 
	for(i=0; i<6;i++) 
	{
		LCD_write_byte(zimu[c*12+i],1); 
	}
	
    LCD_set_XY(row*6, page+1);// 列，页 
	for(i=6; i<12;i++) 
	{
		LCD_write_byte(zimu[c*12+i],1);
	}	 	
}
/*---------------------------------------------
LCD_write_hanzi: 显示12（宽）*16（高）点阵列汉字等半角类
输入参数：c：显示的字符；
编写日期：20080918 
-----------------------------------------------*/
void LCD_write_hanzi(unsigned char row, unsigned char page,unsigned char c) //row:列 page:页 dd:字符
{
	unsigned char i;  	
	
	LCD_set_XY(row*6, page);// 列，页 
	for(i=0; i<12;i++) 
	{
		LCD_write_byte(hanzi[c*24+i],1); 
	}
	
    LCD_set_XY(row*6, page+1);// 列，页 
	for(i=12; i<24;i++) 
	{
	LCD_write_byte(hanzi[c*24+i],1);
	}	
}
/********打点********/
void dadian(unsigned int x,unsigned int y)
{
  unsigned int y1,y2,y3,y4=1,i;
  y1=y/8;y2=1+y-8*y1;y3=5-y1;
  for(i=0;i<(8-y2);i++)
  {
    y4*=2;
  }
  LCD_set_XY(x,y3);
  LCD_write_byte(y4,1);
}
/********************************************************
* 名称：Line()
      采用布兰森汉姆(Bresenham)算法画线
* 功能：任意两点间的直线。根据硬件特点，实现加速。
* 入口参数：x0       直线起点所在行的位置
*         y0       直线起点所在列的位置
*         x1     直线终点所在行的位置
‘       y1     直线终点所在列的位置
* 出口参数：   无
* 说明：操作失败原因是指定地址超出缓冲区范围。
*********************************************************/
void Line(double x0,double y0,double x1,double y1)
{
int temp;
int dx,dy;               //定义起点到终点的横、纵坐标增加值
int s1,s2,status,i;
int Dx,Dy,sub;

dx=x1-x0;
if(dx>=0)                 //X的方向是增加的
  s1=1;
else                     //X的方向是降低的
  s1=-1;     
dy=y1-y0;                 //判断Y的方向是增加还是降到的
if(dy>=0)
  s2=1;
else
  s2=-1;
  
Dx=fabs(x1-x0);             //计算横、纵标志增加值的绝对值
Dy=fabs(y1-y0);
if(Dy>Dx)                 //               
  {                     //以45度角为分界线，靠进Y轴是status=1,靠近X轴是status=0 
  temp=Dx;
  Dx=Dy;
  Dy=temp;
  status=1;
  } 
else
  status=0;

/********判断垂直线和水平线********/
if(dx==0)                   //横向上没有增量，画一条水平线
  Line(x0,y0,y1,1);
if(dy==0)                   //纵向上没有增量，画一条垂直线
  Line(x0,y0,x1,1);


/*********Bresenham算法画任意两点间的直线********/ 
  sub=2*Dy-Dx;                 //第1次判断下个点的位置
  for(i=0;i<Dx;i++)
  { 
    dadian(x0,y0);           //画点 
    if(sub>=0)                               
    { 
    if(status==1)               //在靠近Y轴区，x值加1
      x0+=s1; 
    else                     //在靠近X轴区，y值加1               
      y0+=s2; 
    sub-=2*Dx;                 //判断下下个点的位置 
    } 
    if(status==1)
  y0+=s2; 
    else       
    x0+=s1; 
    sub+=2*Dy; 
        
  } 
}
/*****************************************
名称：Draw_circle (在任意位置画圆)
说明：使用Bresenham法画1/8个圆，在用对称性画出其他的7/8个圆 
    

    按下图把圆分为8份
        0
      7     1
    6       2
      5     3
        4 
*****************************************/
void Draw_circle(uchar x0,uchar y0,uchar r)
{
int a,b;
int di;
a=0;
b=r;
di=3-2*r;             //判断下个点位置的标志
while(a<=b)
{
  dadian(x0-b,y0-a);             //3           
  dadian(x0+b,y0-a);             //0           
  dadian(x0-a,y0+b);             //1       
  dadian(x0-b,y0-a);             //7           
  dadian(x0-a,y0-b);             //2             
  dadian(x0+b,y0+a);             //4               
  dadian(x0+a,y0-b);             //5
  dadian(x0+a,y0+b);             //6 
  dadian(x0-b,y0+a);             
  a++;
  /***使用Bresenham算法画圆**/     
  if(di<0)
  di +=4*a+6;
  else
  {
    di +=10+4*(a-b);   
    b--;
  } 
  dadian(x0+a,y0+b);
  }
}

void sin()
{
  double j;
  for(j=0;j<84;j++)
  {
    dadian(j,23*sin(6.28*j/84)+24);
    delay_ms(100);
  }
}
main()
{	
        WDTCTL = WDTPW + WDTHOLD;
       
        //DCOCTL=CALDCO_1MHZ;
        //BCSCTL1=CALBC1_1MHZ;
        P4DIR=0XFF;
  	res0;
	delay_us(100);
	res1;	  	
  	LCD_init();  //初始化LCD模块 
	LCD_clear(); //清屏幕
	/*LCD_write_shu(2,4,9);//j
        LCD_write_shu(3,4,20);//u
        LCD_write_shu(4,4,18);//s
        LCD_write_shu(5,4,19);//t
        LCD_write_shu(6,4,26);//.
        LCD_write_shu(7,4,4);//e
        LCD_write_shu(8,4,3);//d
        LCD_write_shu(9,4,20);//u
        LCD_write_shu(10,4,26);//.
        LCD_write_shu(11,4,2);//c
        LCD_write_shu(12,4,13);//n*/
        while(1)
        {
          //dadian(24,24);
          Line(0,0,83,47);
          Line(0,47,83,0);
          delay_ms(1500);
          LCD_clear();
          Draw_circle(41,23,7);
          Draw_circle(41,23,15);
          Draw_circle(41,23,23);
          delay_ms(1500);
          LCD_clear();
          sin();
          delay_ms(1500);
          LCD_clear();
          
        }
}