 /**********************************************************

file:  lcd.h

Author:	  gguoqing

**********************************************************/

#define  TYPE_LCD_DATA		1
#define  TYPE_LCD_COMMAND	0

#define uchar unsigned char
#define uint unsigned int
#define  DATA   P0

sbit LCD_RST  = P2^7;
sbit LCD_RD   = P3^2;
sbit LCD_WR   = P3^3;
sbit LCD_RS   = P2^5;
sbit LCD_CS   = P2^6;

extern  uint colors[];
extern  void  delay_ms(uint ms);
extern  void  LCD_Write(uchar type, uint value);
extern  void  LCD_SendData8(uchar value);
extern  void  LCD_Wirte_Data16(uint value);
extern  void  Reg_Write(uint reg,uint value);
extern  void  LCD_SetRamAddr(uint xStart, uint xEnd, uint yStart, uint yEnd);
extern  void  LCD_init(void);
extern  void  pic_play(uint Start_X, uint End_X,uint Start_Y,uint End_Y);
extern  void  LCD_clear(uchar n);


