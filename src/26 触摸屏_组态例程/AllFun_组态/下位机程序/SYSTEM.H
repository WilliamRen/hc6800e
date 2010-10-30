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

sbit SCK = P3^6;	    //时钟	
sbit SData=P3^4;	    //数据	
sbit RST = P3^5;   // DS1302复位


/*//SPI IO  (HC595)
sbit    MOSIO =P3^6;
sbit    R_CLK =P3^5;
sbit    S_CLK =P3^4;  */

//SPI IO  (HC165)
sbit    IN_PL =P1^6;    //74HC165  shift load   把数据加载到锁存器中
sbit    IN_Dat=P1^7;    //74HC165  output       数据移出
sbit    OE    =P3^6;
					   
//else IO (138译码器)
sbit    LS138A=P2^2;  
sbit    LS138B=P2^3;
sbit    LS138C=P2^4; 
 
sbit    BEEP  =P1^4;
sbit    D18B20=P3^7;

//Motor	(步进电机)
sbit F1 = P1^0;
sbit F2 = P1^1;
sbit F3 = P1^2;
sbit F4 = P1^3;

//PWM
//sbit PWM=P1^2;




#define  PCF8591 0x90  //PCF8591 地址
#define  AT24C02 0xa0  //AT24C02 地址
#define  PCF8563 0xa2  //PCF8563 地址 0xa2


// 此表为 LED 的字模          
extern  unsigned char code  Disp_Tab[]; 
extern  unsigned char idata LedUnit;
extern  unsigned int  idata LedNumVal;
extern  unsigned char idata LedOut[]; 

extern unsigned char bdata X0,Y0,Key;
extern unsigned char idata M[];
extern unsigned int  xdata D[];
extern unsigned int  Tick_10ms; 


extern xdata unsigned int xdata temp_time;
extern unsigned int  xdata Temperature;
extern bit ReadRTC_Flag; //定义读DS1302标志
extern unsigned char l_tmpdate[]; //秒分时日月周年08-05-15 12:00:00
extern code unsigned char table[];
extern unsigned char l_tmpdisplay[];


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


extern void InitLcd();
extern void LcdLeftIn();
extern unsigned char ReHC74165(void);
extern void HC595SendData(unsigned int SendVal);
extern void delay(unsigned int v);
extern void ad_da();
extern unsigned char TickOut(unsigned int * tick, unsigned int tickCnt);
extern void GetTemp();
extern void InitTimer();
extern void GetTimer();
extern void ConvertASCII (void);
extern void ADDA_ConvertASCII (void);

extern void InitMotor();
extern void SetMotor();
extern unsigned int  CurTemper;





/******************************************************************/
/*                    函数声明                                    */
/******************************************************************/												
//extern void Write_Ds1302_byte(unsigned char temp); 
//extern void Write_Ds1302( unsigned char address,unsigned char dat );
//extern unsigned char Read_Ds1302 ( unsigned char address );
extern void Read_RTC(void);//read RTC 
extern void InitTimer(void); //set RTC 
//extern void InitTIMER0(void);//inital timer0
extern void LED_Array(void);





