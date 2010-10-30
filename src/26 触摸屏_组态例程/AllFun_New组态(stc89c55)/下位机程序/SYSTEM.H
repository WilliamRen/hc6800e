#include <reg52.H>
#include <absacc.h> 
#include <intrins.h>
#include <stdio.h>

#define  NOP()   _nop_()   /* 定义空指令 */
#define  _Nop()  _nop_()   /*定义空指令*/
#define  uchar  unsigned char
#define  UCHAR  unsigned char 
#define  UINT   unsigned int


//LCD IO
sbit LCDRWPIN = P2^5;
sbit LCDRSPIN = P2^6;
sbit LCDEPIN  = P2^7;

//SPI IO
sbit    MOSIO =P3^4;
sbit    R_CLK =P3^5;
sbit    S_CLK =P3^6;

sbit    IN_PL =P1^6;    //74HC165  shift load   把数据加载到锁存器中
sbit    IN_Dat=P1^7;    //74HC165  output       数据移出
sbit    LED8_1=P1^1;

//else IO
sbit    LS138A=P2^2;  
sbit    LS138B=P2^3;
sbit    LS138C=P2^4;  
sbit    BEEP  =P1^5;
sbit    D18B20=P3^7;

//Motor
sbit F1 = P1^0;
sbit F2 = P1^1;
sbit F3 = P1^2;
sbit F4 = P1^3;

//PWM
//sbit PWM=P1^2;

sbit     SCL=P2^0;     //I2C  时钟 
sbit     SDA=P2^1;     //I2C  数据 


#define  PCF8591 0x90  //PCF8591 地址
#define  AT24C02 0xa0  //AT24C02 地址
#define  PCF8563 0xa2  //PCF8563 地址


// 此表为 LED 的字模          
extern unsigned char code  Disp_Tab[]; 
extern unsigned char code LED7Code[];

extern unsigned char idata LedUnit;
extern unsigned int  idata LedNumVal;
extern unsigned char idata LedOut[]; 

extern unsigned char bdata X0,Y0,Key;
extern unsigned char idata M[];
extern unsigned int  xdata D[];
extern unsigned int  Tick_10ms; 


extern bit ack;
//起动总线函数
extern void Start_I2c();
//结束总线函数  
extern void Stop_I2c();
//应答子函数
extern void Ack_I2c(bit a);
//字节数据发送函数
extern void  SendByte(unsigned char  c);
//有子地址发送多字节数据函数               
extern bit ISendStr(uchar sla,uchar suba,uchar *s,uchar no) ;
//无子地址发送多字节数据函数   
extern bit ISendStrExt(uchar sla,uchar *s,uchar no);
//无子地址读字节数据函数               
extern unsigned char RcvByte();
//有子地址读取多字节数据函数               
extern bit IRcvStr(uchar sla,uchar suba,uchar *s,uchar no);
//无子地址读取多字节数据函数               
extern bit IRcvStrExt(uchar sla,uchar *s,uchar no);


extern unsigned char ReHC74165(void);
extern void HC595SendData(unsigned char SendVal);
extern void delay(unsigned int v);
extern void ad_da();
extern unsigned char TickOut(unsigned int * tick, unsigned int tickCnt);
extern void ConvertASCII (void);

extern void InitMotor();
extern void SetMotor();

extern void Modbus_Init(unsigned long Fosc, unsigned int BaudRate, unsigned char ID, unsigned char *Baddr,  unsigned short *Waddr); 
extern void Modbus_Handle(unsigned int Tick);

