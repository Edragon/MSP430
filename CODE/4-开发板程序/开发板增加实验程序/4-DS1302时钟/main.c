/****************************************************
程序功能：在1602液晶上显示一个数字日历，可以通过
          四个按键来设置各种参数
-----------------------------------------------------
测试说明：（1）按K1键进入设置模式并可以选择更改参
               数的位置，
          （2）按K2键单方向增加数值
          （3）按K3键放弃当前修改回到工作模式
          （4）按K4键保存当前数值回到工作模式
           实际按键观察测试。
******************************************************/
#include <msp430x14x.h>
#include "cry1602.h"
#include "DS1302.h"

//顺序：秒，分，时，日，月，星期，年；格式：BCD
unsigned char times[7];
//液晶显示数字编码
unsigned char shuzi[] = {"0123456789"};
//游标位置变量
unsigned char PP = 0;
//是否处于修改状态标志，1--是，0--否
unsigned char cflag = 0;

unsigned char Key4Scan(void);
void ShowTime(void);
/****************主函数****************/
void main(void)
{    
    WDTCTL = WDTPW + WDTHOLD;   //关闭看门狗    
   // P1DIR = 0x80;               //P1.7设置为输出，其余为输入
   // P1OUT = 0x00;
    
    Reset_DS1302();             //初始化DS1302
    LcdReset();                 //初始化液晶
    while(1)
    {
        if(!cflag)
        {
            Get_DS1302(times);          //获取时间数据
            ShowTime();                 //转换显示
        }
        
        switch(Key4Scan())
        {
        case 0x01:
                    switch(PP++)      //确定游标地址
                    {
                    case 0: LocateXY(4,0);break;
                    case 1: LocateXY(7,0);break;
                    case 2: LocateXY(10,0);break;
                    case 3: LocateXY(13,0);break;
                    case 4: LocateXY(4,1);break;
                    case 5: LocateXY(7,1);break;
                    case 6: LocateXY(10,1);break;
                    default:break;
                    }                  
                    LcdWriteCommand(0x0f, 1);   //打开游标
                    if(PP == 7) PP = 0;
                    cflag = 1;          //标志置位
                    break;
        case 0x02:
                    if(cflag)
                    {
                        switch(PP)
                        {
                        case 1:         //年
                                times[6]++;
                                if((times[6]&0x0f) == 0x0a)
                                {
                                    times[6] += 0x06;       
                                }
                                if(times[6] > 0x99)
                                {
                                    times[6] = 0x00;    
                                }
                                Disp1Char(3,0,shuzi[times[6]>>4]);
                                Disp1Char(4,0,shuzi[times[6]&0x0f]);
                                LocateXY(4,0); 
                                break;
                        case 2:         //月
                                times[4]++;
                                if((times[4]&0x0f) == 0x0a)
                                {
                                    times[4] += 0x06;       
                                }
                                if(times[4] > 0x12)
                                {
                                    times[4] = 0x01;    
                                }
                                Disp1Char(6,0,shuzi[times[4]>>4]);
                                Disp1Char(7,0,shuzi[times[4]&0x0f]);
                                LocateXY(7,0);
                                break;
                         case 3:         //日
                                times[3]++;
                                if((times[3]&0x0f) == 0x0a)
                                {
                                    times[3] += 0x06;       
                                }
                                if(times[3] > 0x31)
                                {
                                    times[3] = 0x01;    
                                }
                                Disp1Char(9,0,shuzi[times[3]>>4]);
                                Disp1Char(10,0,shuzi[times[3]&0x0f]);
                                LocateXY(10,0);
                                break;
                         case 4:         //周
                                times[5]++;
                                if((times[5]&0x0f) == 0x08)
                                {
                                    times[5] = 0x01;       
                                }
                                Disp1Char(13,0,shuzi[times[5]]);
                                LocateXY(13,0);
                                break;
                         case 5:         //时
                                times[2]++;
                                if((times[2]&0x0f) == 0x0a)
                                {
                                    times[2] += 0x06;       
                                }
                                if(times[2] > 0x23)
                                {
                                    times[2] = 0x00;    
                                }
                                Disp1Char(3,1,shuzi[times[2]>>4]);
                                Disp1Char(4,1,shuzi[times[2]&0x0f]);
                                LocateXY(4,1);
                                break;
                         case 6:         //分
                                times[1]++;
                                if((times[1]&0x0f) == 0x0a)
                                {
                                    times[1] += 0x06;       
                                }
                                if(times[1] > 0x59)
                                {
                                    times[1] = 0x00;    
                                }
                                Disp1Char(6,1,shuzi[times[1]>>4]);
                                Disp1Char(7,1,shuzi[times[1]&0x0f]);
                                LocateXY(7,1);
                                break;
                         case 0:         //时
                                times[0]++;
                                if((times[0]&0x0f) == 0x0a)
                                {
                                    times[0] += 0x06;       
                                }
                                if(times[0] > 0x59)
                                {
                                    times[0] = 0x00;    
                                }
                                Disp1Char(9,1,shuzi[times[0]>>4]);
                                Disp1Char(10,1,shuzi[times[0]&0x0f]);
                                LocateXY(10,1);
                                break;
                        default:
                                break;                            
                                
                        }
                    }
                    break;
        case 0x03:
                    if(cflag)
                    {
                        cflag = 0;
                        PP = 0;
                        LcdWriteCommand(0x0c, 1);   //关闭游标
                    }
                    break;
        case 0x04:
                    if(cflag)
                    {
                        cflag = 0;
                        PP = 0;
                        LcdWriteCommand(0x0c, 1);   //关闭游标
                        Set_DS1302(times);
                    }
                    break; 
        default:
                    break;                    
        }
    }
}
/*******************************************
函数名称：ShowTime
功    能：将DS1302的时间转换成10进制显示
参    数：无
返回值  ：无
********************************************/    
void ShowTime(void)
{
    unsigned char h1[14];       //第1行显示数据
    unsigned char h2[8];        //第2行显示数据
    
    h1[0] = shuzi[2];
    h1[1] = shuzi[0];
    h1[2] = shuzi[times[6]>>4];     //年
    h1[3] = shuzi[times[6]&0x0f];
    h1[4] = 0x2d;       //"-"
    h1[5] = shuzi[times[4]>>4];     //月
    h1[6] = shuzi[times[4]&0x0f];
    h1[7] = 0x2d;       //"-"
    h1[8] = shuzi[times[3]>>4];     //日
    h1[9] = shuzi[times[3]&0x0f];
    h1[10] = 0x20;      //" "
    h1[11] = 0x2a;      //"*"
    h1[12] = shuzi[times[5]];       //周
    h1[13] = 0x2a;      //"*"
    DispNChar(1,0,14,h1);           //在第一行显示
   
    h2[0] = shuzi[times[2]>>4];     //时
    h2[1] = shuzi[times[2]&0x0f];
    h2[2] = 0x3a;       //":"
    h2[3] = shuzi[times[1]>>4];     //分
    h2[4] = shuzi[times[1]&0x0f];
    h2[5] = 0x3a;       //":"
    h2[6] = shuzi[times[0]>>4];     //秒
    h2[7] = shuzi[times[0]&0x0f];
    DispNChar(3,1,8,h2);           //在第二行显示
}
