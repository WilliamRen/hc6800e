/********************************************************************************
*  描述:                LCD1602 滚动显示                                       *
* 显示方式：                                                                   *
* 1、从左到右逐字显示，闪动二次，清屏。                                        *
* 2、再从右到左逐字显示，闪动二次，清屏。                                      *
* 3、周期性地重复上述显示方式。                                                *                            
********************************************************************************/

#include <reg51.h>
#include <intrins.h>

typedef unsigned char BYTE;
typedef bit BOOL;  

sbit LCD_RS = P2^6;             
sbit LCD_RW = P2^5;
sbit LCD_EP = P2^7;

BYTE code dis1[] = {"   WELCOME  TO   "};
BYTE code dis2[] = {" WWW.PRECHIN.COM "};
BYTE code dis3[] = {"   OT  EMOCLEW   "};
BYTE code dis4[] = {" MOC.NIHCERP.WWW "};

delay(int ms)
{                           // 延时子程序
int i;
while(ms--)
{
  for(i = 0; i< 250; i++)
  {
   _nop_();
   _nop_();
   _nop_();
   _nop_();
  }
}
}

BOOL lcd_bz()
{                          // 测试LCD忙碌状态
BOOL result;
LCD_RS = 0;
LCD_RW = 1;
LCD_EP = 1;
_nop_();
_nop_();
_nop_();
_nop_();
result = (BOOL)(P0 & 0x80);
LCD_EP = 0;
return result; 
}

lcd_wcmd(BYTE cmd)
{                          // 写入指令数据到LCD
while(lcd_bz());
LCD_RS = 0;
LCD_RW = 0;
LCD_EP = 0;
_nop_();
_nop_(); 
P0 = cmd;
_nop_();
_nop_();
_nop_();
_nop_();
LCD_EP = 1;
_nop_();
_nop_();
_nop_();
_nop_();
LCD_EP = 0;  
}

lcd_pos(BYTE pos)
{                          //设定显示位置
lcd_wcmd(pos | 0x80);
}

lcd_wdat(BYTE dat) 
{                          //写入字符显示数据到LCD
while(lcd_bz());
LCD_RS = 1;
LCD_RW = 0;
LCD_EP = 0;
P0 = dat;
_nop_();
_nop_();
_nop_();
_nop_();
LCD_EP = 1;
_nop_();
_nop_();
_nop_();
_nop_();
LCD_EP = 0; 
}

lcd_init()
{                        //LCD初始化设定
lcd_wcmd(0x38);          //16*2显示，5*7点阵，8位数据
delay(1);
lcd_wcmd(0x0c);          //显示开，关光标
delay(1);
lcd_wcmd(0x06);          //移动光标
delay(1);
lcd_wcmd(0x01);          //清除LCD的显示内容
delay(1);
}

main()
{
      BYTE i;
      lcd_init();               // 初始化LCD
      delay(10);

    while(1)              
   { 
      lcd_wcmd(0x06);            //向右移动光标
      lcd_pos(0);                //设置显示位置为第一行的第1个字符
      i = 0;
     while(dis1[ i ] != '\0')
     {                           //显示字符"WLCOME  TO"
       lcd_wdat(dis1[ i ]);
       i++;
       delay(30);                //控制两字之间显示速度
     }
      lcd_pos(0x40);             //设置显示位置为第二行第1个字符
      i = 0;
     while(dis2[ i ] != '\0')
     {
       lcd_wdat(dis2[ i ]);      //显示字符" WWW.PRECHIN.COM  "
       i++;
       delay(30);                //控制两字之间显示速度
     }
      delay(800);                //控制停留时间

      lcd_wcmd(0x01);            //清除LCD的显示内容
      delay(1);
      lcd_wcmd(0x04);            //向左移动光标
     
      lcd_pos(15);               //设置显示位置为第一行的第16个字符
      i = 0;
     while(dis3[ i ] != '\0')
     {                           //显示字符"WLCOME  TO"
       lcd_wdat(dis3[ i ]);
       i++;
       delay(30);                //控制两字之间显示速度
     }
      lcd_pos(0x4F);             //设置显示位置为第二行的第16个字符
      i = 0;
     while(dis4[ i ] != '\0')
     {
       lcd_wdat(dis4[ i ]);      //显示字符" WWW.PRECHIN.COM  "
       i++;
       delay(30);                //控制两字之间显示速度
     }
      delay(800);                //控制停留时间

      lcd_wcmd(0x01);            //清除LCD的显示内容
      delay(200);                //控制两屏转换时间
          
   }
}
 
 
