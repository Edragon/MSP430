#include <msp430x14x.h>
#include "code.h"
typedef unsigned char uchar;
typedef unsigned int  uint;

#define BufferSize  32
extern uchar kb_buffer[BufferSize];
extern uchar input;
extern uchar output;
extern uchar flag;
/*******************************************
函数名称：PushBuff
功    能：将一个字符压入显示缓存，如果缓存以
          满则覆盖前面的数据
参    数：c--要显示的字符
返回值  ：无
********************************************/
void PutChar(uchar c)
{
    kb_buffer[input] = c;
    if (input < (BufferSize-1))
        input++; 
    else
        input = 0;	 
}
/*******************************************
函数名称：PopChar
功    能：从显示缓存中取出一个字符
参    数：无
返回值  ：取出的字符
********************************************/
uchar GetChar(void)
{
    uchar temp;
    
    if(output == input)
        return 0xff;
    else
    {
	    temp = kb_buffer[output];
	    if(output < (BufferSize-1))
	    {
	        output++;
        }
	    else
        {
	        output = 0;
        }
	    return temp;	  
    }	     
}
/*******************************************
函数名称：Init_KB
功    能：初始化与键盘相关的IO
参    数：无
返回值  ：无
********************************************/
void Init_KB(void)
{     
  P1DIR =0XCF;
  P1OUT=0XFF;
}
/*******************************************
函数名称：Decode
功    能：对来自键盘的信息进行解码，转换成对
          应的ASCII编码并压入缓存
参    数：sc--键盘发送过来的信息
返回值  ：是否收到有效数据：0--否，1--是
说明    ：本程序只能对基本按键（即键被按下时产
          生三个字节的扫描码的按键）做出解码，
          包括所有的可显示字符键和Table，
          Back Space和Enter三个特殊功能键。
基本按键的扫描码由三个字节组成，第1个字节为接通
码，第2、3字节为断开码；其中第1字节和第3字节相
同，中间字节为断开标志0xf0。
********************************************/
uchar Decode(uchar sc)
{
      static uchar shift = 0; //Shift键是否按下标志：1--按下，0--未按
      static uchar up = 0;    //键已放开标志：       1--放开，0--按下
      uchar i,flag = 0;
      
      if(sc == 0xf0)    //如果收到的是扫描码的第2个字节---0xf0：按键断开标志
      {
          up = 1;        
          return 0;
      }
      else if(up == 1)  //如果收到的是扫描码的第3个字节
      {
	      up = 0;         
          if((sc == 0x12) || ( sc==0x59))   shift = 0;
	      return 0;
      }	
      
      //如果收到的是扫描码的第1个字节
      if((sc == 0x12) || (sc == 0x59)) //如果是左右shift键
      {      
	      shift = 1;	        //设置Shift按下标志
          flag = 0;
      }		           	           
      else
      {
	      if(shift) //对按下Shift的键进行解码
		  {
		       for(i = 0;(shifted[i][0] != sc) && shifted[i][0];i++);
               if (shifted[i][0] == sc) 
               {
                    PutChar(shifted[i][1]);
                    flag = 1;
               }
		  }
		  else  //直接对按键进行解码
		  {
		       for(i = 0;(unshifted[i][0] != sc) && unshifted[i][0];i++);
               if(unshifted[i][0] == sc)  
               {
                    PutChar(unshifted[i][1]);
                    flag = 1;
               }
	      } 
      }
      if(flag)  return 1;
      else      return 0;
}
