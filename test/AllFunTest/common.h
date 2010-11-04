#ifndef ___COMMON_H_H___
#define ___COMMON_H_H___
#include < reg51.h >
#include < intrins.h >
#include "key.h"

#define uchar unsigned char
#define uint  unsigned int

#define delayNOP() ; {_nop_() ;_nop_() ;_nop_() ;_nop_() ;} ; //4us

//根据自己的接线来改
sbit BEEP    = P1^5 ;         //蜂鸣器驱动线
sbit LED     = P2^0 ;		  //自定义的LED的输出端口
sbit LCD_RS  = P2^6 ;         //1602 LCD RS 端口   
sbit LCD_RW  = P2^5 ;		  //1602 LCD RW 端口   
sbit LCD_EN  = P2^7 ;		  //1602 LCD EN 端口   
//138译码器管脚定义
sbit LS138A  = P2^2;          //138管脚定义
sbit LS138B  = P2^3;
sbit LS138C  = P2^4; 
sbit IRIN    = P3^2 ;         //红外接收器数据线
sbit DQ      = P3^7 ;         //定义DS18B20端口DQ  

//公共函数声明
void delay_ms(int ms);            //ms*1ms
void delay_us(unsigned int num);  //num*1us
void IRdelay(char x);             //x*0.14ms
void IRInit(void);				  //红外端口初始化

void lcd_init(void);			  //LCD1602端口初始化
void led_display(unsigned char value);											       //led显示
void lcd_display(unsigned char value, unsigned char pos, unsigned char is_decimal );   //lcd显示 参数包括内容 位置 和十进制标识符
void process_message(void);		  //核心消息处理函数（在主循环中）
uchar is_quit(void);			  //判断是否有退出键按下
void lcd_temp_menu(void);
void lcd_welcome_menu(void);
void beep(void);

//全局遥控器按键值的声明
extern unsigned char  ir_key_value;	 //全局遥控器按键键值声明
extern unsigned char  ir_key_last;	 //TODO：暂未使用 

#endif
