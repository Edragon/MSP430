/*************************************************************
�����ܣ���ȡDS18B20�¶�ֵ����1602Һ������ʾ
*************************************************************/
#include  "msp430.h"
#include  "cry1602.h"
#include "DS18B20.h"
#define uchar unsigned char
#define uint unsigned int
//Ҫ��ʾ��6λ�¶�����
uchar dN[6]; 

void Disp_Numb(uint temper);
/*************************������*************************/
void main( void )
{
    uchar i;
    
    WDTCTL = WDTPW + WDTHOLD;           //�ع�
    /*------ѡ��ϵͳ��ʱ��Ϊ8MHz-------*/
    BCSCTL1 &= ~XT2OFF;                 //��XT2��Ƶ��������
    do
    {
        IFG1 &= ~OFIFG;                 //�������ʧ�ܱ�־
        for (i = 0xFF; i > 0; i--);     //�ȴ�8MHz��������
    }
    while ((IFG1 & OFIFG));             //����ʧЧ��־��Ȼ���ڣ�
    BCSCTL2 |= SELM_2 + SELS;           //MCLK��SMCLKѡ���Ƶ����
   
    //����DS18B20��IO״̬
    P1DIR |= BIT7;
    P1OUT |= BIT7;
    //����ʱ��ѡ��SMLK=8MHz��1/8��Ƶ��Ϊ1MHz
    TACTL |= TASSEL_2 + ID_3; 
    //��ȫ���ж�
    _EINT();
    
    LcdReset();
    DispStr(0,0,"Temperature is:");
    //ѭ��������ʾ
    while(1)
    {
       Disp_Numb(Do1Convert());
       Disp1Char(4,1,dN[5]+0x30);
       Disp1Char(5,1,dN[4]+0x30);
       Disp1Char(6,1,0x2e);         //0x2e��С�����Ӧ��ASCII��ֵ
       Disp1Char(7,1,dN[3]+0x30);
       Disp1Char(8,1,dN[2]+0x30);
       Disp1Char(9,1,dN[1]+0x30);
       Disp1Char(10,1,dN[0]+0x30);
    }          
}

/*******************************************
�������ƣ�Disp_Numb
��    �ܣ�����DS18B20��ȡ��11bit�¶�����ת��
          ���������ʾ���¶�����
��    ����temper--11bit�¶�����
����ֵ  ����
********************************************/
void Disp_Numb(uint temper)
{
    uchar i;
    
    for(i = 0;i < 6;i++) dN[i] = 0; //��ʼ����ʾ����

    //��ֵת��
    if(temper & BIT0) 
    {
        dN[0] = 5;
        dN[1] = 2;
        dN[2] = 6;
    }
    if(temper&BIT1)     
    {
        dN[1] += 5;
        dN[2] += 2;
        dN[3] += 1;
    }
    if(temper & BIT2)     
    {
        dN[2] += 5;
        dN[3] += 2;
        if(dN[2] >= 10)
        {
            dN[2] -= 10;
            dN[3] += 1;
        }
    }
    if(temper&BIT3)     
    {
        dN[3] += 5;
    }
    if(temper & BIT4)
    {
        dN[4] += 1;
    }
    if(temper & BIT5)     
    {
        dN[4] += 2;
    }
    if(temper & BIT6)
    {
        dN[4] += 4;
    }
    if(temper & BIT7)     
    {
        dN[4] += 8;
        if(dN[4] >= 10)
        {
            dN[4] -= 10;
            dN[5] += 1;
        }
    }
    if(temper & BIT8)
    {
        dN[4] += 6;
        dN[5] += 1;
        if(dN[4] >= 10)
        {
            dN[4] -= 10;
            dN[5] += 1;
        }
    }
    if(temper & BIT9)
    {
        dN[4] += 2;
        dN[5] += 3;
        if(dN[4] >= 10)
        {
            dN[4] -= 10;
            dN[5] += 1;
        }
    }
    if(temper & BITA)
    {
        dN[4] += 4;
        dN[5] += 6;
        if(dN[4] >= 10)
        {
            dN[4] -= 10;
            dN[5] += 1;
        }
        if(dN[5] >= 10)
        {
            dN[5] -= 10;
        }
    }
}
