#ifndef __DS18B20_H
#define __DS18B20_H

#include "../sys/sys.h"

uchar Init_18B20(void);
void Write_18B20(uchar wdata);
uchar Read_18B20(void);
void Skip(void);
void Convert(void);
void Read_SP(void);
uint Read_Temp(void);
uint Ds18b20_Convert(void);
void Disp_Numb(uint temper);

#endif