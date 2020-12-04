#include "hs0038.h"
#include "../time/time.h"

#define t_1ms   5328    //83

/*******************************************
函数名称：HS0038_High
功    能：HS0038数据线输出高
参    数：无
返回值  ：无
********************************************/
void HS0038_High()
{
  P6DIR |= BIT0;
  P6OUT |= BIT0;
}

/*******************************************
函数名称：HS0038_Low
功    能：HS0038数据线输出低
参    数：无
返回值  ：无
********************************************/
void HS0038_Low()
{
  P6DIR |= BIT0;
  P6OUT &= ~BIT0;
}

/*******************************************
函数名称：Read_HS0038
功    能：读HS0038数据
参    数：无
返回值  ：无
********************************************/
uchar Read_HS0038()
{ 
  uchar stat;
  
  P6DIR &= ~BIT0;
  stat = P6IN;
  
  return (stat);
}


/*******************************************
函数名称：Decode_HS0038
功    能：解码键盘发送的值
参    数：无
返回值  ：按键值
********************************************/
uchar Decode_HS0038(void)
{  
  uchar HS0038_Value_Temp;
  uchar Err_Value = 'Z';
  uchar HS0038_Value;
  uchar Data_Decode;
  
  HS0038_High();                         //初始化HS0038数据线置高
 
  do
  {
    HS0038_Value_Temp = Read_HS0038();  //读HS0038值
  }while(HS0038_Value_Temp & BIT0);     //等待，直到数据线变低
  Delay_ms(5);                          //延时58毫秒  
  HS0038_Value_Temp = Read_HS0038();    //读HS0038值
  if(HS0038_Value_Temp & BIT0)          //如果不为低
  {
    Delay_ms(50);
    return Err_Value; 
  }
  do
  {
    HS0038_Value_Temp = Read_HS0038();  //读HS0038值
  }while(!(HS0038_Value_Temp & BIT0));  //等待，直到数据线变高
  Delay_ms(3);                          //延时3.5毫秒  
//  Delay_us(500);
  HS0038_Value_Temp = Read_HS0038();    //读HS0038值
  if(!(HS0038_Value_Temp & BIT0))          //如果不为高
  {
    Delay_ms(50);
    return Err_Value; 
  } 
  Data_Decode = Decode_HS0038_Value();
  
  switch(Data_Decode)
  {
    case 0x2D:HS0038_Value='0'; break;//0
    case 0x19:HS0038_Value='1'; break;//1
    case 0x31:HS0038_Value='2'; break;//2
    case 0xBD:HS0038_Value='3'; break;//3
    case 0x11:HS0038_Value='4'; break;//4
    case 0x39:HS0038_Value='5'; break;//5
    case 0xB5:HS0038_Value='6'; break;//6
    case 0x85:HS0038_Value='7'; break;//7
    case 0xA5:HS0038_Value='8'; break;//8
    case 0x95:HS0038_Value='9'; break;//9
    case 0x8B:HS0038_Value='A'; break;//关机    
    case 0x8F:HS0038_Value='B'; break;//MENU，菜单
    case 0x89:HS0038_Value='C'; break;//TEST，测试
    case 0x87:HS0038_Value='D'; break;//返回
    case 0x2B:HS0038_Value='E'; break;//播放
    case 0x81:HS0038_Value='F'; break;//+
    case 0x33:HS0038_Value='G'; break;//-
    case 0x0F:HS0038_Value='H'; break;//左
    case 0x13:HS0038_Value='I'; break;//右
    default:  HS0038_Value='Z'; break;
  }

  return HS0038_Value; 
}


/*******************************************
函数名称：Decode_HS0038
功    能：解码键盘发送的值
参    数：无
返回值  ：按键值
********************************************/
uchar Decode_HS0038_Value()
{ 
  uchar i, j, dat=0;
  uchar HS0038_Value_Temp;
  uchar HS0038_Code[4];
  uint Time_Value = 0;
  
  for(j=0;j<4;j++)              //四组解码值，前两组为地址码，后两组为按键值编码
  { 
    for(i=8;i>0;i--)
    {
      dat>>=1; 
      do
      { 
        HS0038_Value_Temp = Read_HS0038();      //读HS0038值 
      }while(!(HS0038_Value_Temp & BIT0));	//等待数据线变高
     
      Set_Timer();		                //检测高电平时间
      do
      {
        HS0038_Value_Temp = Read_HS0038();      //读HS0038值
      }while(HS0038_Value_Temp & BIT0);	        //等待数据线变高
      Time_Value = TA1R;
      Stop_Timer();
      
      if(Time_Value > t_1ms)
        dat=dat|0x80;
      else 
        dat=dat&0x7f;  	  
      }
     HS0038_Code[j]=dat;
   }
  
  return(HS0038_Code[2]);    
}

