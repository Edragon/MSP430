/******************************************************************************
	            MSP430F5438A Taurus Ver4.2

功能：测试MSP430F5438A SD卡，将数据以TXT的形式写入SD卡，可以使用读卡器在电脑查看

说明：SD卡座在液晶背面

测试工具：读卡器、SD卡

编译环境：IAR5.3
Time：2015、8、1

By：天星科技
网址：gods-star.com
技术支持：天星科技.com
淘宝网店：gods-star.taobao.com
QQ：3184693939
******************************************************************************/

#include "main.h"

uchar Init_Flag = 9;
unsigned char buffer[512];
unsigned char Read_buffer[512];
unsigned long cardSize = 0;
unsigned char status = 1;
unsigned int timeout = 0, i;

void main()
{
  Init_System();                        //初始化系统
  UCSCTL5 |= DIVS_3;
  Init_SPI();                           // 初始化SPI
  Init_Flag = Init_SD();      	        // SD卡初始化  
  
  //Initialisation of the MMC/SD-card
  while (status != 0)                       // if return in not NULL an error did occur and the, MMC/SD-card will be initialized again 
  {
    status = initMMC();
    timeout++;
    if (timeout == 50)                      // Try 50 times till error
    {
      UART_SendString ("No MMC/SD-card found!!");
      break;
    }
  }
  
  UART_SendString("System Start...\r\n");
  while ((mmc_ping() != MMC_SUCCESS));      // Wait till card is inserted
  cardSize =  MMC_ReadCardSize();
  ReadBPB();
  CreateFile("TEST0001TXT");

  //OperateFile
  for (i = 0; i <= 512; i++) buffer[i] = 'A';
  OperateFile(1,"TEST0001TXT",0,512,buffer);
  for (i = 0; i <= 512; i++) buffer[i] = 'B';  
  OperateFile(1,"TEST0001TXT",512,512,buffer);
  for (i = 0; i <= 512; i++) buffer[i] = 'C';  
  OperateFile(1,"TEST0001TXT",1024,512,buffer);
  for (i = 0; i <= 512; i++) buffer[i] = 'D';  
  OperateFile(1,"TEST0001TXT",1536,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'E';  
  OperateFile(1,"TEST0001TXT",2048,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'F';  
  OperateFile(1,"TEST0001TXT",2560,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'G';  
  OperateFile(1,"TEST0001TXT",3072,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'H';  
  OperateFile(1,"TEST0001TXT",3584,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'I';  
  OperateFile(1,"TEST0001TXT",4096,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'J';  
  OperateFile(1,"TEST0001TXT",4608,512,buffer); 
  for (i = 0; i <= 512; i++) buffer[i] = 'K';  
  OperateFile(1,"TEST0001TXT",5120,5,buffer);  
  
  //读FAT表
  ReadBlock(FATStartSec());
  //读根目录
  ReadBlock(DirStartSec());  
  //读数据开始区
  ReadBlock(DataStartSec());  
//  DelFile("TEST0001TXT");
  //读根目录区
  //ReadBlock(DirStartSec());
  
  mmc_GoIdle();                             // set MMC in Idle mode
  
  UART_SendString("-->System Over...\r\n");
  
  while(1);
}