/******************************************************************************
中景园电子 	            MSP430F5438A 
店铺连接：https://shop73023976.taobao.com/
功能：测试MSP430F5438A- 2.4/2.8/3.2寸TFT液晶
      像素点：240*320
  
说明：启用XT1--外部32.768Khz，XT2--外部24Mhz

编译环境：IAR
中景园电子 	          
店铺连接：https://shop73023976.taobao.com/
******************************************************************************/

#include "main.h"

uchar Taobao_Webpage[] = "https://shop73023976.taobao.com";

void main(void)                             //主函数 
{ 
  uint i, y;
  Init_System();                            //初始化时钟系统、LED
  CLR_Screen(White);                      //用背景色清屏   
  	CLR_Screen(White); //???á
         LCD_PutString_English(80,72,"0123456789",Black,Green);       
   // Show_RGB(0,0,240-1,320-1,Red);
  while(1)
  {
    LCD_Clear(Blue2);                      //用背景色清屏   
    LCD_PutString_Chinese24(70,50,"中景园电子",Yellow,Blue2);    //欢迎界面，24x24大小汉字，字模软件隶书小二号
    LCD_PutString_Chinese24(20, 100,"全心全力",White,Blue2);       //汉字显示
    LCD_PutString_Chinese24(125, 100,"共创未来",White,Blue2);    //欢迎界面，24x24大小汉字，字模软件隶书小二号
    LCD_PutString_English(0,290,Taobao_Webpage,White,Blue2);               //字符显示官方网站
    Delay_ms(5000);    
    CLR_Screen(White);               //用背景色清屏
    Delay_ms(1000);    
    CLR_Screen(Magenta);               //用背景色清屏
    Delay_ms(1000);    
    CLR_Screen(Green);               //用背景色清屏
    Delay_ms(1000);    
    CLR_Screen(Cyan);               //用背景色清屏
    Delay_ms(1000);    
    CLR_Screen(Yellow);               //用背景色清屏
    Delay_ms(1000);    
    CLR_Screen(Red);               //用背景色清屏
    Delay_ms(1000);    
    CLR_Screen(Blue);               //用背景色清屏
    Delay_ms(1000);    
    CLR_Screen(Black);               //用背景色清屏    
    Delay_ms(1000);    
    CLR_Screen(Blue2);               //用背景色清屏    
    Delay_ms(1000); 
    
    for(y=0;y<11;y++)
    {    
      LCD_PutString_Chinese16(15,y*30,"不怕万人阻挡，只怕自己投降",White,Blue2);      //汉字显示
      Delay_ms(100);
    }
    Delay_ms(3000);
 
    Show_RGB(0,240,0,64,Blue);                                        //5种颜色将屏分为5个区域
    Show_RGB(0,240,64,128,Green);
    Show_RGB(0,240,128,192,Magenta);
    Show_RGB(0,240,192,256,Red);
    Show_RGB(0,240,256,320,Red);
     
    LCD_PutString_Chinese24(70,10,"中景园",Yellow,Blue);    //欢迎界面，24x24大小汉字，字模软件隶书小二号
    LCD_PutString_Chinese24(20, 35,"全心全力",White,Blue);       //汉字显示
    LCD_PutString_Chinese24(125, 35,"共创未来",White,Blue);    //欢迎界面，24x24大小汉字，字模软件隶书小二号
    
    LCD_PutString_English(80,72,"0123456789",Black,Green);            //第2个区域显示ascii字符中的数字和符号
    LCD_PutString_English(16,96,",,`,./<>';:[]{}\|?-=+*&^%$",Black,Green);
    
    LCD_PutString_English(16,136,"abcdefghijklmnopqrstuvwxyz",Blue,Magenta);//第3个区域显示ascii字符中字母
    LCD_PutString_English(16,156,"ABCDEFGHIJKLMNOPQRSTUVWXYZ",Blue,Magenta);
    
   
//    第5个区域打点实现画线       
    for(i=312; i>264; i--)
    { 
      Put_pixel(68,i,Blue);
    }                 
    for(i=68;i<172;i++) 
    { 
      Put_pixel(i,264,Blue);
    }
    for(i=264;i<312;i++)
    { 
      Put_pixel(172,i,Blue);
    } 
    for(i=172;i>68;i--) 
    { 
      Put_pixel(i,312,Blue);
    }
    
    Delay_ms(50);
    
        Show_RGB(0,240,0,64,Blue);                                        //5种颜色将屏分为5个区域
    Show_RGB(0,240,64,128,Green);
    Show_RGB(0,240,128,192,Magenta);
    Show_RGB(0,240,192,256,Red);
    Show_RGB(0,240,256,320,Red);
  }
} 