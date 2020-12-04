void Delay_1ms(void);
void Delay(unsigned int n);
void Write_Cmd(unsigned char cod);
void Write_Data(unsigned char dat);
void Ini_Lcd(void);
void Disp_HZ(unsigned char addr,const unsigned char * pt,unsigned char num);
void Disp_ND(unsigned char addr,unsigned int  thickness);
void Draw_TX(unsigned char Yaddr,unsigned char Xaddr,const unsigned char * dp) ; 
void Draw_PM(const unsigned char *ptr);
