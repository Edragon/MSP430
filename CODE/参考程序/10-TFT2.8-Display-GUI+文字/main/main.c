/******************************************************************************
中景园电子 	            MSP430F5438A 
店铺连接：https://shop73023976.taobao.com/
功能：测试MSP430F5438A- 2.4/2.8/3.2寸TFT液晶
      像素点：240*320

功能：测试MSP430F5438A - 2.8寸TFT液晶GUI操作
      打点、画直线、画虚线、画圆、填充图像

编译环境：IAR5.3
中景园电子 	            MSP430F5438A 
店铺连接：https://shop73023976.taobao.com/
******************************************************************************/

#include "main.h"

void main(void)                             //主函数 
{   uint i, y;
  Init_System();                            //初始化时钟系统、LED
 
  CLR_Screen(White);                        //用背景色清屏   
  
  while(1)
  {  
         
    LCD_PutString_Chinese24(70,10,"中景园",Yellow,Blue);    //欢迎界面，24x24大小汉字，字模软件隶书小二号
    LCD_PutString_Chinese24(20, 35,"全心全力",White,Blue);       //汉字显示
    LCD_PutString_Chinese24(125, 35,"共创未来",White,Blue);    //欢迎界面，24x24大小汉字，字模软件隶书小二号
    GUIline(0,0,319,239,Blue);																			  //任意画线，这里是屏幕对角线
   GUIline(319,0,0,239,Blue);
    Delay_ms(200);                                               //显示一段时间
    CLR_Screen(White);
				
    GUIfull(0,0,159,119,Red);
    GUIfull(160,0,319,119,Green);
    GUIfull(0,120,159,239,Yellow);
    GUIfull(160,120,319,239,Magenta);
    Delay_ms(200);                                               //显示一段时间
    CLR_Screen(White);
    
    GUIDotline(10,10,310,10,Magenta);		                  //画虚线边框																	  //任意画虚线，这里是屏幕边框
    GUIDotline(310,10,310,230,Magenta);																		//任意画虚线，这里是屏幕边框
    GUIDotline(10,230,310,230,Magenta);																		//任意画虚线，这里是屏幕边框
    GUIDotline(10,10,10,230,Magenta);						
    Delay_ms(200);                                               //显示一段时间
    
    
    CLR_Screen(White);
    
    GUIcircle(50,50,20,Blue);	                                  //画3个大小不同的圆																			//任意画圆形
    GUIcircle(159,119,70,Blue);
    GUIcircle(270,150,30,Blue);
    Delay_ms(200);    
    CLR_Screen(White);
    
    GUITable(0,0,319,239,5,2,Blue);                           //画任意位置任意行列的表格
    Delay_ms(200); 
    CLR_Screen(White);
    GUITable(0,0,319,239,7,4,Blue);
    Delay_ms(200);  
    CLR_Screen(White);
    GUITable(10,10,300,200,4,4,Blue);
    Delay_ms(200);
    CLR_Screen(White);
    GUITable(40,50,250,200,6,4,Blue);
    Delay_ms(200);
    CLR_Screen(White);
     LCD_Clear(Blue2);                      //用背景色清屏   
    LCD_PutString_Chinese24(70,50,"中景园电子",Yellow,Blue2);    //欢迎界面，24x24大小汉字，字模软件隶书小二号
    LCD_PutString_Chinese24(20, 100,"全心全力",White,Blue2);       //汉字显示
    LCD_PutString_Chinese24(125, 100,"共创未来",White,Blue2);    //欢迎界面，24x24大小汉字，字模软件隶书小二号
  
    Delay_ms(5000);    
    CLR_Screen(White);               //用背景色清屏
    Delay_ms(100);    
    CLR_Screen(Magenta);               //用背景色清屏
    Delay_ms(100);    
    CLR_Screen(Green);               //用背景色清屏
    Delay_ms(100);    
    CLR_Screen(Cyan);               //用背景色清屏
    Delay_ms(100);    
    CLR_Screen(Yellow);               //用背景色清屏
    Delay_ms(100);    
    CLR_Screen(Red);               //用背景色清屏
    Delay_ms(100);    
    CLR_Screen(Blue);               //用背景色清屏
    Delay_ms(100);    
    CLR_Screen(Black);               //用背景色清屏    
    Delay_ms(100);    
    CLR_Screen(Blue2);               //用背景色清屏    
    Delay_ms(100); 
    
    for(y=0;y<11;y++)
    {    
      LCD_PutString_Chinese16(15,y*30,"不怕万人阻挡，只怕自己投降",White,Blue2);      //汉字显示
      Delay_ms(100);
    }
    Delay_ms(300);
 
    Show_RGB(0,240,0,64,Blue);                                        //5种颜色将屏分为5个区域
    Show_RGB(0,240,64,128,Green);
    Show_RGB(0,240,128,192,Magenta);
    Show_RGB(0,240,192,256,Red);
    Show_RGB(0,240,256,320,Red);

    
    LCD_PutString_English(80,72,"0123456789",Black,Green);            //第2个区域显示ascii字符中的数字和符号
    LCD_PutString_English(16,96,",,`,./<>';:[]{}\|?-=+*&^%$",Black,Green);
    
    LCD_PutString_English(16,136,"abcdefghijklmnopqrstuvwxyz",Blue,Magenta);//第3个区域显示ascii字符中字母
    LCD_PutString_English(16,156,"ABCDEFGHIJKLMNOPQRSTUVWXYZ",Blue,Magenta);
  }
} 