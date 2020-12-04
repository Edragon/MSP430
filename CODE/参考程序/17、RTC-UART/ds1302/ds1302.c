/*

Ӳ��ԭ��ͼ��  DS1302_SCLK --> P7.5       
              DS1302_IO   --> P7.6
              DS1302_RST  --> P7.7

*/


#include "ds1302.h"
#include "../sys/sys.h"
#include "../uart/uart.h"


//��λ��
#define RST_CLR	P7OUT &= ~BIT7    //��ƽ�õ�
#define RST_SET	P7OUT |=  BIT7    //��ƽ�ø�

//˫������
#define DS1302_IO_OUT P7DIR |= BIT6
#define DS1302_IO_IN  P7DIR &= ~BIT6
#define IO_CLR	P7OUT &= ~BIT6    //��ƽ�õ�
#define IO_SET	P7OUT |=  BIT6    //��ƽ�ø�
#define IO_R	P7IN  &  BIT6     //��ƽ��ȡ

//ʱ���ź�
#define SCK_CLR	P7OUT &= ~BIT5    //ʱ���ź�
#define SCK_SET	P7OUT |=  BIT5    //��ƽ�ø�


#define ds1302_sec_add			0x80		//�����ݵ�ַ
#define ds1302_min_add			0x82		//�����ݵ�ַ
#define ds1302_hr_add			0x84		//ʱ���ݵ�ַ
#define ds1302_date_add			0x86		//�����ݵ�ַ
#define ds1302_month_add		0x88		//�����ݵ�ַ
#define ds1302_day_add			0x8a		//�������ݵ�ַ
#define ds1302_year_add			0x8c		//�����ݵ�ַ
#define ds1302_control_add		0x8e		//�������ݵ�ַ
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

extern unsigned char time_buf1[];
unsigned char time_buf[8] ;                         //��������ʱ������

extern uchar Uart_Array[];              //���ջ�������
extern int   Uart_Array_Num;   			//�������ݳ���
extern uchar Key_Value[];     
extern uint  Key_Value_Num;      

uchar DS1302_Time[20];

/*------------------------------------------------
��DS1302д��ʱ������
------------------------------------------------*/
void Ds1302_Write_Time(void) 
{
  Ds1302_Write_Byte(ds1302_control_add,0x00);			//�ر�д���� 
  Ds1302_Write_Byte(ds1302_sec_add,0x80);				//��ͣ 
  Ds1302_Write_Byte(ds1302_charger_add,0xa9);		//������ 
  
  Ds1302_Write_Byte(ds1302_year_add,time_buf1[1]);		//�� 
  Ds1302_Write_Byte(ds1302_month_add,time_buf1[2]);	//�� 
  Ds1302_Write_Byte(ds1302_date_add,time_buf1[3]);		//�� 
  Ds1302_Write_Byte(ds1302_day_add,time_buf1[7]);		//�� 
  Ds1302_Write_Byte(ds1302_hr_add,time_buf1[4]);		//ʱ 
  Ds1302_Write_Byte(ds1302_min_add,time_buf1[5]);		//��
  Ds1302_Write_Byte(ds1302_sec_add,time_buf1[6]);		//��
  Ds1302_Write_Byte(ds1302_day_add,time_buf1[7]);		//�� 
  Ds1302_Write_Byte(ds1302_control_add,0x80);			//��д���� 
  
  Send_DS1302();
}

/*------------------------------------------------
��DS1302����ʱ������
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
  time_buf[1]=Ds1302_Read_Byte(ds1302_year_add);		//�� 
  time_buf[2]=Ds1302_Read_Byte(ds1302_month_add);		//�� 
  time_buf[3]=Ds1302_Read_Byte(ds1302_date_add);		//�� 
  time_buf[4]=Ds1302_Read_Byte(ds1302_hr_add);		//ʱ 
  time_buf[5]=Ds1302_Read_Byte(ds1302_min_add);		//�� 
  time_buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;//�� 
  time_buf[7]=Ds1302_Read_Byte(ds1302_day_add);		//�� 
}

void Init_DS1302_Port()
{
  P7DIR |= BIT5 + BIT6 + BIT7;
}

/*------------------------------------------------
DS1302��ʼ��
------------------------------------------------*/
void Ds1302_Init(void)
{
  Init_DS1302_Port();
  
  RST_CLR;			//RST���õ�
  SCK_CLR;			//SCK���õ�
  Ds1302_Write_Byte(ds1302_sec_add,0x00);				 
}

/**************************************************
����: Send_DS1302
����: ������ʾ��ǰDS1302ʱ��
**************************************************/
void Send_DS1302(void)
{ 	
  Ds1302_Read_Time();  //��ȡʱ��    
  
  DS1302_Time[0] = '2';   //�������ǧλ
  DS1302_Time[1] = '0'; //��������λ 
  DS1302_Time[2] = (time_buf[1]>>4) + '0';   //�������ʮλ
  DS1302_Time[3] = (time_buf[1]&0x0F) + '0'; //��������λ 
  DS1302_Time[4] = '-';
  DS1302_Time[5] = (time_buf[2]>>4) + '0';   //�������ʮλ
  DS1302_Time[6] = (time_buf[2]&0x0F) + '0'; //������¸�λ 
  DS1302_Time[7] = '-';
  DS1302_Time[8] = (time_buf[3]>>4) + '0';   //�������ʮλ
  DS1302_Time[9] = (time_buf[3]&0x0F) + '0'; //������ո�λ 
  DS1302_Time[10] = ' ';
  DS1302_Time[11] = (time_buf[4]>>4) + '0';   //�����Сʱʮλ
  DS1302_Time[12] = (time_buf[4]&0x0F) + '0'; //�����Сʱ��λ 
  DS1302_Time[13] = ':';
  DS1302_Time[14] = (time_buf[5]>>4) + '0';   //���������ʮλ
  DS1302_Time[15] = (time_buf[5]&0x0F) + '0'; //��������Ӹ�λ
  DS1302_Time[16] = ' ';
  DS1302_Time[17] = (time_buf[6]>>4) + '0';   //���������ʮλ
  DS1302_Time[18] = (time_buf[6]&0x0F) + '0'; //��������Ӹ�λ
  DS1302_Time[19] = '\0';
  
  UART_SendString(DS1302_Time);
  UART_SendString("\r\n");
}


/*------------------------------------------------
��DS1302д��һ�ֽ�����
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{
  
  unsigned char i;
  RST_SET;	
  
  //д��Ŀ���ַ��addr
  addr = addr & 0xFE;     //���λ����
  for (i = 0; i < 8; i ++) 
  { 
    if (addr & 0x01) 
    {
      IO_SET;
    }
    else 
    {
      IO_CLR;
    }
    SCK_SET;
    SCK_CLR;
    addr = addr >> 1;
  }
  
  //д�����ݣ�d
  for (i = 0; i < 8; i ++) 
  {
    if (d & 0x01) 
    {
      IO_SET;
    }
    else 
    {
      IO_CLR;
    }
    SCK_SET;
    SCK_CLR;
    d = d >> 1;
  }
  RST_CLR;					//ֹͣDS1302����
}

/*------------------------------------------------
��DS1302����һ�ֽ�����
------------------------------------------------*/
unsigned char Ds1302_Read_Byte(unsigned char addr) 
{
  
  unsigned char i;
  unsigned char temp;
  RST_SET;	
  
  //д��Ŀ���ַ��addr
  addr = addr | 0x01;//���λ�ø�
  for (i = 0; i < 8; i ++) 
  {
    if (addr & 0x01) 
    {
      IO_SET;
    }
    else 
    {
      IO_CLR;
    }
    SCK_SET;
    SCK_CLR;
    addr = addr >> 1;
  }
  
  DS1302_IO_IN;
  //������ݣ�temp
  for (i = 0; i < 8; i ++) 
  {
    temp = temp >> 1;
    if (IO_R) 
    {
      temp |= 0x80;
    }
    else 
    {
      temp &= 0x7F;
    }
    SCK_SET;
    SCK_CLR;
  }
  DS1302_IO_OUT;
  
  RST_CLR;	//ֹͣDS1302����
  return temp;
}