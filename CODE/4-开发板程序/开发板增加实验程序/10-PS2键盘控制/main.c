/*****************************************************
�����ܣ����ղ��������Ա�׼���̵Ļ���������ɨ����
����ļ���ͨ��LED��ʾ��
*****************************************************/
#include  <msp430x14x.h>
#include "Keyboard.h"
#include "gdata.h"
#define  uchar unsigned char
#define  uint  unsigned int
uchar flag=1,flagbeep;
#define SIDval  P5IN & BIT1//SIDval  P5IN & BIT6
 void  PORT(void);
 
 void beep()
 {
   uint i;
   for(i=0;i<40000;i++)
   P5OUT=0xfe;
   for(i=0;i<40000;i++)
   P5OUT=0xff;   
 }
/****************������****************/
void main(void)
{
    uchar disptmp,i;
    
    WDTCTL = WDTPW + WDTHOLD;   //�رտ��Ź�
    
    /*------ѡ��ϵͳ��ʱ��Ϊ8MHz-------*/
    BCSCTL1 &= ~XT2OFF;                 // ��XT2��Ƶ��������
    do
    {
        IFG1 &= ~OFIFG;                 // �������ʧ�ܱ�־
        for (i = 0xFF; i > 0; i--);     // �ȴ�8MHz��������
    }
        while ((IFG1 & OFIFG));             // ����ʧЧ��־��Ȼ���ڣ�
    BCSCTL2 |= SELM_2 + SELS;           // MCLK��SMCLKѡ���Ƶ����
    
    TACCTL0 |= CCIE;                    //ʹ�ܱȽ��ж�
    TACTL |= TASSEL_2 + ID_3 ;          //����ʱ��ѡ��SMLK=8MHz��1/8��Ƶ��Ϊ1MHz

    P2DIR = 0xff;
    P2OUT = 0xff;
    
    Init_KB();                  //��ʼ�����̶˿�
    //_EINT();                    //��ȫ���ж�
  
    while(1)
    {
      while(flag)
      {
        PORT();
      }
        disptmp = GetChar();    //��ȡ��ֵ��Ӧ��ASCII��
        
        if((disptmp > 0x2f)&&(disptmp < 0x3a)) //������յ����ַ���0~9
        {
            switch(disptmp) 
            {
              case '0':
                        P2OUT = ~0x55;
                        break;
              case '1':
                        P2OUT = ~0x1;
                        break;
              case '2':
                        P2OUT = ~0x2;
                        break;
              case '3':
                        P2OUT = ~0x4;
                        break;
              case '4':
                        P2OUT = ~0x8;
                        break;
              case '5':
                        P2OUT = ~0x10;
                        break;
              case '6':
                        P2OUT = ~0x20;
                        break;
              case '7':
                        P2OUT = ~0x40;
                        break;
              case '8':
                        P2OUT = 0x7f;  
                        break;
              case '9':
                        P2OUT = 0x7e;
                        break;
              default:
                        _NOP();
              }
          }
        else if(disptmp == 'b')
        {flagbeep=1;
            P2OUT = 0xff;
        }
        else if(disptmp == 's')
        {flagbeep=0;
            P5OUT = BIT0;
            P2OUT = 0x0;
        }
        flag=1;
    }
} 
/*******************************************
�������ƣ�Timer_A
��    �ܣ���ʱ��A���жϷ�����������������
          ����������
��    ������
����ֵ  ����
********************************************/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  P5OUT ^= BIT0;                            // Toggle P6.7
//flag=1;
}

/*******************************************
�������ƣ�PORT1_ISR 
��    �ܣ�P1�˿ڵ��жϷ������������������
          �Լ��̵��ַ�
��    ������
����ֵ  ����
********************************************/
 void  PORT(void)
{
    if(!(P5IN & BIT2))            //�����clock���ж�
    {
     // flag=1;//  P1IFG &=~ BIT7;         //����жϱ�־ 
        
        if(bitcount == 11)        //���յ�1λ
        {
            if(SIDval)          //���������ʼλ
                return;
            else  
                bitcount--;
        } 
        else if(bitcount == 2)    //������żУ��λ
        {   
            if(SIDval)          //���У��λ����1
                pebit = 1;
            else
                pebit = 0;
            bitcount--;
        }
        else if(bitcount == 1)    //����ֹͣλ
        {
            if(SIDval)          //��ֹͣλ��ȷ
            {
                bitcount = 11;    //��λλ��������
                if( Decode(recdata) )    //�����ô˼�ֵ��ASCIIֵ������
                   // LPM3_EXIT;           //�˳��͹���ģʽ
                recdata = 0;          //�����������
                flag=0;
            }
            else                //�������
            {
                bitcount = 11;
                recdata = 0;   
                flag=1;
            }
        }
        else                    //����8������λ
        {
            recdata >>= 1;
            if(SIDval)  recdata |= 0x80;
            bitcount--;
        }
   }
  /// else
   //  flag=0;
}




