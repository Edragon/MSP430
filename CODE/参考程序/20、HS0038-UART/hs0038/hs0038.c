#include "hs0038.h"
#include "../time/time.h"

#define t_1ms   5328    //83

/*******************************************
�������ƣ�HS0038_High
��    �ܣ�HS0038�����������
��    ������
����ֵ  ����
********************************************/
void HS0038_High()
{
  P6DIR |= BIT0;
  P6OUT |= BIT0;
}

/*******************************************
�������ƣ�HS0038_Low
��    �ܣ�HS0038�����������
��    ������
����ֵ  ����
********************************************/
void HS0038_Low()
{
  P6DIR |= BIT0;
  P6OUT &= ~BIT0;
}

/*******************************************
�������ƣ�Read_HS0038
��    �ܣ���HS0038����
��    ������
����ֵ  ����
********************************************/
uchar Read_HS0038()
{ 
  uchar stat;
  
  P6DIR &= ~BIT0;
  stat = P6IN;
  
  return (stat);
}


/*******************************************
�������ƣ�Decode_HS0038
��    �ܣ�������̷��͵�ֵ
��    ������
����ֵ  ������ֵ
********************************************/
uchar Decode_HS0038(void)
{  
  uchar HS0038_Value_Temp;
  uchar Err_Value = 'Z';
  uchar HS0038_Value;
  uchar Data_Decode;
  
  HS0038_High();                         //��ʼ��HS0038�������ø�
 
  do
  {
    HS0038_Value_Temp = Read_HS0038();  //��HS0038ֵ
  }while(HS0038_Value_Temp & BIT0);     //�ȴ���ֱ�������߱��
  Delay_ms(5);                          //��ʱ58����  
  HS0038_Value_Temp = Read_HS0038();    //��HS0038ֵ
  if(HS0038_Value_Temp & BIT0)          //�����Ϊ��
  {
    Delay_ms(50);
    return Err_Value; 
  }
  do
  {
    HS0038_Value_Temp = Read_HS0038();  //��HS0038ֵ
  }while(!(HS0038_Value_Temp & BIT0));  //�ȴ���ֱ�������߱��
  Delay_ms(3);                          //��ʱ3.5����  
//  Delay_us(500);
  HS0038_Value_Temp = Read_HS0038();    //��HS0038ֵ
  if(!(HS0038_Value_Temp & BIT0))          //�����Ϊ��
  {
    Delay_ms(50);
    return Err_Value; 
  } 
  Data_Decode = Decode_HS0038_Value();
  
  switch(Data_Decode)
  {
    case 0x2D:HS0038_Value='0'; break;//0
    case 0x19:HS0038_Value='1'; break;//1
    case 0x31:HS0038_Value='2'; break;//2
    case 0xBD:HS0038_Value='3'; break;//3
    case 0x11:HS0038_Value='4'; break;//4
    case 0x39:HS0038_Value='5'; break;//5
    case 0xB5:HS0038_Value='6'; break;//6
    case 0x85:HS0038_Value='7'; break;//7
    case 0xA5:HS0038_Value='8'; break;//8
    case 0x95:HS0038_Value='9'; break;//9
    case 0x8B:HS0038_Value='A'; break;//�ػ�    
    case 0x8F:HS0038_Value='B'; break;//MENU���˵�
    case 0x89:HS0038_Value='C'; break;//TEST������
    case 0x87:HS0038_Value='D'; break;//����
    case 0x2B:HS0038_Value='E'; break;//����
    case 0x81:HS0038_Value='F'; break;//+
    case 0x33:HS0038_Value='G'; break;//-
    case 0x0F:HS0038_Value='H'; break;//��
    case 0x13:HS0038_Value='I'; break;//��
    default:  HS0038_Value='Z'; break;
  }

  return HS0038_Value; 
}


/*******************************************
�������ƣ�Decode_HS0038
��    �ܣ�������̷��͵�ֵ
��    ������
����ֵ  ������ֵ
********************************************/
uchar Decode_HS0038_Value()
{ 
  uchar i, j, dat=0;
  uchar HS0038_Value_Temp;
  uchar HS0038_Code[4];
  uint Time_Value = 0;
  
  for(j=0;j<4;j++)              //�������ֵ��ǰ����Ϊ��ַ�룬������Ϊ����ֵ����
  { 
    for(i=8;i>0;i--)
    {
      dat>>=1; 
      do
      { 
        HS0038_Value_Temp = Read_HS0038();      //��HS0038ֵ 
      }while(!(HS0038_Value_Temp & BIT0));	//�ȴ������߱��
     
      Set_Timer();		                //���ߵ�ƽʱ��
      do
      {
        HS0038_Value_Temp = Read_HS0038();      //��HS0038ֵ
      }while(HS0038_Value_Temp & BIT0);	        //�ȴ������߱��
      Time_Value = TA1R;
      Stop_Timer();
      
      if(Time_Value > t_1ms)
        dat=dat|0x80;
      else 
        dat=dat&0x7f;  	  
      }
     HS0038_Code[j]=dat;
   }
  
  return(HS0038_Code[2]);    
}

