// ------------------  汉字字模的数据结构定义 ------------------------ //
struct  typFNT_GB16                 // 汉字字模数据结构 
{
       unsigned char  Index[3];               // 汉字内码索引	
       unsigned char   Msk[32];                        // 点阵码数据 
};

/////////////////////////////////////////////////////////////////////////
// 汉字字模表                                                          //
// 汉字库: 宋体16.dot,横向取模左高位,数据排列:从左到右从上到下         //
/////////////////////////////////////////////////////////////////////////
 const struct  typFNT_GB16 codeGB_16[] =          // 数据表 
{
"支", 0x01,0x00,0x01,0x00,0x01,0x04,0xFF,0xFE,
      0x01,0x00,0x01,0x00,0x3F,0xF0,0x08,0x20,
      0x08,0x20,0x04,0x40,0x02,0x80,0x01,0x00,
      0x02,0x80,0x0C,0x60,0x30,0x1E,0xC0,0x04,

"持", 0x10,0x40,0x10,0x40,0x10,0x50,0x13,0xF8,
      0xFC,0x40,0x10,0x48,0x17,0xFC,0x18,0x10,
      0x30,0x14,0xD7,0xFE,0x12,0x10,0x11,0x10,
      0x11,0x10,0x10,0x10,0x50,0x50,0x20,0x20,

"横", 0x11,0x10,0x11,0x10,0x17,0xFC,0x11,0x10,
      0xFD,0x14,0x17,0xFE,0x30,0x48,0x3B,0xFC,
      0x56,0x48,0x53,0xF8,0x92,0x48,0x13,0xF8,
      0x10,0x00,0x11,0x10,0x13,0x0C,0x14,0x04,

"竖", 0x04,0x00,0x25,0xFC,0x25,0x04,0x24,0x88,
      0x24,0x50,0x24,0x20,0x24,0x50,0x24,0x8E,
      0x07,0x04,0x01,0x10,0x3F,0xF8,0x08,0x20,
      0x08,0x20,0x04,0x44,0xFF,0xFE,0x00,0x00,

"屏", 0x00,0x08,0x3F,0xFC,0x20,0x08,0x20,0x08,
      0x3F,0xF8,0x22,0x20,0x21,0x48,0x2F,0xFC,
      0x22,0x20,0x22,0x24,0x3F,0xFE,0x22,0x20,
      0x22,0x20,0x42,0x20,0x84,0x20,0x08,0x20,

"电", 0x02,0x00,0x02,0x00,0x02,0x10,0x7F,0xF8,
      0x42,0x10,0x42,0x10,0x7F,0xF0,0x42,0x10,
      0x42,0x10,0x7F,0xF0,0x42,0x10,0x02,0x00,
      0x02,0x04,0x02,0x04,0x01,0xFC,0x00,0x00,

"话", 0x00,0x10,0x40,0x38,0x33,0xC0,0x10,0x40,
      0x00,0x40,0x00,0x44,0xF7,0xFE,0x10,0x40,
      0x10,0x48,0x13,0xFC,0x12,0x08,0x12,0x08,
      0x16,0x08,0x1A,0x08,0x13,0xF8,0x02,0x08,

"：", 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x00,
      0x0F,0x00,0x06,0x00,0x00,0x00,0x06,0x00,
      0x0F,0x00,0x0F,0x00,0x06,0x00,0x00,0x00,

"好", 0x10,0x00,0x11,0xFC,0x10,0x04,0x10,0x08,
      0xFC,0x10,0x24,0x20,0x24,0x24,0x27,0xFE,
      0x24,0x20,0x44,0x20,0x28,0x20,0x10,0x20,
      0x28,0x20,0x44,0x20,0x84,0xA0,0x00,0x40,

"钜", 0x10,0x08,0x15,0xFC,0x1F,0x00,0x21,0x00,
      0x21,0x08,0x7D,0xFC,0x91,0x08,0x15,0x08,
      0xFF,0x08,0x11,0xF8,0x11,0x00,0x11,0x00,
      0x15,0x00,0x19,0x04,0x11,0xFE,0x00,0x00,

"润", 0x04,0x04,0x43,0x7E,0x31,0x04,0x14,0x24,
      0x85,0xF4,0x44,0x44,0x44,0x44,0x14,0x44,
      0x15,0xF4,0x24,0x44,0xE4,0x44,0x24,0x44,
      0x25,0xF4,0x24,0x04,0x24,0x14,0x24,0x08,

"科", 0x04,0x10,0x0E,0x10,0xF8,0x90,0x08,0x50,
      0x08,0x10,0xFE,0x90,0x08,0x50,0x1C,0x14,
      0x1A,0x1E,0x29,0xF0,0x28,0x10,0x48,0x10,
      0x88,0x10,0x08,0x10,0x08,0x10,0x08,0x10,

"技", 0x10,0x40,0x10,0x40,0x10,0x48,0x13,0xFC,
      0xFC,0x40,0x10,0x40,0x10,0x40,0x13,0xF8,
      0x1A,0x08,0x31,0x10,0xD1,0x10,0x10,0xA0,
      0x10,0x40,0x10,0xB0,0x51,0x0E,0x26,0x04,
"寸", 0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x44,
      0xFF,0xFE,0x00,0x40,0x00,0x40,0x08,0x40,
      0x04,0x40,0x04,0x40,0x00,0x40,0x00,0x40,
      0x00,0x40,0x00,0x40,0x01,0x40,0x00,0x80
};
