#include <msp430x14x.h>
typedef unsigned int uint;
typedef unsigned char uchar;

#define BIT(x)	(1 << (x))

extern const unsigned char shuzi_table[];

void Send(uchar type,uchar transdata);
/**********液晶控制IO的宏定义*************/
#define cyCS      0    //P3.0，片选信号
#define cySID     1    //P3.1，串行数据  
#define cyCLK     2    //P3.2，同步时钟
#define cyPORT    P3OUT  
#define cyDDR     P3DIR

/*******************************************
函数名称：delay_Nus
功    能：延时N个us的时间
参    数：n--延时长度
返回值  ：无
********************************************/
void delay_Nus(uint n)
{
	uchar i;
	for(i = n;i > 0;i--)
        _NOP();
} 
/*******************************************
函数名称：delay_1ms
功    能：延时约1ms的时间
参    数：无
返回值  ：无
********************************************/
void delay_1ms(void)
{
	uchar i;
	for(i = 150;i > 0;i--)	  _NOP();
}  
/*******************************************
函数名称：delay_Nms
功    能：延时N个ms的时间
参    数：无
返回值  ：无
********************************************/  
void delay_Nms(uint n)
{
    uint i = 0;
    
    for(i = n;i > 0;i--)
        delay_1ms();
}
/*******************************************
函数名称：Ini_Lcd
功    能：初始化液晶模块
参    数：无
返回值  ：无
********************************************/
void Ini_Lcd(void)
{	
    cyDDR |= BIT(cyCLK) + BIT(cySID) + BIT(cyCS);   //相应的位端口设置为输出
    delay_Nms(100);                 //延时等待液晶完成复位
    Send(0,0x30);  /*功能设置:一次送8位数据,基本指令集*/
    delay_Nus(72);
    Send(0,0x02);  /*DDRAM地址归位*/
    delay_Nus(72);
    Send(0,0x0c);  /*显示设定:开显示,不显示光标,不做当前显示位反白闪动*/
    delay_Nus(72);
    Send(0,0x01);  /*清屏，将DDRAM的位址计数器调整为“00H”*/
    delay_Nus(72);
    Send(0,0x06);  /*功能设置，点设定:显示字符/光标从左到右移位,DDRAM地址加1*/
    delay_Nus(72);		
}  
/*******************************************
函数名称：Send
功    能：MCU向液晶模块发送1一个字节的数据
参    数：type--数据类型，0--控制命令，1--显示数据
          transdata--发送的数据
返回值  ：无
********************************************/
void Send(uchar type,uchar transdata)
{
    uchar firstbyte = 0xf8;
    uchar temp;
    uchar i,j = 3;

    if(type) firstbyte |= 0x02;
    
    cyPORT |= BIT(cyCS);		 	
    cyPORT &= ~BIT(cyCLK);		
    while(j > 0)
    {
            if(j == 3) temp = firstbyte;
            else if(j == 2) temp = transdata&0xf0;
            else  temp = (transdata << 4) & 0xf0;
            
            for(i = 8;i > 0;i--)
            {
                    if(temp & 0x80)	cyPORT |= BIT(cySID); 
                    else			cyPORT &= ~BIT(cySID);			
                    cyPORT |= BIT(cyCLK);				
                    temp <<= 1;
                    cyPORT &= ~BIT(cyCLK);
            }
            //三个字节之间一定要有足够的延时，否则易出现时序问题
            if(j == 3) delay_Nus(600); 
            else       delay_Nus(200);
            
            j--;
    } 
    
    cyPORT &= ~BIT(cySID);       
    cyPORT &= ~BIT(cyCS);		
}
