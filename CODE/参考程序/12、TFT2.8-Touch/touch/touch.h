#ifndef __TOUCH_H__
#define __TOUCH_H__

struct struct1                          //定义一个类型名为struct1的结构,包含两个成员x和y
{
  unsigned int x;                         //成员x
  unsigned int y;                         //成员y
};

void Init_Touch_Port();
void Init_Touch();
unsigned char Getpix();
struct struct1 AD_Touch();
void Write_Touch(unsigned char temp);
unsigned int Read_Touch();
unsigned char pix_filter(struct struct1 pix1, struct struct1 pix2);
void drawpoint(unsigned int x,unsigned int y,unsigned int color);

#endif