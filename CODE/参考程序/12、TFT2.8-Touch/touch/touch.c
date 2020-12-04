#include "touch.h"
#include "../sys/sys.h"
#include "../lcd/lcd.h"

//Xmin��max��Ymin��Ymax�ֱ��Ǵ����������������С/���ֵ,���ƫ���ʱ���û������±궨
#define Xmin           0x0133
#define Xmax           0x0EDA
#define Ymin           0x00D0
#define Ymax           0x0E2F

/*********************Һ�������ܽŶ��� Start************************************/
#define PEN_CLR	        P4OUT &= ~BIT6           //PEN�õ�,����������ʱ��ADS7843Penirq������δ����ʱ�ĸߵ�ƽ��Ϊ�͵�ƽ
#define PEN_SET	        P4OUT |=  BIT6           //PEN�ø�
#define PEN             (P4IN & BIT6)            //P4.6�����ֵ 

#define TPDO_CLR	P7OUT &= ~BIT2           //TPDO�õ�   --> MIS0
#define TPDO_SET	P7OUT |=  BIT2           //TPDO�ø�
#define TPDOUT          (P7IN & BIT2)            //P7.2�����ֵ 

#define BUSY_CLR	P4OUT &= ~BIT5           //BUSY�õ�
#define BUSY_SET	P4OUT |=  BIT5           //BUSY�ø�

#define TPDI_CLR	P7OUT &= ~BIT3           //TPDI�õ�  --> MOSI
#define TPDI_SET	P7OUT |=  BIT3           //TPDI�ø�

#define TPCS_CLR	P4OUT &= ~BIT4           //TPCS�õ�
#define TPCS_SET	P4OUT |=  BIT4           //TPCS�ø�

#define TPCLK_CLR	P4OUT &= ~BIT7           //TPCLK�õ�
#define TPCLK_SET	P4OUT |=  BIT7           //TPCLK�ø�
/*********************Һ�������ܽŶ��� END************************************/  

struct struct1 coordinate;        //����һ����ΪTp_pix�Ľṹ����,���ڴ�Ŵ�����������

uint  lx,ly;                      //���ݲɼ����Ĳ���ת��ΪTFT�ϵ�ʵ������ֵ	

void Init_Touch_Port()
{
  P4DIR|= BIT4 + BIT5 + BIT7;     //TFT��ʾ������
  P7DIR|= BIT3;                   //TFT��ʾ������
}

void Init_Touch()
{
  Init_Touch_Port();
  TPCLK_CLR;
  TPCS_SET;
  TPDI_SET;
  TPCLK_SET;
  TPCS_CLR;  
}

//================================================================================================
//	ʵ�ֹ��ܣ���ȡ�ɼ����,����ȡ��ֵ
//  ȡ����ֵ���˴�ʹ������˲���2��ȡ����ֵ���̫�����Ϊ����
//================================================================================================
uchar Getpix() 
{
  uchar flag=0;
  struct struct1 pix1;
  struct struct1 pix2; 
  
  if(PEN==0)
  {  	
    pix1=AD_Touch();
    pix2=AD_Touch();
    
    if(pix_filter(pix1,pix2)==1) //�õ���ǰTP��ȡ��ֵ���˴�ʹ������˲���2��ȡ����ֵ���̫�����Ϊ����
    {
      if((coordinate.x>Xmin)&&(coordinate.y>Ymin))
      {
        lx=(uint)(240.0*(coordinate.x-Xmin)/(Xmax-Xmin));       //����ת���������ݲ���ֵ����ʵ������ֵ
        ly=(uint)(320.0*(coordinate.y-Ymin)/(Ymax-Ymin));   //Xmin��max��Ymin��Ymax�ֱ��Ǵ����������������С/���ֵ
        flag=1;
      }
    }				 
  }
  
  return flag; 	
}

//================================================================================================
//	ʵ�ֹ��ܣ���ȡ������X���Y���ѹֵ
//	���ز�����pix ��ȡ���Ĵ������ѹֵ
//================================================================================================
struct struct1 AD_Touch()            
{
  struct struct1 pix;
  TPCS_CLR;
  
  Write_Touch(0x90);                 //�Ϳ����� 10010000 ���ò�ַ�ʽ��X���꣬��ϸ����й�����
  TPCLK_SET; 
  Delay_us(1); 
  TPCLK_CLR; 
  Delay_us(1); 
  pix.y=Read_Touch();
  
  Write_Touch(0xD0);                 //�Ϳ����� 11010000 ���ò�ַ�ʽ��Y���� ��ϸ����й�����
  TPCLK_SET; 
  Delay_us(1); 
  TPCLK_CLR; 
  Delay_us(1); 
  pix.x=Read_Touch();
  
  TPCS_SET; 
  return pix;
}

//================================================================================================
//	ʵ�ֹ��ܣ�	д8λ�����������IC
//	���������  temp ��Ҫд���8λ��������
//================================================================================================
void Write_Touch(uchar temp)                         //SPIд8λ�����������IC
{
  uchar i=0;
  
  for(i=0;i<8;i++)                                    //ѭ��8��д��һ�ֽ�
  {
    if(temp&0x80)
      TPDI_SET;
    else 
      TPDI_CLR;    //�ж����λ�Ƿ�Ϊ1,Ϊ1��������λд1
    TPCLK_CLR; 
    Delay_us(1);                //��һ�����壬��������Ч����DINλ�������뵽IC
    TPCLK_SET; 
    Delay_us(1);                
    temp<<=1;                           //��д��������1λ��׼����д��һλ����
  }
}

//================================================================================================
//	ʵ�ֹ��ܣ�	�Ӵ�������IC��8λ���ݵ�������
//	���ز�����  temp ��Ҫд���8λ��������
//================================================================================================
uint Read_Touch()                          //SPI ������
{
  uchar i=0;
  uint temp=0;
  
  for(i=0;i<12;i++)                         //ѭ��12�ζ�ȡ12λ���
  { 
    temp<<=1;                                  //temp����һλ��׼����ȡ��һλ
    TPCLK_SET; 
    Delay_us(1);                   //�½�����Ч
    TPCLK_CLR; 
    Delay_us(1);
    if(TPDOUT) 
      temp++;                     //�жϿ���IC�ͳ���һλ�����Ƿ�Ϊ1,���Ϊ1,����temp�����λ
  }
  return(temp);                                 //���ؽ��
}	

//================================================================================================
//	ʵ�ֹ��ܣ�����˲����˵���������Ĳ�����
//	���ز�����flag �ɼ������Ƿ���Ч��־,flag=1;��������Ч
//================================================================================================
uchar pix_filter(struct struct1 pix1,struct struct1 pix2)
{
  uchar flag=0;
  int x=pix1.x>pix2.x?pix1.x-pix2.x:pix2.x-pix1.x;  //X�����β�������ֵ
  int y=pix1.y>pix2.y?pix1.y-pix2.y:pix2.y-pix1.y;  //Y�����β�������ֵ
  if(x<10&&y<10)	                                  //����˲���2��ȡ����ֵ���̫�����Ϊ����
  {
    flag=1;
    coordinate.x=(pix1.x+pix2.x)/2;           //�����β���ƽ��ֵ
    coordinate.y=(pix1.y+pix2.y)/2;
  }	
  return flag;
}

/********************************************************************
�ڴ�����λ�û���
********************************************************************/
void drawpoint(unsigned int x,unsigned int y,unsigned int color)
{
  LCD_SetPos(x,240-y,x+1,240-y-1);
  Write_Data(color);
  Write_Data(color);
  Write_Data(color);
  Write_Data(color);
} 