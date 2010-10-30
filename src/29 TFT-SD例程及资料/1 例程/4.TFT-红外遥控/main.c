/**********************************************************************
*                                                                     *
*           TFT液晶红外遥控键值显示程序                               *
*              （  普中科技  ）                                       *
*                                                                     *
* 实验目的：1.掌握TFT彩屏驱动程序的编写、							  *
            2.学会运用取字模软件应用(取字符和汉字数组)				  *
			3.学会怎样处理字模数组数据                                *
            4.掌握红外遥控中断处理                                    *
*                                                                     *
*实验现象： 彩屏中显示红外遥控按钮的键值                              *
*                                                                     *
***********************************************************************/

#include <reg52.h>
#include <intrins.h>
#include "lcd.h"

unsigned char Val[2];  //初始化数组

#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

void delay(uchar x);  //x*0.14MS
void delay1(int ms);
void beep();
void IR_initi();

sbit IRIN = P3^2;         //红外接收器数据线
sbit BEEP = P1^5;         //蜂鸣器驱动线
sbit RELAY= P1^4;         //继电器驱动线

uchar IRCOM[7] = {0,0,0,0,0,0,0};

/**********************************************************

  主函数

**********************************************************/
void main(void)
{  
   P2 = 0xff;
   P0 = 0xff;
   
   IR_initi() ; 
   LCD_init();
   LCD_clear(5);
  
    ChineseChar(14,10,2,colors[2],colors[7],1);	  //普
    ChineseChar(38,10,2,colors[2],colors[7],2);	  //中
    ChineseChar(62,10,2,colors[2],colors[7],3);	  //科
    ChineseChar(86,10,2,colors[2],colors[7],4);	  //技
     
   
    ChineseChar(32,60,1,colors[4],colors[7],1);	  //红
	ChineseChar(48,60,1,colors[4],colors[7],2);	  //外
	ChineseChar(64,60,1,colors[6],colors[7],3);	  //遥
	ChineseChar(80,60,1,colors[6],colors[7],4);	  //控
	
    ChineseChar(24,80,1,colors[2],colors[7],5);   //键
    ChineseChar(40,80,1,colors[2],colors[7],6);	  //值
	ChineseChar(56,80,1,colors[2],colors[7],7);	  //读
    ChineseChar(72,80,1,colors[2],colors[7],8);   //取
    ChineseChar(88,80,1,colors[2],colors[7],9);   //器
    
    LCD_ShowString(22,100,colors[7],colors[2],"Value:    H");  //将字符送到彩屏中显示
    //LCD_ShowString(102,100,colors[7],colors[2],"");  //将字符送到彩屏中显示
   


   while(1)	
  {
        Val[0] =  IRCOM[6];
        Val[1] =  IRCOM[5];
        
	    LCD_ShowString(78,100,colors[7],colors[2],Val); //将字符送到彩屏中显示
	   
  }  
}

/***********************************************************
*                                                          *
*                通过外部中断来解码
***********************************************************/
void IR_initi()  
{
   IRIN=1;                    //I/O口初始化
   BEEP=1;
   RELAY=1; 
   delay1(10);                 //延时
   IE = 0x81;                 //允许总中断中断,使能 INT0 外部中断
   TCON = 0x01;               //触发方式为脉冲负边沿触发

}

/**********************************************************/
void IR_IN() interrupt 0 using 0
{
  unsigned char j,k,N=0;
     EX0 = 0;   
	 delay(15);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
    {delay(1);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
   while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     {delay(1);}
    while (!IRIN)          //等 IR 变为高电平
     {delay(1);}
     while (IRIN)           //计算IR高电平时长
      {
    delay(1);
    N++;           
    if (N>=30)
	 { EX0=1;
	 return;}                  //0.14ms计数过长自动离开。
      }                        //高电平计数完毕                
     IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
     N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //取键码的低四位
   IRCOM[6]=IRCOM[2] >> 4;       //右移4次，高四位变为低四位

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5]+0x37;}    //第二位数显示
   else
	  IRCOM[5]=IRCOM[5]+0x30;

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6]+0x37;}    //第一位数显示 
   else
	  IRCOM[6]=IRCOM[6]+0x30;
   


     beep();
     EX0 = 1; 
} 


/**********************************************************/
void beep()
{
  unsigned char i;
  for (i=0;i<100;i++)
   {
   delay(4);
   BEEP=!BEEP;                 //BEEP取反
   } 
  BEEP=1;                      //关闭蜂鸣器
}
/**********************************************************/
void delay(unsigned char x)    //x*0.14MS
{
 unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}

/**********************************************************/
void delay1(int ms)
{
 unsigned char y;
  while(ms--)
 {
  for(y = 0; y<250; y++)
  {
   _nop_();
   _nop_();
   _nop_();
   _nop_();
  }
 }
}


