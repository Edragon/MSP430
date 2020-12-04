#include "touch.h"
#include "../sys/sys.h"
#include "../lcd/lcd.h"

//Xmin、max和Ymin、Ymax分别是触摸屏横纵坐标的最小/最大值,如果偏差大时，用户可重新标定
#define Xmin           0x0133
#define Xmax           0x0EDA
#define Ymin           0x00D0
#define Ymax           0x0E2F

/*********************液晶触摸管脚定义 Start************************************/
#define PEN_CLR	        P4OUT &= ~BIT6           //PEN置低,触碰触摸屏时，ADS7843Penirq引脚由未触摸时的高电平变为低电平
#define PEN_SET	        P4OUT |=  BIT6           //PEN置高
#define PEN             (P4IN & BIT6)            //P4.6输入的值 

#define TPDO_CLR	P7OUT &= ~BIT2           //TPDO置低   --> MIS0
#define TPDO_SET	P7OUT |=  BIT2           //TPDO置高
#define TPDOUT          (P7IN & BIT2)            //P7.2输入的值 

#define BUSY_CLR	P4OUT &= ~BIT5           //BUSY置低
#define BUSY_SET	P4OUT |=  BIT5           //BUSY置高

#define TPDI_CLR	P7OUT &= ~BIT3           //TPDI置低  --> MOSI
#define TPDI_SET	P7OUT |=  BIT3           //TPDI置高

#define TPCS_CLR	P4OUT &= ~BIT4           //TPCS置低
#define TPCS_SET	P4OUT |=  BIT4           //TPCS置高

#define TPCLK_CLR	P4OUT &= ~BIT7           //TPCLK置低
#define TPCLK_SET	P4OUT |=  BIT7           //TPCLK置高
/*********************液晶触摸管脚定义 END************************************/  

struct struct1 coordinate;        //定义一个名为Tp_pix的结构变量,用于存放触摸点采样结果

uint  lx,ly;                      //根据采集到的参数转换为TFT上的实际坐标值	

void Init_Touch_Port()
{
  P4DIR|= BIT4 + BIT5 + BIT7;     //TFT显示控制线
  P7DIR|= BIT3;                   //TFT显示控制线
}

void Init_Touch()
{
  Init_Touch_Port();
  TPCLK_CLR;
  TPCS_SET;
  TPDI_SET;
  TPCLK_SET;
  TPCS_CLR;  
}

//================================================================================================
//	实现功能：读取采集结果,两次取均值
//  取采样值，此处使用软件滤波，2次取样的值相差太大的视为噪声
//================================================================================================
uchar Getpix() 
{
  uchar flag=0;
  struct struct1 pix1;
  struct struct1 pix2; 
  
  if(PEN==0)
  {  	
    pix1=AD_Touch();
    pix2=AD_Touch();
    
    if(pix_filter(pix1,pix2)==1) //得到当前TP的取样值，此处使用软件滤波，2次取样的值相差太大的视为噪声
    {
      if((coordinate.x>Xmin)&&(coordinate.y>Ymin))
      {
        lx=(uint)(240.0*(coordinate.x-Xmin)/(Xmax-Xmin));       //坐标转换，即根据采样值计算实际坐标值
        ly=(uint)(320.0*(coordinate.y-Ymin)/(Ymax-Ymin));   //Xmin、max和Ymin、Ymax分别是触摸屏横纵坐标的最小/最大值
        flag=1;
      }
    }				 
  }
  
  return flag; 	
}

//================================================================================================
//	实现功能：读取触摸点X轴和Y轴电压值
//	返回参数：pix 读取到的触摸点电压值
//================================================================================================
struct struct1 AD_Touch()            
{
  struct struct1 pix;
  TPCS_CLR;
  
  Write_Touch(0x90);                 //送控制字 10010000 即用差分方式读X坐标，详细请见有关资料
  TPCLK_SET; 
  Delay_us(1); 
  TPCLK_CLR; 
  Delay_us(1); 
  pix.y=Read_Touch();
  
  Write_Touch(0xD0);                 //送控制字 11010000 即用差分方式读Y坐标 详细请见有关资料
  TPCLK_SET; 
  Delay_us(1); 
  TPCLK_CLR; 
  Delay_us(1); 
  pix.x=Read_Touch();
  
  TPCS_SET; 
  return pix;
}

//================================================================================================
//	实现功能：	写8位命令到触摸控制IC
//	输入参数：  temp 需要写入的8位控制命令
//================================================================================================
void Write_Touch(uchar temp)                         //SPI写8位命令到触摸控制IC
{
  uchar i=0;
  
  for(i=0;i<8;i++)                                    //循环8次写入一字节
  {
    if(temp&0x80)
      TPDI_SET;
    else 
      TPDI_CLR;    //判断最高位是否为1,为1则向数据位写1
    TPCLK_CLR; 
    Delay_us(1);                //送一个脉冲，上升沿有效，将DIN位数据送入到IC
    TPCLK_SET; 
    Delay_us(1);                
    temp<<=1;                           //待写数据左移1位，准备好写下一位数据
  }
}

//================================================================================================
//	实现功能：	从触摸控制IC读8位数据到控制器
//	返回参数：  temp 需要写入的8位控制命令
//================================================================================================
uint Read_Touch()                          //SPI 读数据
{
  uchar i=0;
  uint temp=0;
  
  for(i=0;i<12;i++)                         //循环12次读取12位结果
  { 
    temp<<=1;                                  //temp左移一位，准备读取下一位
    TPCLK_SET; 
    Delay_us(1);                   //下降沿有效
    TPCLK_CLR; 
    Delay_us(1);
    if(TPDOUT) 
      temp++;                     //判断控制IC送出的一位数据是否为1,如果为1,赋给temp的最低位
  }
  return(temp);                                 //返回结果
}	

//================================================================================================
//	实现功能：软件滤波，滤掉波动过大的采样点
//	返回参数：flag 采集数据是否有效标志,flag=1;则数据有效
//================================================================================================
uchar pix_filter(struct struct1 pix1,struct struct1 pix2)
{
  uchar flag=0;
  int x=pix1.x>pix2.x?pix1.x-pix2.x:pix2.x-pix1.x;  //X轴两次采样绝对值
  int y=pix1.y>pix2.y?pix1.y-pix2.y:pix2.y-pix1.y;  //Y轴两次采样绝对值
  if(x<10&&y<10)	                                  //软件滤波，2次取样的值相差太大的视为噪声
  {
    flag=1;
    coordinate.x=(pix1.x+pix2.x)/2;           //求两次采样平均值
    coordinate.y=(pix1.y+pix2.y)/2;
  }	
  return flag;
}

/********************************************************************
在触摸点位置画点
********************************************************************/
void drawpoint(unsigned int x,unsigned int y,unsigned int color)
{
  LCD_SetPos(x,240-y,x+1,240-y-1);
  Write_Data(color);
  Write_Data(color);
  Write_Data(color);
  Write_Data(color);
} 