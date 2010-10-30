#include <reg52.h> 
#include <intrins.h>
#include <absacc.h>
#include <math.h>
#include <string.h>

#define      uint8   unsigned char
#define      Uint8   unsigned char
#define      uint16  unsigned int
#define      Uint16  unsigned int



//输入
sbit Jiao_ban       = P3^0;
sbit shui_beng      = P3^1;
sbit alarm_remove   = P3^2;

//设置按钮

sbit ADD = P3^4;
sbit SUB = P3^5;
sbit SET = P3^7;

//报警信号
sbit jiao_ban_Err   = P0^5;
sbit shui_beng_Err  = P0^6;
sbit power_Err      = P0^7;
sbit liq_level_low  = P2^1;

//输出继电器
sbit beep           = P0^5;
sbit jiao_relay	    = P0^2;
sbit shui_relay	    = P0^3;



sbit LS138A=P2^2;  
sbit LS138B=P2^3;
sbit LS138C=P2^4; 



extern unsigned int D[] ,W_buffer[];

extern void earseSector(unsigned int addr);
extern void wrData(unsigned int addr, unsigned int len);
extern void rdData(unsigned int addr, unsigned int len);