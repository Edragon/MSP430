/******************************************************************************
�о�԰���� 	            MSP430F5438A 
�������ӣ�https://shop73023976.taobao.com/
���ܣ�����MSP430F5438A- 2.4/2.8/3.2��TFTҺ��
      ���ص㣺240*320
  
˵��������XT1--�ⲿ32.768Khz��XT2--�ⲿ24Mhz

���뻷����IAR
�о�԰���� 	          
�������ӣ�https://shop73023976.taobao.com/
******************************************************************************/

#include "main.h"

uchar Taobao_Webpage[] = "https://shop73023976.taobao.com";

void main(void)                             //������ 
{ 
  uint i, y;
  Init_System();                            //��ʼ��ʱ��ϵͳ��LED
  CLR_Screen(White);                      //�ñ���ɫ����   
  	CLR_Screen(White); //???��
         LCD_PutString_English(80,72,"0123456789",Black,Green);       
   // Show_RGB(0,0,240-1,320-1,Red);
  while(1)
  {
    LCD_Clear(Blue2);                      //�ñ���ɫ����   
    LCD_PutString_Chinese24(70,50,"�о�԰����",Yellow,Blue2);    //��ӭ���棬24x24��С���֣���ģ�������С����
    LCD_PutString_Chinese24(20, 100,"ȫ��ȫ��",White,Blue2);       //������ʾ
    LCD_PutString_Chinese24(125, 100,"����δ��",White,Blue2);    //��ӭ���棬24x24��С���֣���ģ�������С����
    LCD_PutString_English(0,290,Taobao_Webpage,White,Blue2);               //�ַ���ʾ�ٷ���վ
    Delay_ms(5000);    
    CLR_Screen(White);               //�ñ���ɫ����
    Delay_ms(1000);    
    CLR_Screen(Magenta);               //�ñ���ɫ����
    Delay_ms(1000);    
    CLR_Screen(Green);               //�ñ���ɫ����
    Delay_ms(1000);    
    CLR_Screen(Cyan);               //�ñ���ɫ����
    Delay_ms(1000);    
    CLR_Screen(Yellow);               //�ñ���ɫ����
    Delay_ms(1000);    
    CLR_Screen(Red);               //�ñ���ɫ����
    Delay_ms(1000);    
    CLR_Screen(Blue);               //�ñ���ɫ����
    Delay_ms(1000);    
    CLR_Screen(Black);               //�ñ���ɫ����    
    Delay_ms(1000);    
    CLR_Screen(Blue2);               //�ñ���ɫ����    
    Delay_ms(1000); 
    
    for(y=0;y<11;y++)
    {    
      LCD_PutString_Chinese16(15,y*30,"���������赲��ֻ���Լ�Ͷ��",White,Blue2);      //������ʾ
      Delay_ms(100);
    }
    Delay_ms(3000);
 
    Show_RGB(0,240,0,64,Blue);                                        //5����ɫ������Ϊ5������
    Show_RGB(0,240,64,128,Green);
    Show_RGB(0,240,128,192,Magenta);
    Show_RGB(0,240,192,256,Red);
    Show_RGB(0,240,256,320,Red);
     
    LCD_PutString_Chinese24(70,10,"�о�԰",Yellow,Blue);    //��ӭ���棬24x24��С���֣���ģ�������С����
    LCD_PutString_Chinese24(20, 35,"ȫ��ȫ��",White,Blue);       //������ʾ
    LCD_PutString_Chinese24(125, 35,"����δ��",White,Blue);    //��ӭ���棬24x24��С���֣���ģ�������С����
    
    LCD_PutString_English(80,72,"0123456789",Black,Green);            //��2��������ʾascii�ַ��е����ֺͷ���
    LCD_PutString_English(16,96,",,`,./<>';:[]{}\|?-=+*&^%$",Black,Green);
    
    LCD_PutString_English(16,136,"abcdefghijklmnopqrstuvwxyz",Blue,Magenta);//��3��������ʾascii�ַ�����ĸ
    LCD_PutString_English(16,156,"ABCDEFGHIJKLMNOPQRSTUVWXYZ",Blue,Magenta);
    
   
//    ��5��������ʵ�ֻ���       
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
    
        Show_RGB(0,240,0,64,Blue);                                        //5����ɫ������Ϊ5������
    Show_RGB(0,240,64,128,Green);
    Show_RGB(0,240,128,192,Magenta);
    Show_RGB(0,240,192,256,Red);
    Show_RGB(0,240,256,320,Red);
  }
} 