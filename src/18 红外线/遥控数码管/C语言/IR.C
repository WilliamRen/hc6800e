/********************************************************************************
*  描述:                                                                        *
*         数码管显示 遥控键值读取器                                             *
*          遥控器接p3.2                                                         *
*  现象：当按遥控器时，按下事 数码管会有闪烁，为什么会闪烁呢                    *
*                                                                               *
*         连接方法：使用红外功能时 J1跳线短接                                   *
*****************************************************************************8**/

#include <reg51.h>

sbit IRIN = P3^2;         //红外接收器数据线


void IRdelay(char x);  //x*0.14MS
void beep();

unsigned char  IRCOM[7];
extern unsigned char Y0;
extern void display(unsigned char Key_Val);
/*******************************************************************/
void IRInit()
{
    IE |= 0x81;                 //允许总中断中断,使能 INT0 外部中断
    TCON |= 0x01;               //触发方式为脉冲负边沿触发
    
    IRIN=1;                    //I/O口初始化
} 

/**********************************************************/
void IR_IN(void) interrupt 0 
{
  unsigned char j,k,N=0;
     EX0 = 0;  
	   
	 IRdelay(5);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
  {IRdelay(1);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
   while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     {IRdelay(1);}
    while (!IRIN)          //等 IR 变为高电平
     {IRdelay(1);}
     while (IRIN)           //计算IR高电平时长
      {
    IRdelay(1);
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
     return; 
	 
	}

       EX0 = 1; 
} 


/**********************************************************/
void IRdelay(unsigned char x)    //x*0.14MS
{
 unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}




