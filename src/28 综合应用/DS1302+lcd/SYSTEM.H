/*--------------------------------------------------------------*/
//包含头文件
#include<REG52.H>
#include <intrins.h>
//#include"LCD1602_8.H"
//#include"DS1302_A.H"


/*------------------------------------------------
P0 Bit Registers
------------------------------------------------*/
sbit P0_0 = 0x80;
sbit P0_1 = 0x81;
sbit P0_2 = 0x82;
sbit P0_3 = 0x83;
sbit P0_4 = 0x84;
sbit P0_5 = 0x85;
sbit P0_6 = 0x86;
sbit P0_7 = 0x87;


/*------------------------------------------------
P1 Bit Registers
------------------------------------------------*/
sbit P1_0 = 0x90;
sbit P1_1 = 0x91;
sbit P1_2 = 0x92;
sbit P1_3 = 0x93;
sbit P1_4 = 0x94;
sbit P1_5 = 0x95;
sbit P1_6 = 0x96;
sbit P1_7 = 0x97;


/*------------------------------------------------
P2 Bit Registers
------------------------------------------------*/
sbit P2_0 = 0xA0;
sbit P2_1 = 0xA1;
sbit P2_2 = 0xA2;
sbit P2_3 = 0xA3;
sbit P2_4 = 0xA4;
sbit P2_5 = 0xA5;
sbit P2_6 = 0xA6;
sbit P2_7 = 0xA7;


/*------------------------------------------------
P3 Bit Registers (Mnemonics & Ports)
------------------------------------------------*/
sbit P3_0 = 0xB0;
sbit P3_1 = 0xB1;
sbit P3_2 = 0xB2;
sbit P3_3 = 0xB3;
sbit P3_4 = 0xB4;
sbit P3_5 = 0xB5;
sbit P3_6 = 0xB6;
sbit P3_7 = 0xB7;


/*--------------------------------------------------------------*/
//宏定义便于理解
#define unchar unsigned char 
#define unint  unsigned int 
#define DS1302_SECOND      0x80
#define DS1302_MINUTE      0x82
#define DS1302_HOUR        0x84
#define DS1302_WEEK        0x8A
#define DS1302_DAY         0x86
#define DS1302_MONTH       0x88
#define DS1302_YEAR        0x8C
#define DS1302_RAM(X)      (0xC0+(X)*2)  //用于计算RAM地址的宏运算

/*--------------------------------------------------------------*/
//定义时间类型
typedef struct _SYSTEMTIME_
{
	unchar Second;
	unchar Minute;
	unchar Hour;
	unchar Week;
	unchar Day;
	unchar Month;
	unchar Year;
	unchar DateString[9];
	unchar TimeString[9];
}SYSTEMTIME;    



/*************LCD1602外部引用函数***************************************/
extern void lcd_init(void);				    	//LCD初始化设定程序
extern void prints(unsigned char *string);		//显示字符串
extern void lcd_pos (unsigned char x, bit y);	//LCD数据指针位置程序
extern void lcd_sef_chr() ;
extern void lcd_wcmd(unsigned char cmd);		//写入指令到LCD程序


/*************DS1302外部引用函数***************************************/
extern void DS1302_SetTime(unchar Address, unchar Value);  //设置时间函数
extern void DS1302_Initial (void);                         //DS1302初始化
extern void DS1302_GetTime(SYSTEMTIME *Time);              //读出时间
extern void DateToStr(SYSTEMTIME *Time);                   //日期转化成字符
extern void TimeToStr(SYSTEMTIME *Time);                   //时间转化成字符


