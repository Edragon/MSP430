#define BufferSize  32      //显示缓存大小
unsigned char bitcount=11;          //位计数变量
unsigned char kb_buffer[BufferSize];    //显示缓存
unsigned char input=0;       //数据压入缓存位置指针
unsigned char output=0;      //数据弹出缓存位置指针   
unsigned char pebit=0xff;    //奇偶校验标志位
unsigned char recdata=0;     //接收到的数据
