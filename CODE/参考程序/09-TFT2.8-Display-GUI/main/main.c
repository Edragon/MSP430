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
{ 
  Init_System();                            //��ʼ��ʱ��ϵͳ��LED
 
  CLR_Screen(White);                        //�ñ���ɫ����   
  
  while(1)
  {  
    GUIline(0,0,319,239,Blue);																			  //���⻭�ߣ���������Ļ�Խ���
   GUIline(319,0,0,239,Blue);
    Delay_ms(2000);                                               //��ʾһ��ʱ��
    CLR_Screen(White);
				
    GUIfull(0,0,159,119,Red);
    GUIfull(160,0,319,119,Green);
    GUIfull(0,120,159,239,Yellow);
    GUIfull(160,120,319,239,Magenta);
    Delay_ms(2000);                                               //��ʾһ��ʱ��
    CLR_Screen(White);
    
    GUIDotline(10,10,310,10,Magenta);		                  //�����߱߿�																	  //���⻭���ߣ���������Ļ�߿�
    GUIDotline(310,10,310,230,Magenta);																		//���⻭���ߣ���������Ļ�߿�
    GUIDotline(10,230,310,230,Magenta);																		//���⻭���ߣ���������Ļ�߿�
    GUIDotline(10,10,10,230,Magenta);						
    Delay_ms(2000);                                               //��ʾһ��ʱ��
    
    
    CLR_Screen(White);
    
    GUIcircle(50,50,20,Blue);	                                  //��3����С��ͬ��Բ																			//���⻭Բ��
    GUIcircle(159,119,70,Blue);
    GUIcircle(270,150,30,Blue);
    Delay_ms(2000);    
    CLR_Screen(White);
    
    GUITable(0,0,319,239,5,2,Blue);                           //������λ���������еı��
    Delay_ms(2000); 
    CLR_Screen(White);
    GUITable(0,0,319,239,7,4,Blue);
    Delay_ms(2000);  
    CLR_Screen(White);
    GUITable(10,10,300,200,4,4,Blue);
    Delay_ms(2000);
    CLR_Screen(White);
    GUITable(40,50,250,200,6,4,Blue);
    Delay_ms(2000);
    CLR_Screen(White);
  }
} 