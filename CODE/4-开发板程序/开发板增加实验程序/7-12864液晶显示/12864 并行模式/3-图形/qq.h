//阴码格式的图形
const unsigned char qq1[] = 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x7F,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,
0x00,0x07,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xC0,0x00,0x03,0xFF,0xC0,0x00,0x00,
0x00,0x0F,0xFF,0xFF,0xFC,0x00,0x00,0x0F,0xFF,0xE0,0x00,0x0F,0xFF,0xE0,0x00,0x00,
0x00,0x1F,0xF3,0xFB,0xFE,0x00,0x00,0x1F,0xFF,0xF0,0x00,0x1F,0xFF,0xF0,0x00,0x00,
0x00,0x1F,0xE1,0xE1,0xFF,0x00,0x00,0x3F,0xFF,0xF8,0x00,0x3F,0xFF,0xF8,0x00,0x00,
0x00,0x3F,0xE0,0xE0,0xFF,0x00,0x00,0x7F,0xFF,0xF8,0x00,0x7F,0xFF,0xF8,0x00,0x00,
0x00,0x3F,0xC2,0xE0,0xFF,0x00,0x00,0xFF,0xFF,0xFC,0x00,0xFF,0xFF,0xFC,0x00,0x00,
0x00,0x7F,0xC2,0xCC,0xFF,0x80,0x01,0xFF,0xFF,0xFC,0x01,0xFF,0xFF,0xFC,0x00,0x00,
0x00,0x7F,0xC7,0xC0,0xFF,0x80,0x03,0xFF,0xFF,0xFC,0x03,0xFF,0xFF,0xFC,0x00,0x00,
0x00,0x7F,0xC0,0xC0,0xFF,0xC0,0x03,0xFF,0xDF,0xFE,0x03,0xFF,0xDF,0xFE,0x00,0x00,
0x00,0xFF,0xE1,0xE0,0xFF,0x80,0x07,0xFF,0x0F,0xFE,0x07,0xFF,0x0F,0xFE,0x00,0x00,
0x00,0x7F,0xE1,0xF1,0xFF,0xC0,0x07,0xFF,0x07,0xFF,0x07,0xFF,0x03,0xFF,0x00,0x00,
0x00,0xFF,0xFF,0xFF,0xFF,0xC0,0x0F,0xFE,0x03,0xFE,0x0F,0xFE,0x03,0xFE,0x00,0x00,
0x00,0xFF,0xFF,0xFF,0xFF,0xE0,0x0F,0xFE,0x01,0xFF,0x0F,0xFE,0x01,0xFF,0x00,0x00,
0x00,0xFF,0xC0,0x01,0x7F,0xE0,0x1F,0xFC,0x01,0xFF,0x1F,0xFC,0x01,0xFF,0x00,0x00,
0x01,0xFF,0x00,0x00,0x1F,0xE0,0x1F,0xFC,0x01,0xFF,0x1F,0xFC,0x01,0xFF,0x00,0x00,
0x01,0xFC,0x00,0x00,0x0F,0xE0,0x3F,0xF8,0x00,0xFF,0x3F,0xF8,0x00,0xFF,0x00,0x00,
0x01,0xFF,0x60,0x00,0xBF,0xE0,0x1F,0xF8,0x01,0xFF,0x1F,0xF8,0x00,0xFF,0x00,0x00,
0x03,0xFF,0xF0,0x03,0xFF,0xE0,0x3F,0xF0,0x7B,0xFF,0x3F,0xF8,0x7B,0xFF,0x00,0x00,
0x03,0xFF,0xFF,0xBF,0xFF,0xF0,0x1F,0xF8,0x6E,0x1F,0x1F,0xF8,0x6F,0x3F,0x00,0x00,
0x0F,0xFF,0xFF,0xFF,0xFF,0xF0,0x3F,0xF0,0xCC,0x0E,0x3F,0xF0,0x4C,0x1F,0x00,0x00,
0x0F,0xFF,0xFF,0xFF,0xFF,0xF8,0x1F,0xF8,0xD8,0xC7,0x1F,0xF8,0xCC,0xC7,0x00,0x00,
0x3F,0x8F,0xFF,0xFF,0xF9,0xFC,0x1F,0xFD,0xD9,0x01,0x1F,0xFD,0xDB,0x01,0x00,0x00,
0x3F,0x87,0xFF,0xFF,0xE0,0xFC,0x0F,0xFF,0xF8,0x00,0x0F,0xFF,0xFD,0x00,0x00,0x00,
0x7F,0x8F,0xFF,0xFF,0x00,0xFE,0x0F,0xFF,0xFC,0x00,0x0F,0xFF,0xFC,0x00,0x00,0x00,
0x7F,0x87,0xFB,0xF4,0x00,0x7E,0x03,0xFF,0xFE,0x00,0x43,0xFF,0xFE,0x00,0x40,0x00,
0x7F,0x0F,0xE0,0x00,0x00,0x7E,0x01,0xFF,0xFE,0x00,0x81,0xFF,0xFE,0x00,0x80,0x00,
0xFF,0x07,0xF0,0x00,0x00,0x7E,0x00,0x3F,0xFF,0x00,0x80,0x3F,0xFF,0x81,0x80,0x00,
0xFF,0x0F,0xE0,0x00,0x00,0x7E,0x00,0x00,0x01,0x83,0x00,0x00,0x01,0xC7,0x00,0x00,
0xFF,0x87,0xF0,0x00,0x00,0x7F,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0xFE,0x00,0x00,
0xF7,0x07,0xE0,0x00,0x00,0x7E,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x38,0x00,0x00,
0xF3,0x83,0xE0,0x00,0x00,0x4E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x43,0x80,0x00,0x00,0x00,0xCE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0xC0,0x00,0x00,0x00,0x80,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x01,0xC0,0x00,0x00,0x01,0x80,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x00,0x60,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x70,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0x38,0x00,0x00,0x0C,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x02,0x0E,0x00,0x00,0x18,0xC0,0x0F,0xF0,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0xE0,
0x03,0x03,0x80,0x00,0x70,0x20,0x06,0x60,0x00,0x00,0x00,0x06,0xC0,0x00,0x00,0x60,
0x02,0x01,0xF0,0x01,0xC0,0x20,0x06,0x60,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x60,
0x02,0x00,0x07,0x5A,0x00,0x20,0x03,0xC3,0xC7,0xEF,0xF0,0x0C,0x03,0xC3,0xC3,0xE0,
0x01,0x40,0x16,0x1D,0x45,0x40,0x03,0xC6,0x63,0x86,0x60,0x0C,0x06,0x66,0x66,0x60,
0x00,0x00,0xA8,0x00,0x00,0x00,0x03,0xC7,0xE3,0x03,0xC0,0x0D,0xE6,0x66,0x66,0x60,
0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC6,0x03,0x03,0xC0,0x0C,0xC6,0x66,0x66,0x60,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x86,0x63,0x01,0x80,0x06,0xC6,0x66,0x66,0x60,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x83,0xC7,0xC1,0x80,0x03,0x83,0xC3,0xC3,0xF0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//阳码格式的图形
const unsigned char qq2[] = 
{

0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xBF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xF0,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x80,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFE,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFC,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,
0xFF,0xF8,0x00,0x00,0x03,0xFF,0xFF,0xFC,0x00,0x3F,0xFF,0xFC,0x00,0x3F,0xFF,0xFF,
0xFF,0xF0,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x1F,0xFF,0xF0,0x00,0x1F,0xFF,0xFF,
0xFF,0xE0,0x0C,0x04,0x01,0xFF,0xFF,0xE0,0x00,0x0F,0xFF,0xE0,0x00,0x0F,0xFF,0xFF,
0xFF,0xE0,0x1E,0x1E,0x00,0xFF,0xFF,0xC0,0x00,0x07,0xFF,0xC0,0x00,0x07,0xFF,0xFF,
0xFF,0xC0,0x1F,0x1F,0x00,0xFF,0xFF,0x80,0x00,0x07,0xFF,0x80,0x00,0x07,0xFF,0xFF,
0xFF,0xC0,0x3D,0x1F,0x00,0xFF,0xFF,0x00,0x00,0x03,0xFF,0x00,0x00,0x03,0xFF,0xFF,
0xFF,0x80,0x3D,0x33,0x00,0x7F,0xFE,0x00,0x00,0x03,0xFE,0x00,0x00,0x03,0xFF,0xFF,
0xFF,0x80,0x38,0x3F,0x00,0x7F,0xFC,0x00,0x00,0x03,0xFC,0x00,0x00,0x03,0xFF,0xFF,
0xFF,0x80,0x3F,0x3F,0x00,0x3F,0xFC,0x00,0x20,0x01,0xFC,0x00,0x20,0x01,0xFF,0xFF,
0xFF,0x00,0x1E,0x1F,0x00,0x7F,0xF8,0x00,0xF0,0x01,0xF8,0x00,0xF0,0x01,0xFF,0xFF,
0xFF,0x80,0x1E,0x0E,0x00,0x3F,0xF8,0x00,0xF8,0x00,0xF8,0x00,0xFC,0x00,0xFF,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x3F,0xF0,0x01,0xFC,0x01,0xF0,0x01,0xFC,0x01,0xFF,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x1F,0xF0,0x01,0xFE,0x00,0xF0,0x01,0xFE,0x00,0xFF,0xFF,
0xFF,0x00,0x3F,0xFE,0x80,0x1F,0xE0,0x03,0xFE,0x00,0xE0,0x03,0xFE,0x00,0xFF,0xFF,
0xFE,0x00,0xFF,0xFF,0xE0,0x1F,0xE0,0x03,0xFE,0x00,0xE0,0x03,0xFE,0x00,0xFF,0xFF,
0xFE,0x03,0xFF,0xFF,0xF0,0x1F,0xC0,0x07,0xFF,0x00,0xC0,0x07,0xFF,0x00,0xFF,0xFF,
0xFE,0x00,0x9F,0xFF,0x40,0x1F,0xE0,0x07,0xFE,0x00,0xE0,0x07,0xFF,0x00,0xFF,0xFF,
0xFC,0x00,0x0F,0xFC,0x00,0x1F,0xC0,0x0F,0x84,0x00,0xC0,0x07,0x84,0x00,0xFF,0xFF,
0xFC,0x00,0x00,0x40,0x00,0x0F,0xE0,0x07,0x91,0xE0,0xE0,0x07,0x90,0xC0,0xFF,0xFF,
0xF0,0x00,0x00,0x00,0x00,0x0F,0xC0,0x0F,0x33,0xF1,0xC0,0x0F,0xB3,0xE0,0xFF,0xFF,
0xF0,0x00,0x00,0x00,0x00,0x07,0xE0,0x07,0x27,0x38,0xE0,0x07,0x33,0x38,0xFF,0xFF,
0xC0,0x70,0x00,0x00,0x06,0x03,0xE0,0x02,0x26,0xFE,0xE0,0x02,0x24,0xFE,0xFF,0xFF,
0xC0,0x78,0x00,0x00,0x1F,0x03,0xF0,0x00,0x07,0xFF,0xF0,0x00,0x02,0xFF,0xFF,0xFF,
0x80,0x70,0x00,0x00,0xFF,0x01,0xF0,0x00,0x03,0xFF,0xF0,0x00,0x03,0xFF,0xFF,0xFF,
0x80,0x78,0x04,0x0B,0xFF,0x81,0xFC,0x00,0x01,0xFF,0xBC,0x00,0x01,0xFF,0xBF,0xFF,
0x80,0xF0,0x1F,0xFF,0xFF,0x81,0xFE,0x00,0x01,0xFF,0x7E,0x00,0x01,0xFF,0x7F,0xFF,
0x00,0xF8,0x0F,0xFF,0xFF,0x81,0xFF,0xC0,0x00,0xFF,0x7F,0xC0,0x00,0x7E,0x7F,0xFF,
0x00,0xF0,0x1F,0xFF,0xFF,0x81,0xFF,0xFF,0xFE,0x7C,0xFF,0xFF,0xFE,0x38,0xFF,0xFF,
0x00,0x78,0x0F,0xFF,0xFF,0x80,0xFF,0xFF,0xFF,0x01,0xFF,0xFF,0xFF,0x01,0xFF,0xFF,
0x08,0xF8,0x1F,0xFF,0xFF,0x81,0xFF,0xFF,0xFF,0xC7,0xFF,0xFF,0xFF,0xC7,0xFF,0xFF,
0x0C,0x7C,0x1F,0xFF,0xFF,0xB1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xBC,0x7F,0xFF,0xFF,0xFF,0x31,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFE,0x3F,0xFF,0xFF,0xFF,0x7F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFE,0x3F,0xFF,0xFF,0xFE,0x7F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x9F,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0x8F,0xFF,0xFF,0xF9,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFE,0xC7,0xFF,0xFF,0xF3,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFD,0xF1,0xFF,0xFF,0xE7,0x3F,0xF0,0x0F,0xFF,0xFF,0xFF,0xFC,0x3F,0xFF,0xFF,0x1F,
0xFC,0xFC,0x7F,0xFF,0x8F,0xDF,0xF9,0x9F,0xFF,0xFF,0xFF,0xF9,0x3F,0xFF,0xFF,0x9F,
0xFD,0xFE,0x0F,0xFE,0x3F,0xDF,0xF9,0x9F,0xFF,0xFF,0xFF,0xF3,0xFF,0xFF,0xFF,0x9F,
0xFD,0xFF,0xF8,0xA5,0xFF,0xDF,0xFC,0x3C,0x38,0x10,0x0F,0xF3,0xFC,0x3C,0x3C,0x1F,
0xFE,0xBF,0xE9,0xE2,0xBA,0xBF,0xFC,0x39,0x9C,0x79,0x9F,0xF3,0xF9,0x99,0x99,0x9F,
0xFF,0xFF,0x57,0xFF,0xFF,0xFF,0xFC,0x38,0x1C,0xFC,0x3F,0xF2,0x19,0x99,0x99,0x9F,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x39,0xFC,0xFC,0x3F,0xF3,0x39,0x99,0x99,0x9F,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x79,0x9C,0xFE,0x7F,0xF9,0x39,0x99,0x99,0x9F,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x7C,0x38,0x3E,0x7F,0xFC,0x7C,0x3C,0x3C,0x0F,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
