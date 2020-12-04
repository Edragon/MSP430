/******************************************************************************
�о�԰���� 	            MSP430F5438A 
�������ӣ�https://shop73023976.taobao.com/
���ܣ�����MSP430F5438A- 2.4/2.8/3.2��TFTҺ��
      ���ص㣺240*320

���ܣ�����MSP430F5438A - 2.8��TFTҺ��GUI����
      ��㡢��ֱ�ߡ������ߡ���Բ�����ͼ��

���뻷����IAR5.3
�о�԰���� 	            MSP430F5438A 
�������ӣ�https://shop73023976.taobao.com/
******************************************************************************/

#include "main.h"

void main(void)                             //������ 
{   uint i, y;
  Init_System();                            //��ʼ��ʱ��ϵͳ��LED
 
  CLR_Screen(White);                        //�ñ���ɫ����   
  
  while(1)
  {  
         
    LCD_PutString_Chinese24(70,10,"�о�԰",Yellow,Blue);    //��ӭ���棬24x24��С���֣���ģ�������С����
    LCD_PutString_Chinese24(20, 35,"ȫ��ȫ��",White,Blue);       //������ʾ
    LCD_PutString_Chinese24(125, 35,"����δ��",White,Blue);    //��ӭ���棬24x24��С���֣���ģ�������С����
    GUIline(0,0,319,239,Blue);																			  //���⻭�ߣ���������Ļ�Խ���
   GUIline(319,0,0,239,Blue);
    Delay_ms(200);                                               //��ʾһ��ʱ��
    CLR_Screen(White);
				
    GUIfull(0,0,159,119,Red);
    GUIfull(160,0,319,119,Green);
    GUIfull(0,120,159,239,Yellow);
    GUIfull(160,120,319,239,Magenta);
    Delay_ms(200);                                               //��ʾһ��ʱ��
    CLR_Screen(White);
    
    GUIDotline(10,10,310,10,Magenta);		                  //�����߱߿�																	  //���⻭���ߣ���������Ļ�߿�
    GUIDotline(310,10,310,230,Magenta);																		//���⻭���ߣ���������Ļ�߿�
    GUIDotline(10,230,310,230,Magenta);																		//���⻭���ߣ���������Ļ�߿�
    GUIDotline(10,10,10,230,Magenta);						
    Delay_ms(200);                                               //��ʾһ��ʱ��
    
    
    CLR_Screen(White);
    
    GUIcircle(50,50,20,Blue);	                                  //��3����С��ͬ��Բ																			//���⻭Բ��
    GUIcircle(159,119,70,Blue);
    GUIcircle(270,150,30,Blue);
    Delay_ms(200);    
    CLR_Screen(White);
    
    GUITable(0,0,319,239,5,2,Blue);                           //������λ���������еı��
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
     LCD_Clear(Blue2);                      //�ñ���ɫ����   
    LCD_PutString_Chinese24(70,50,"�о�԰����",Yellow,Blue2);    //��ӭ���棬24x24��С���֣���ģ�������С����
    LCD_PutString_Chinese24(20, 100,"ȫ��ȫ��",White,Blue2);       //������ʾ
    LCD_PutString_Chinese24(125, 100,"����δ��",White,Blue2);    //��ӭ���棬24x24��С���֣���ģ�������С����
  
    Delay_ms(5000);    
    CLR_Screen(White);               //�ñ���ɫ����
    Delay_ms(100);    
    CLR_Screen(Magenta);               //�ñ���ɫ����
    Delay_ms(100);    
    CLR_Screen(Green);               //�ñ���ɫ����
    Delay_ms(100);    
    CLR_Screen(Cyan);               //�ñ���ɫ����
    Delay_ms(100);    
    CLR_Screen(Yellow);               //�ñ���ɫ����
    Delay_ms(100);    
    CLR_Screen(Red);               //�ñ���ɫ����
    Delay_ms(100);    
    CLR_Screen(Blue);               //�ñ���ɫ����
    Delay_ms(100);    
    CLR_Screen(Black);               //�ñ���ɫ����    
    Delay_ms(100);    
    CLR_Screen(Blue2);               //�ñ���ɫ����    
    Delay_ms(100); 
    
    for(y=0;y<11;y++)
    {    
      LCD_PutString_Chinese16(15,y*30,"���������赲��ֻ���Լ�Ͷ��",White,Blue2);      //������ʾ
      Delay_ms(100);
    }
    Delay_ms(300);
 
    Show_RGB(0,240,0,64,Blue);                                        //5����ɫ������Ϊ5������
    Show_RGB(0,240,64,128,Green);
    Show_RGB(0,240,128,192,Magenta);
    Show_RGB(0,240,192,256,Red);
    Show_RGB(0,240,256,320,Red);

    
    LCD_PutString_English(80,72,"0123456789",Black,Green);            //��2��������ʾascii�ַ��е����ֺͷ���
    LCD_PutString_English(16,96,",,`,./<>';:[]{}\|?-=+*&^%$",Black,Green);
    
    LCD_PutString_English(16,136,"abcdefghijklmnopqrstuvwxyz",Blue,Magenta);//��3��������ʾascii�ַ�����ĸ
    LCD_PutString_English(16,156,"ABCDEFGHIJKLMNOPQRSTUVWXYZ",Blue,Magenta);
  }
} 