#ifndef __GUI_H__
#define __GUI_H__

void GUIcircle(unsigned int xc,unsigned int yc,unsigned int r,unsigned int circle_colour);
void plotC(unsigned int x,unsigned int y,unsigned int xc,int yc,unsigned int yc_colour);
void GUIfull(unsigned  int x0,unsigned  int y0,unsigned  int x1,unsigned  int y1,unsigned  int full_colour);
void GUIDotline(unsigned  int x0,unsigned  int y0,unsigned  int x1,unsigned  int y1,unsigned  int line_color);
void GUIline(unsigned  int x0,unsigned  int y0,unsigned  int x1,unsigned  int y1,unsigned  int line_color);
void GUIpoint(unsigned int x,unsigned int y,unsigned int color);
void GUITable(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned char TH,unsigned char TL,unsigned int color);

#endif