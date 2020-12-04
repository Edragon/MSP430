#include <msp430x14x.h>
#define CPU_F ((double)1000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define   res1 P4OUT|=BIT6;  //��λ,0��λ
#define   res0 P4OUT&=~BIT6;
#define   sce1 P4OUT|=BIT5;  //Ƭѡ
#define   sce0 P4OUT&=~BIT5;  //Ƭѡ
#define   dc1  P4OUT|=BIT4;  //1д���ݣ�0дָ��
#define   dc0  P4OUT&=~BIT4;
#define   sdin1 P4OUT|=BIT3;  //����
#define   sdin0 P4OUT&=~BIT3;
#define   sclk1 P4OUT|=BIT2;  //ʱ��
#define   sclk0 P4OUT&=~BIT2;
#define DQ1 P1OUT|= BIT6    //DS18B20�ӿ�ΪP1.6��
#define DQ0 P1OUT&=~BIT6
float Temper;
unsigned int a,b,c;
//float a,b,c;
unsigned char Error=0;
unsigned int x[8],y[8];
unsigned int sec,min,hr,date,month,year,day;
unsigned int sec1,sec2,min1,min2,hr1,hr2,date1,date2,month1,month2,year1,year2;
unsigned char zimu[]={
/*--  ����:  0  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0xF8,0x04,0x04,0x04,0xF8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  ����:  1  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x00,0x08,0xFC,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x00,0x00,

/*--  ����:  2  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x18,0x84,0x44,0x24,0x18,0x00,0x03,0x02,0x02,0x02,0x02,0x00,

/*--  ����:  3  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x08,0x04,0x24,0x24,0xD8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  ����:  4  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x40,0xB0,0x88,0xFC,0x80,0x00,0x00,0x00,0x00,0x03,0x02,0x00,

/*--  ����:  5  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x3C,0x24,0x24,0x24,0xC4,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  ����:  6  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0xF8,0x24,0x24,0x2C,0xC0,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  ����:  7  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x0C,0x04,0xE4,0x1C,0x04,0x00,0x00,0x00,0x03,0x00,0x00,0x00,

/*--  ����:  8  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0xD8,0x24,0x24,0x24,0xD8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,

/*--  ����:  9  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x38,0x44,0x44,0x44,0xF8,0x00,0x00,0x03,0x02,0x02,0x01,0x00,

/*--  ����:  :  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,

/*--  ����:     --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  .  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,

/*--  ����:     --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0x00,0x00,0x1C,0x22,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:     --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=6x16  --*/
0xC0,0x20,0x20,0x40,0x00,0x00,0x03,0x04,0x04,0x02,0x00,0x00,

};

unsigned char hanzi[]=
{
  /*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x89,0x72,0x00,0xC0,0x5F,0xD5,0x55,0xD5,0x55,0xDF,0x00,0x00,0x07,0x00,0x04,0x07,
0x04,0x07,0x04,0x07,0x04,0x07,0x04,0x00,

/*--  ����:  һ  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x30,0x20,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x06,0x04,0x00,0x00,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x00,0x02,0x22,0x22,0x22,0x22,0x22,0x22,0x23,0x02,0x00,0x00,0x04,0x04,0x04,0x04,
0x04,0x04,0x04,0x04,0x04,0x06,0x04,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x00,0xFF,0x81,0x41,0x3F,0x01,0x01,0xFF,0x81,0x81,0xFF,0x00,0x00,0x07,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x07,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x00,0x11,0x11,0x91,0x7F,0x11,0x11,0x11,0xF1,0x01,0x00,0x00,0x04,0x04,0x04,0x07,
0x04,0x04,0x04,0x04,0x07,0x04,0x04,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x08,0x08,0x88,0x68,0x09,0x0E,0x28,0x48,0x88,0x08,0x08,0x00,0x04,0x02,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x06,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x00,0x00,0xFF,0x11,0x11,0x11,0x11,0x11,0x11,0xFF,0x00,0x00,0x00,0x00,0x07,0x02,
0x02,0x02,0x02,0x02,0x02,0x07,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x88,0x84,0xF3,0x92,0x92,0x92,0xFE,0x92,0x92,0x92,0x82,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x00,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,0x00,0x00,0x04,0x02,0x01,
0x00,0x00,0x04,0x04,0x07,0x00,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x00,0x00,0xFF,0x11,0x11,0x11,0x11,0x11,0x11,0xFF,0x00,0x00,0x00,0x00,0x07,0x02,
0x02,0x02,0x02,0x02,0x02,0x07,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x00,0x80,0x7F,0x55,0x55,0xF5,0x55,0x55,0x55,0x5F,0x00,0x00,0x05,0x04,0x05,0x05,
0x05,0x07,0x05,0x05,0x05,0x05,0x04,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x82,0xFF,0xAA,0xAA,0xFF,0x82,0x00,0xFF,0x49,0x49,0xFF,0x00,0x04,0x02,0x01,0x00,
0x01,0x02,0x04,0x03,0x00,0x04,0x07,0x00,

/*--  ����:  ��  --*/
/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=12x12   --*/
/*--  �߶Ȳ���8�ı������ֵ���Ϊ�����x�߶�=12x16  --*/
0x00,0xFE,0x0A,0x8A,0xBE,0xAA,0xAB,0xAA,0xBE,0x0A,0x0A,0x00,0x06,0x01,0x04,0x04,
0x04,0x03,0x02,0x03,0x04,0x04,0x04,0x00,
};

/*--------------------------------------------
LCD_write_byte: ʹ��SPI�ӿ�д���ݵ�LCD
���������dt��д������ݣ�
command ��д����/����ѡ��
��д���ڣ�20080918 
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
LCD_init: 3310LCD��ʼ��
��д���ڣ�20080918 
-----------------------------------------  */
void LCD_init(void)
{
	res0;  	
  	delay_ms(1);
  	res1;  
	LCD_write_byte(0x21,0);//��ʼ��Lcd,�����趨ʹ������ָ��
	LCD_write_byte(0xd0,0);//�趨Һ��ƫ�õ�ѹ
	LCD_write_byte(0x20,0);//ʹ�û���ָ��
	LCD_write_byte(0x0C,0);//�趨��ʾģʽ��������ʾ
}
/*-------------------------------------------
LCD_set_XY: ����LCD���꺯��
���������X��0��83  Y��0��5
��д���ڣ�20080918 
---------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x40 | Y, 0);// column
	LCD_write_byte(0x80 | X, 0);// row
} 
/*------------------------------------------
LCD_clear: LCD��������
��д���ڣ�20080918 
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
LCD_write_shu: ��ʾ8����*16���ߣ�������������ĸ���ŵȰ����
���������c����ʾ���ַ���
��д���ڣ�20080918 
-----------------------------------------------*/
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c) //row:�� page:ҳ dd:�ַ�
{
	unsigned char i;  	
	
	LCD_set_XY(row*6, page);// �У�ҳ 
	for(i=0; i<6;i++) 
	{
		LCD_write_byte(zimu[c*12+i],1); 
	}
	
    LCD_set_XY(row*6, page+1);// �У�ҳ 
	for(i=6; i<12;i++) 
	{
		LCD_write_byte(zimu[c*12+i],1);
	}	 	
}
/*---------------------------------------------
LCD_write_hanzi: ��ʾ12����*16���ߣ������к��ֵȰ����
���������c����ʾ���ַ���
��д���ڣ�20080918 
-----------------------------------------------*/
void LCD_write_hanzi(unsigned char row, unsigned char page,unsigned char c) //row:�� page:ҳ dd:�ַ�
{
	unsigned char i;  	
	
	LCD_set_XY(row*6, page);// �У�ҳ 
	for(i=0; i<12;i++) 
	{
		LCD_write_byte(hanzi[c*24+i],1); 
	}
	
    LCD_set_XY(row*6, page+1);// �У�ҳ 
	for(i=12; i<24;i++) 
	{
		LCD_write_byte(hanzi[c*24+i],1);
	}	
}

void  init_io()
{
  //P4DIR |=BIT2;                        //DS1302 CS
  //P4OUT &=~BIT2;       
  //P4DIR |=BIT0;                        //CLOCK
  //P4OUT &=~BIT0;
  //P4DIR |=BIT1;                        //I/O
  //P4OUT &=~BIT1;
   P2DIR |=BIT7;                        //DS1302 CS
   P2OUT &=~BIT7;       
   P2DIR |=BIT5;                        //CLOCK
   P2OUT &=~BIT5;
   P2DIR |=BIT6;                        //I/O
   P2OUT &=~BIT6;
}
void set_r()                            //��λ
{ 
   P2OUT |=BIT7;
}
void clr_r()                            //��λ
{
  P2OUT &=~BIT7;
}
 void set_clk()
{ 
   P2OUT|=BIT5;
}
void clr_clk()
{
  P2OUT&=~BIT5;
} 
 void set_data()
{ 
   P2OUT|=BIT6;
}
void clr_data()
{
  P2OUT&=~BIT6;
}  
//////////////////////////////////////////////////
void changedata(unsigned char data)                    //ת�����ݸ�ʽ
{for(unsigned int i=0;i<=7;i++)
{x[i]=data%2;  data=data/2;}  
}
/////////////////////////////////////////////////
void send_com(unsigned char com)        //����ָ��
{  changedata(com);
   clr_clk();
   clr_data();
   for(int j=0 ;j<=7;j++)
    {  set_r();
       clr_clk(); 
       if(x[j]) 
         set_data();
       else
         clr_data();
       set_clk();
    }
}    
void send_data(unsigned char data)     //��������
{
  changedata(data);
  for(  int j=0 ;j<=7    ; j++   )
  {  clr_clk();
     if(x[j]) 
       set_data();
     else
       clr_data();
     set_clk();
    
     for(int i=1000;i>=0;i--);
   }
  clr_clk();clr_r();
}

////////////////////////////////
void  read_data()              //��������
{   P2DIR&=~BIT6;           //p4.5  ����
    for(  int j=0 ;j<=7    ; j++   ) 
      {   clr_clk(); 
          if(P2IN&BIT6)
           y[j]=1;
          else
           y[j]=0;
          set_clk();
       }
    P2DIR|=BIT6;
    clr_r();
}
void wp_on()                     //д��������
{ 
  send_com(0x8e);
  send_data(0x80);
}
void wp_off()                    //д������ֹ
{
  send_com(0x8e);
  send_data(0x00);
}
//////////////////////////////////////////////
void write_sec(unsigned int sec)       //д��
{   
  wp_off();
  
  send_com(0x80);
  send_data(sec);
  wp_on();
}
 unsigned int read_sec()     //����
{
  send_com(0x81);
  read_data();
 unsigned int a,b; 
     a=y[0]+y[1]*2+y[2]*4+y[3]*8;
     b=y[4]+y[5]*2+y[6]*4;
     sec=b*10+a;  
  return sec;
}
////////////////////////////////////////////
void write_min(unsigned int min)                  //д��
{ wp_off();
  
  send_com(0x82);
  send_data(min);
  wp_on();
}
unsigned int read_min()              //����
{
  send_com(0x83);
  read_data();
 unsigned int a,b; 
     a=y[0]+y[1]*2+y[2]*4+y[3]*8;
     b=y[4]+y[5]*2+y[6]*4;
     min=b*10+a;  
  return min;
} 
//////////////////////////////////////////
void write_hr(unsigned int hr)                   // дСʱ
{ wp_off();
  
  send_com(0x84);
  send_data(hr);
  wp_on();
}
unsigned int read_hr()             //��Сʱ
{
  send_com(0x85);
  read_data();
 unsigned int a,b; 
     a=y[0]+y[1]*2+y[2]*4+y[3]*8;
     b=y[4]+y[5]*2;
     hr=b*10+a;  
  return hr;
} 
////////////////////////////////////////
void write_date(unsigned int date)               //д����
{ wp_off();
  
  send_com(0x86);
  send_data(date);
  wp_on();
}
unsigned int read_date()            //������
{
  send_com(0x87);
  read_data();
 unsigned int a,b; 
     a=y[0]+y[1]*2+y[2]*4+y[3]*8;
     b=y[4]+y[5]*2;
     date=b*10+a;  
  return date;
} 
//////////////////////////////////////////
void write_month(unsigned int month)               //д��
{ wp_off();
  
  send_com(0x88);
  send_data(month);
  wp_on();
}
unsigned int read_month()            //����
{
  send_com(0x89);
  read_data();
 unsigned int a,b; 
     a=y[0]+y[1]*2+y[2]*4+y[3]*8;
     b=y[4];
     month=b*10+a;  
  return month;
} 
////////////////////////////////////////////////////
void write_day(unsigned int day)               //д����
{ wp_off();
  
  send_com(0x8a);
  send_data(day);
  wp_on();
}
unsigned int read_day()            //������
{
  send_com(0x8b);
  read_data();
 unsigned int a; 
   a=y[0]+y[1]*2+y[2]*4;//+y[3]*8;
   //  b=y[4]+y[5]*2+y[6]*4+y[7]*8;
  day=a;
  return day;
} 

///////////////////////////////////////
void write_year(unsigned int year)               //д��
{ wp_off();
  
  send_com(0x8c);
  send_data(year);
  wp_on();
}
unsigned int read_year()            //����
{
  send_com(0x8d);
  read_data();
 unsigned int a,b; 
     a=y[0]+y[1]*2+y[2]*4+y[3]*8;
     b=y[4]+y[5]*2+y[6]*4+y[7]*8;
     year=b*10+a;  
  return year;
} 

void shijianjishuan()
{
  year1=year/10;
  year2=year%10;
  date1=date/10;
  date2=date%10;
  month1=month/10;
  month2=month%10;
  hr1=hr/10;
  hr2=hr%10;
  min1=min/10;
  min2=min%10;
  sec1=sec/10;
  sec2=sec%10;
}
void read_all()
{
  sec=read_sec();
  min=read_min();
  hr=read_hr();
  date=read_date();
  month=read_month();
  day=read_day();
  year=read_year();
  shijianjishuan();
}
void weishu(float k)
{
  int q=k;
  a=q/10;
  b=q%10;
  float p=k-a*10-b;
  c=p*10;
}

//=======================================================================================
//============���ܣ�д18B20 =============================================================
//=======================================================================================
void Write_18B20(unsigned char n)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        DQ0;
        _NOP();_NOP();                           //==��ʱ5us===
        _NOP();_NOP();_NOP();
        if((n&0X01)==0X01) 
        DQ1;
        else DQ0;
        n=n>>1;
       delay_us(60);                             //==��ʱ50us ����===
        DQ1;
    }
}
//=======================================================================================
//============���ܣ���ȡ18B20 ===========================================================
//=======================================================================================
unsigned char Read_18B20(void)
{
    unsigned char i;
    unsigned char temp;
    for(i=0;i<8;i++)
    {
        temp=temp>>1;
        DQ0;
        _NOP();                                  //==��ʱ1us===
        DQ1;
        _NOP();_NOP();                           //==��ʱ5us===
        _NOP();_NOP();_NOP();
        P1DIR&=~BIT6;
        if((P1IN&BIT6)==0)
        {
            temp=temp&0x7F;
        }else
        {
            temp=temp|0x80;
        }
        delay_us(45);                             //==��ʱ40us===
        P1DIR|=BIT6;
        DQ1;
    }
    return    temp;
}
//=======================================================================================
//============DS18B20�ĳ�ʼ��  ==========================================================
//=======================================================================================
void Init (void)
{
    DQ0;
    delay_us(600);                                //==��ʱ500us=================
    DQ1;
    delay_us(50);                                //==��ʱ16��60us==============
    P1DIR&=~BIT6;
    if((P1IN&BIT6)==0x40)                        //==0001 1111b=1f  ===========
    {
        Error=1;                               //==ʧ��1=====================
        P1DIR|=BIT6;
        DQ0;
    }else
    {
        Error=0;                                //==��ʼ���ɹ�================
        P1DIR|=BIT6;
        DQ1;
        delay_us(500);
    }
}


//=======================================================================================
//======ָ������:����ROM����,ָ������ΪCCH,����64λROM��ַ,ֱ����DS1820���¶ȱ任����,===
//              �����ڵ�Ƭ������.
//=======================================================================================
void Skip(void)
{
    Write_18B20(0xcc);
}
//=======================================================================================
//========== ָ������:�¶�ת������,ָ������Ϊ44H.����DS1820�����¶�ת��,12λת��ʱ�===
//                    Ϊ750ms(9λΪ93.75ms).��������ڲ�9�ֽ�RAM��.
//=======================================================================================
void Convert (void)
{
    Write_18B20(0x44);
}
//=======================================================================================
//================ָ������:���ݴ���,ָ������ΪBEH.���ڲ�RAM��9�ֽڵ�����.================
//=======================================================================================
void ReadDo (void)
{
    Write_18B20(0xbe);
}
//=======================================================================================

void ReadTemp (void)
{
    char temp_low,temp_high;                     //== �¶�ֵ =====
    unsigned int temperature;
    temp_low=Read_18B20();                       //== ����λ =====
    temp_high=Read_18B20();                      //== ����λ =====
    
    temperature=(temp_high&0x0f);                //=== ���θ�4λ==
    temperature<<=8;                    //=== ��temp_high���������Ƶ�temperature��8λ===
    temperature|=temp_low;              //=== ���ߵ����ֽ��ڲ��ϲ���һ��16λ����===
    Temper=temperature*0.0625;          //=== ������ʵ�¶�ֵ===
}

//=======================================================================================
//============��ȡDS18B20���¶�ֵ========================================================
//=======================================================================================
//===  MCU��DS18B20�����¶�ת��ʱ,����������������¹���:
//===  1- ÿһ�ζ�д֮ǰ��Ҫ��DS18B20���и�λ.
//===  2- ��ɸ�λ����һ��ROM���DS18B20.
//===  3- �����һ��RAM���DS18B20.
void GetTemp(void)
 {
        Init();                  //=== DS1820��ʼ��=== 
        Skip();                  //=== ����64λROM(ROM����)=== 
        Convert();               //=== ת��(RAM����)=== 
       // delay_ms(300);         //=== 60000x5us=0.3s=== 
        Init();                  //=== DS1820��ʼ��=== 
        Skip();                  //=== ����64λROM=== 
        ReadDo();                //=== ���ݴ���=== 
        ReadTemp();              //=== ��ȡ�¶�ֵ=== 
        weishu(Temper);
 }
 

main()
{	
        WDTCTL = WDTPW + WDTHOLD;
    //    DCOCTL=CALDCO_1MHZ;
     //   BCSCTL1=CALBC1_1MHZ;               
        P2DIR |=BIT7;                        //DS1302 CS
        P2OUT &=~BIT7;       
        P2DIR |=BIT5;                        //CLOCK
        P2OUT &=~BIT5;
        P2DIR |=BIT6;                        //I/O
        P2OUT &=~BIT6;                           //23ʱ59��58��  ������
        P4DIR=0XFF;
  	res0;
	delay_us(100);
	res1;	  	
  	LCD_init();  //��ʼ��LCDģ�� 
	LCD_clear(); //����Ļ
	write_sec(0x27);
        write_min(0x34);
        write_hr(0x19);
        write_date(0x24); //д��ʱ��Ϊ09��10��31��
        write_month(0x05);
        write_year(0x09);
        write_day(0x07);
        LCD_write_shu(0,0,11);//
        LCD_write_shu(1,0,0);//
        LCD_write_shu(2,0,0);// 
        LCD_write_hanzi(3,0,8);// ��
        LCD_write_shu(5,0,0);//
        LCD_write_shu(6,0,0);//
        LCD_write_hanzi(7,0,9);//��
        LCD_write_shu(9,0,0);//
        LCD_write_shu(10,0,0);
        LCD_write_hanzi(11,0,10);//��
        LCD_write_shu(13,0,11);
        LCD_write_hanzi(0,2,11);//��
        LCD_write_hanzi(2,2,12);//��
        LCD_write_hanzi(4,2,7);//��
        LCD_write_shu(6,2,0);//
        LCD_write_shu(7,2,0);
        LCD_write_shu(8,2,10);
        LCD_write_shu(9,2,0);
        LCD_write_shu(10,2,0);
        LCD_write_shu(11,2,10);
        LCD_write_shu(12,2,0);
        LCD_write_shu(13,2,0);
        LCD_write_shu(0,4,11);
	LCD_write_shu(1,4,11);
        LCD_write_hanzi(1,4,0);//��
        LCD_write_hanzi(3,4,13);//��
        LCD_write_shu(5,4,11);
	LCD_write_shu(6,4,11);
        LCD_write_shu(7,4,0); 
        LCD_write_shu(8,4,0); 
        LCD_write_shu(9,4,12);
        LCD_write_shu(10,4,0);
        LCD_write_shu(11,4,13);
        LCD_write_shu(12,4,14);
     while(1)
     {
       read_all();
        LCD_write_shu(1,0,year1);//
        LCD_write_shu(2,0,year2);// 
        LCD_write_shu(5,0,month1);//
        LCD_write_shu(6,0,month2);//
        LCD_write_shu(9,0,date1);//
        LCD_write_shu(10,0,date2);
        LCD_write_hanzi(4,2,day);//��
        LCD_write_shu(6,2,hr1);
        LCD_write_shu(7,2,hr2);
        LCD_write_shu(9,2,min1);
        LCD_write_shu(10,2,min2);
        LCD_write_shu(12,2,sec1);
        LCD_write_shu(13,2,sec2);
        GetTemp();
        LCD_write_shu(7,4,a); 
        LCD_write_shu(8,4,b);
        LCD_write_shu(10,4,c);
     }
}