/*********************************************************
程序功能：MCU的片内ADC对P6.0端口的电压进行转换
          将模拟电压值显示在1602液晶上。
*********************************************************/
#include  <msp430x14x.h>
#include  "cry1602.h"
#define uchar unsigned char
#define uint  unsigned int
#define   Num_of_Results   32

uchar shuzi[] = {"0123456789."};
uchar tishi[] = {"The volt is:"};

static uint results[Num_of_Results];    //保存ADC转换结果的数组                                                     // is not used for anything.
void Trans_val(uint Hex_Val);           

/************************主函数****************************/
void main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   //关闭看门狗
  LcdReset();                               //复位1602液晶
  DispNChar(2,0,12,tishi);                  //显示提示信息
  Disp1Char(11,1,'V');                      //显示电压单位
  P6SEL |= 0x01;                            // 使能ADC通道
  ADC12CTL0 = ADC12ON+SHT0_8+MSC;           // 打开ADC，设置采样时间
  ADC12CTL1 = SHP+CONSEQ_2;                 // 使用采样定时器
  ADC12IE = 0x01;                           // 使能ADC中断
  ADC12CTL0 |= ENC;                         // 使能转换
  ADC12CTL0 |= ADC12SC;                     // 开始转换
  
  _EINT();
  LPM0;
}

/*******************************************
函数名称：ADC12ISR
功    能：ADC中断服务函数，在这里用多次平均的
          计算P6.0口的模拟电压数值
参    数：无       
返回值  ：无
********************************************/
#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR (void)
{
  static uint index = 0;

  results[index++] = ADC12MEM0;               // Move results
  if(index == Num_of_Results)
  {
        uchar i;
        unsigned long sum = 0;

        index = 0;
        for(i = 0; i < Num_of_Results; i++)
        {
            sum += results[i];
        }
        sum >>= 5;                            //除以32
        
        Trans_val(sum);
  }
}

/*******************************************
函数名称：Trans_val
功    能：将16进制ADC转换数据变换成三位10进制
          真实的模拟电压数据，并在液晶上显示
参    数：Hex_Val--16进制数据
          n--变换时的分母等于2的n次方       
返回值  ：无
********************************************/
void Trans_val(uint Hex_Val)
{
    unsigned long caltmp;
    uint Curr_Volt;
    uchar t1,i;
    uchar ptr[4];
    
    caltmp = Hex_Val;
    caltmp = (caltmp << 5) + Hex_Val;           //caltmp = Hex_Val * 33
    caltmp = (caltmp << 3) + (caltmp << 1);     //caltmp = caltmp * 10
    Curr_Volt = caltmp >> 12;                   //Curr_Volt = caltmp / 2^n
    ptr[0] = Curr_Volt / 100;                   //Hex->Dec变换
    t1 = Curr_Volt - (ptr[0] * 100);
    ptr[2] = t1 / 10;
    ptr[3] = t1 - (ptr[2] * 10);
    ptr[1] = 10;                                //shuzi表中第10位对应符号"."
    //在液晶上显示变换后的结果
    for(i = 0;i < 4;i++)
      Disp1Char((6 + i),1,shuzi[ptr[i]]);
}
