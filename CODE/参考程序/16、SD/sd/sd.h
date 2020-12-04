#ifndef __SD_H__
#define __SD_H__

void Init_SPI_Port();
void Init_SPI();
unsigned char Init_SD(void);
unsigned char SdWrite(unsigned char n);
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC);
unsigned char SdResponse();
unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len);
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len);

#endif