#include "IIC.h"
typedef unsigned char uchar;
typedef unsigned int  uint;

#define deviceaddress 0xa0  //AT24C02的设备地址

/*******************************************
函数名称：delay_10ms
功    能：延时约6ms，等待EEPROM完成内部写入
参    数：无
返回值  ：无
********************************************/
void delay_10ms(void)
{
	uint i = 1000;
	while(i--);
}
/*******************************************
函数名称：Write_1Byte
功    能：向EEPROM中写入1个字节的数据
参    数：Wdata--写入的数据
          dataaddress--数据的写入地址
返回值  ：写入结果：1--成功，0--失败
********************************************/
uchar Write_1Byte(uchar wdata,uchar dataaddress)
{
	start();
	write1byte(deviceaddress);
	if(check()) 
        write1byte(dataaddress);
	else 
        return 0;
	if(check()) 
        write1byte(wdata);
	else 
        return 0;
	if(check())	    stop();
	else            return 0;
    
    delay_10ms();       //等待EEPROM完成内部写入
	return 1; 
}
/*******************************************
函数名称：Write_NByte
功    能：向EEPROM中写入N个字节的数据
参    数：outbuf--指向写入数据存放首地址的指针
          n--数据个数，最大不能超过8，由页地址
             决定其最大长度
          dataaddress--数据写入的首地址
返回值  ：写入结果：1--成功，0--失败
********************************************/
uchar Write_NByte(uchar * outbuf,uchar n,uchar dataaddress)
{
	uchar  flag; 
    
	start();
	write1byte(deviceaddress);                  //写入器件地址
	if(check() == 1) 
        write1byte(dataaddress);                //写入数据字地址
	else 
        return 0;
	if(check()) 
        flag=writeNbyte(outbuf,n);
	else 
        return 0;
    delay_10ms();       //等待EEPROM完成内部写入
	if(flag)    return 1;
	else        return 0;

}
/*******************************************
函数名称：Read_1Byte_currentaddress
功    能：从EEPROM的当前地址读取1个字节的数据
参    数：无
返回值  ：读取的数据
********************************************/
uchar Read_1Byte_currentaddress(void)
{
	uchar temp;
    
	start();
	write1byte((deviceaddress|0x01));
	if(check())	
        temp = read1byte();
	else 
        return 0;
	mnack();
	stop();
	return temp;
}
/*******************************************
函数名称：Read_NByte_currentaddress
功    能：从EEPROM的当前地址读取N个字节的数据
参    数：readbuf--指向保存数据地址的指针
          n--读取数据的个数
返回值  ：读取结果：1--成功，0--失败
********************************************/
uchar Read_NByte_currentaddress(uchar * readbuf,uchar n)
{   
    start();
	write1byte((deviceaddress|0x01));
	if(check()) 
        readNbyte(readbuf,n);
	else 
        return 0;
    
	return  1;
}

/*******************************************
函数名称：Read_1Byte_Randomaddress
功    能：从EEPROM的指定地址读取1个字节的数据
参    数：dataaddress--数据读取的地址
返回值  ：读取的数据
********************************************/
uchar Read_1Byte_Randomaddress(uchar dataaddress)
{
	uchar temp;
    
	start();
	write1byte(deviceaddress);
	if(check())	
        write1byte(dataaddress);
	else 
        return 0;
	if(check()) 
	{
		start();
		write1byte((deviceaddress|0x01));
	}
	else    
        return 0;
	if(check())	
        temp = read1byte();
	else 
        return 0;

	mnack();
	stop();
	return temp;
}
/*******************************************
函数名称：Read_NByte_Randomaddress
功    能：从EEPROM的指定地址读取N个字节的数据
参    数：readbuf--指向保存数据地址的指针
          n--读取数据的个数
          dataaddress--数据读取的首地址
返回值  ：读取结果：1--成功，0--失败
********************************************/
uchar Read_NByte_Randomaddress(uchar * readbuf,uchar n,uchar dataaddress)
{
	start();
	write1byte(deviceaddress);
	if(check())	
        write1byte(dataaddress);
	else 
        return 0;
	if(check()) 
	{
		start();
		write1byte(deviceaddress|0x01);
	}
	else 
        return 0;
	if(check()) 
        readNbyte(readbuf,n);
	else 
        return 0;
	
	return 1;	
}
