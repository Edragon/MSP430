#ifndef __TOUCH_H__
#define __TOUCH_H__

struct struct1                          //����һ��������Ϊstruct1�Ľṹ,����������Աx��y
{
  unsigned int x;                         //��Աx
  unsigned int y;                         //��Աy
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