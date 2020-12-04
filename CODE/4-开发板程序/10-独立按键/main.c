
//����˵��������S1~S2�������۲�LED��ʾ
#include <msp430x14x.h>

#define keyin    (P1IN & 0x03)


void delay(void);

/********************������********************/
void main( void )
{
    unsigned char temp;
    
    WDTCTL = WDTPW + WDTHOLD;   //�رտ��Ź�
    //P1REN=0XFF;
   
    P1DIR = 0xfc;               //����P1.0~P.1Ϊ����״̬    
    P2DIR = 0xff;
    P2OUT = 0xff;
    while(1)
    {
        if(keyin != 0x03)       //��ֵ�иı�
        {
            delay();            //��ʱ����
            if(keyin != 0x03)   //�ٴμ�ⰴ��״̬
            {   
                temp=keyin; 
                
                while(keyin != 0x03);   //�ȴ��������ſ�
                switch(temp)    //ת����ֵ    
                {
                case 0x02:
                            P2OUT = 0xfe;break;
                case 0x01:
                            P2OUT = 0xfd;break;
               // case 0x0b:
               //            P2OUT = 0xfb;break;
               // case 0x07:
               //             P2OUT = 0xf7;break;
                default:
                            P2OUT = 0xff;break;
                }
                delay();delay();delay();delay();delay();delay();delay();delay();
            }
        }
    }
}
/*******************************************
�������ƣ�delay
��    �ܣ�������������ʱ
��    ������
����ֵ  ����
********************************************/
void delay(void)
{
    unsigned int tmp;
    
    for(tmp = 12000;tmp > 0;tmp--);
}
