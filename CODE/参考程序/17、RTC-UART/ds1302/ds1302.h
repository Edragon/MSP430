#ifndef __DS1302_H
#define __DS1302_H

void Ds1302_Write_Byte(unsigned char addr, unsigned char d);
unsigned char Ds1302_Read_Byte(unsigned char addr);
void Ds1302_Write_Time(void);
void Ds1302_Read_Time(void);
void Init_DS1302_Port();
void Ds1302_Init(void);
void Send_DS1302(void);
void Add_Ds1302_Time_Key(void); 
void Add_Ds1302_Time_Uart(void);

#endif