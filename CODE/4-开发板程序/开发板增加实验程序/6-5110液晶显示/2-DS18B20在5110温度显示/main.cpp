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
#define DQ1 P1OUT|= 0x40     //DS18B20接口为P1.6口
#define DQ0 P1OUT&=~0x40
float Temper;
unsigned int a,b,c;
unsigned char Error=0;
unsigned int i=9,j=4,k=0,l=3,m=4,n=1,p=1;//声明数据类型
unsigned char zimu[]={
/*--  文字:  0  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0xF8,0x04,0x04,0x04,0xF8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  文字:  1  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x08,0xFC,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x00,0x00,

/*--  文字:  2  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x18,0x84,0x44,0x24,0x18,0x00,0x03,0x02,0x02,0x02,0x02,0x00,

/*--  文字:  3  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x08,0x04,0x24,0x24,0xD8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  文字:  4  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x40,0xB0,0x88,0xFC,0x80,0x00,0x00,0x00,0x00,0x03,0x02,0x00,

/*--  文字:  5  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x3C,0x24,0x24,0x24,0xC4,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  文字:  6  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0xF8,0x24,0x24,0x2C,0xC0,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  文字:  7  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x0C,0x04,0xE4,0x1C,0x04,0x00,0x00,0x00,0x03,0x00,0x00,0x00,

/*--  文字:  8  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0xD8,0x24,0x24,0x24,0xD8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  文字:  9  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x38,0x44,0x44,0x44,0xF8,0x00,0x00,0x03,0x02,0x02,0x01,0x00,

/*--  文字:  :  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  .  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0x00,0x00,0x1C,0x22,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=6x16  --*/
0xC0,0x20,0x20,0x40,0x00,0x00,0x03,0x04,0x04,0x02,0x00,0x00,


};
void weishu(float k)
{
  int q=k;
  a=q/10;
  b=q%10;
  float p=k-a*10-b;
  c=p*10;
}

unsigned char hanzi[]=
{
/*--  文字:  温  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x89,0x72,0x00,0xC0,0x5F,0xD5,0x55,0xD5,0x55,0xDF,0x00,0x00,0x07,0x00,0x04,0x07,
0x04,0x07,0x04,0x07,0x04,0x07,0x04,0x00,

/*--  文字:  度  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  高度不是8的倍数，现调整为：宽度x高度=12x16  --*/
0x00,0xFE,0x0A,0x8A,0xBE,0xAA,0xAB,0xAA,0xBE,0x0A,0x0A,0x00,0x06,0x01,0x04,0x04,
0x04,0x03,0x02,0x03,0x04,0x04,0x04,0x00,
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

//=======================================================================================
//============功能：写18B20 =============================================================
//=======================================================================================
void Write_18B20(unsigned char n)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        P1DIR|=0X40;
        DQ0;
        _NOP();_NOP();                           //==延时5us===
        _NOP();_NOP();_NOP();
        if((n&0X01)==0X01) 
        DQ1;
        else DQ0;
        n=n>>1;
       delay_us(60);                             //==延时50us 以上===
        DQ1;
    }
}
//=======================================================================================
//============功能：读取18B20 ===========================================================
//=======================================================================================
unsigned char Read_18B20(void)
{
    unsigned char i;
    unsigned char temp;
    for(i=0;i<8;i++)
    {
        temp=temp>>1;
        P1DIR|=0X40;
        DQ0;
        _NOP();                                  //==延时1us===
        DQ1;
        _NOP();_NOP();                           //==延时5us===
        _NOP();_NOP();_NOP();
        P1DIR&=~0x40;
        if((P1IN&0x40)==0)
        {
            temp=temp&0x7F;
        }else
        {
            temp=temp|0x80;
        }
        delay_us(45);                             //==延时40us===
        P1DIR|=0x40;
        DQ1;
    }
    return    temp;
}
//=======================================================================================
//============DS18B20的初始化  ==========================================================
//=======================================================================================
void Init (void)
{
    P1DIR|=0X40;
    DQ0;
    delay_us(600);                                //==延时500us=================
    DQ1;
    delay_us(50);                                //==延时16～60us==============
    P1DIR&=~0x40;
    if((P1IN&0x40)==0x40)                        //==0001 1111b=1f  ===========
    {
        Error=1;                               //==失败1=====================
        P1DIR|=0x40;
        DQ0;
    }else
    {
        Error=0;                                //==初始化成功================
        P1DIR|=0x40;
        DQ1;
        delay_us(500);
    }
}


//=======================================================================================
//======指令描述:跳过ROM命令,指定代码为CCH,忽略64位ROM地址,直接向DS1820发温度变换命令,===
//              适用于单片机工作.
//=======================================================================================
void Skip(void)
{
    Write_18B20(0xcc);
}
//=======================================================================================
//========== 指令描述:温度转换命令,指定代码为44H.启动DS1820进行温度转换,12位转换时最长===
//                    为750ms(9位为93.75ms).结果存入内部9字节RAM中.
//=======================================================================================
void Convert (void)
{
    Write_18B20(0x44);
}
//=======================================================================================
//================指令描述:读暂存器,指定代码为BEH.读内部RAM中9字节的内容.================
//=======================================================================================
void ReadDo (void)
{
    Write_18B20(0xbe);
}
//=======================================================================================

void ReadTemp (void)
{
    char temp_low,temp_high;                     //== 温度值 =====
    unsigned int temperature;
    temp_low=Read_18B20();                       //== 读低位 =====
    temp_high=Read_18B20();                      //== 读高位 =====
    
    temperature=(temp_high&0x0f);                //=== 屏蔽高4位==
    temperature<<=8;                    //=== 将temp_high部分数据移到temperature高8位===
    temperature|=temp_low;              //=== 将高低两字节内部合并成一个16位数据===
    Temper=temperature*0.0625;          //=== 计算真实温度值===
}

//=======================================================================================
//============获取DS18B20的温度值========================================================
//=======================================================================================
//===  MCU对DS18B20进行温度转换时,其操作必须满足以下过程:
//===  1- 每一次读写之前都要对DS18B20进行复位.
//===  2- 完成复位后发送一条ROM命令到DS18B20.
//===  3- 最后发送一条RAM命令到DS18B20.
void GetTemp(void)
 {
        Init();                  //=== DS1820初始化=== 
        Skip();                  //=== 跳过64位ROM(ROM命令)=== 
        Convert();               //=== 转换(RAM命令)=== 
       // delay_ms(300);         //=== 60000x5us=0.3s=== 
        Init();                  //=== DS1820初始化=== 
        Skip();                  //=== 跳过64位ROM=== 
        ReadDo();                //=== 读暂存器=== 
        ReadTemp();              //=== 读取温度值=== 
 }
 
main()
{	
        WDTCTL = WDTPW + WDTHOLD;
     //   DCOCTL=CALDCO_1MHZ;
     //  BCSCTL1=CALBC1_1MHZ;
        P4DIR=0XFF;
  	res0;
	delay_us(100);
	res1;	  	
  	LCD_init();  //初始化LCD模块 
	LCD_clear(); //清屏幕
        
        LCD_write_shu(0,2,11);
	LCD_write_shu(1,2,11);
        LCD_write_hanzi(2,2,0);//温
        LCD_write_hanzi(4,2,1);//度
        LCD_write_shu(6,2,11);
	LCD_write_shu(7,2,11);
        LCD_write_shu(8,2,0); 
        LCD_write_shu(9,2,0); 
        LCD_write_shu(10,2,12);
        LCD_write_shu(11,2,0);
        LCD_write_shu(12,2,13);
        LCD_write_shu(13,2,14);
        while(1)
        {
          GetTemp();                           
          weishu(Temper);
          LCD_write_shu(8,2,a); 
          LCD_write_shu(9,2,b);
          LCD_write_shu(11,2,c);
        }
       
}
