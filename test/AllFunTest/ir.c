/********************************************************************************
*  描述:                                                                        *
*         数码管显示 遥控键值读取器                                             *
*          遥控器接p3.2                                                         *
*  现象：当按遥控器时，按下事 数码管会有闪烁，为什么会闪烁呢                    *
*                                                                               *
*         连接方法：使用红外功能时 J1跳线短接                                   *
*****************************************************************************8**/
#include "common.h"

unsigned char  IRCOM[7];

/**********************************************************/
void IR_IN(void) interrupt 0 
{
  unsigned char j,k,N=0;
  EX0 = 0;  

  IRdelay(5);
  if (IRIN==1) 
  { 
   EX0 =1;
   return;
  } 
                            //确认IR信号出现
  while (!IRIN)             //等IR变为高电平，跳过9ms的前导低电平信号。
  {IRdelay(1);}

  for (j=0;j<4;j++)         //收集四组数据
  { 
   for (k=0;k<8;k++)        //每组数据有8位
   {
    while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
    {IRdelay(1);}
    while (!IRIN)           //等 IR 变为高电平
    {IRdelay(1);}
    while (IRIN)            //计算IR高电平时长
    {
     IRdelay(1);
     N++;           
     if (N>=30)
	 { 
	  EX0=1;
	  return;
	 }                      //0.14ms计数过长自动离开。
    }                       //高电平计数完毕                
    IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
    if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
    N=0;
   }//end for k
  }//end for j
 if (IRCOM[2]!=~IRCOM[3])
 { 
  EX0=1;
  return; 
 }
 EX0 = 1; 
 ir_key_value =  IRCOM[2] ; 
} 

/*******************************************************************/
void IRInit(void)
{
    IE   |= 0x81;               //允许总中断中断,使能 INT0 外部中断
    TCON |= 0x01;               //触发方式为脉冲负边沿触发
    IRIN  = 1;                  //I/O口初始化
}

//判断当前遥控器按下的按键是否是退出按键，
//用于各个子模块循环是否退出的判断条件
uchar is_quit(void)
{
	uchar flag = 0xFF;
	if ( (ir_key_value&0xff) == MY_KEY_ESC )
	{
		flag = 0x00;
	}
	return flag;
}

//测试程序
void test_ir(void)
{
  	//IRInit();
	LED = 1;
	while(is_quit())
	{ 
    	if ( (ir_key_value&0xff) == MY_KEY_MODE )
		{
      		LED = 0;
	  		led_display( ir_key_value );
		}
		else
		{
	  		LED = 1;
      		led_display( ir_key_value );
		}
		//display(IRCOM[2]);
   	}
}

