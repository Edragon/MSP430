/*************************************************************
�����ܣ���12864Һ������ʾASCII�����ַ�
**************************************************************/
#include  "msp430.h"
#include  "cry12864.h"

/***************************������***************************/
void main( void )
{
    unsigned int i,j;
    unsigned char tishi[] = {"This is the end!"};
    
    WDTCTL = WDTPW + WDTHOLD;    //�ع�
    Ini_Lcd();                  //��ʼ��Һ��
    
    Write_Cmd(0x80);            //д��һ�е���ʾ��ַ
    for(i = 0; i < 16; i++)
      Write_Data(0x30 + i);     //��ʾ0x30~0x3f��Ӧ���ַ�
    Write_Cmd(0x90);            //д�ڶ��е���ʾ��ַ
    for(i = 0; i < 16; i++)
      Write_Data(0x40 + i);     //��ʾ0x40~0x4f��Ӧ���ַ�
    Write_Cmd(0x88);            //д�����е���ʾ��ַ
    for(i = 0; i < 16; i++)
      Write_Data(0x50 + i);     //��ʾ0x50~0x5f��Ӧ���ַ�
    Write_Cmd(0x98);            //д�����е���ʾ��ַ
    for(i = 0; i < 16; i++)
      Write_Data(0x60 + i);     //��ʾ0x60~0x6f��Ӧ���ַ�
    
    for(i = 1000; i > 0; i--)        //��ʱһ��
    {
        for(j = 1000; j > 0; j--)
          _NOP();    
    }
    
    Write_Cmd(0x01);            //����
    
    Write_Cmd(0x80);            //д��һ�е���ʾ��ַ
    for(i = 0; i < 16; i++)
      Write_Data(0x70 + i);     //��ʾ0x70~0x7f��Ӧ���ַ�
    
    Write_Cmd(0x88);            //д�����е���ʾ��ַ
    for(i = 0; i < 16; i++)
      Write_Data(tishi[i]);     //��ʾ�Զ�����ַ�
    
    LPM4;
}
