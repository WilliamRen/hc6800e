/*************************************************************************************
*  标题:                   五轴控制步进电机试验										 *
* 																					 *
*    通过本例程了解步进马达使用及驱动程序编写										 *
*;   单双八拍工作方式：                                                           	 *
*;   A-AB-B-BC-C-CD-D-DA-A (即一个脉冲,转 3.75 度)      							 *
*    J14短路冒需断开 ,通过排线连接步进马达数据接口                 	                 *
*    请学员一定要消化掉本例程，领会指针的经典应用，                       	         *
* 现象：5个步进马达以不同加速度运行，运行到不同圈数后停止                    	     *                        									 *
**************************************************************************************/  
#include "reg52.h"

unsigned char TickOut(unsigned int * tick, unsigned int tickCnt);
unsigned char  Acceleration( unsigned int *Base_Tim,  unsigned int Delay_Tim,unsigned int * Flag , unsigned char* Speed) ;

unsigned int Tick_10ms=0;    //时间片计数器

unsigned char code FFW_L[8]={~0xfe,~0xfc,~0xfd,~0xf9,~0xfb,~0xf3,~0xf7,~0xf6};   //低四位 马达相序变化表
unsigned char code FFW_H[8]={~0xef,~0xcf,~0xdf,~0x9f,~0xbf,~0x3f,~0x7f,~0x6f};   //高四位 马达相序变化表

unsigned int  MotorTimer1 = 0 , MotorTimer2 = 0, MotorTimer3 = 0, MotorTimer4 = 0, MotorTimer5 = 0;	      //马达转速基时
unsigned int  MotorDelay1 = 0 , MotorDelay2 = 0, MotorDelay3 = 0, MotorDelay4 = 0, MotorDelay5 = 0; TIM,CT;	//马达转速

unsigned  int xdata Delay_Tim1 =0,	Delay_Tim2 =0,	 Delay_Tim3 =0,   Delay_Tim4 =0,   Delay_Tim5 =0;  //马达加速度
unsigned  int Base_T1 = 0,	Base_T2 = 0,	 Base_T3 = 0,	  Base_T4 = 0,   Base_T5 = 0;	 //马达加速度基时
unsigned char  speed1 = 8,	speed2 = 8,	  speed3 = 8,	   speed4 = 8,    speed5 = 8;  //马达转速
unsigned int  Acc_Flg1 =0,	Acc_Flg2 =0,  Acc_Flg3 =0,	Acc_Flg4 =0,  Acc_Flg5 =0;	   //加减速标志


/***********************************************************
purpose: 系统初始化
/**********************************************************/
void system_Ini()
{
    TMOD|= 0x11;
	//TH0=0xC4; //18.432M
    //TL0=0x00;
    TH0=0xDC; //11.0592M
    TL0=0x00;	
	IE    = 0x8A;	
    TR0  = 1;

//	TH1 = 0xfe; //11.0592
//	TL1 = 0x33;
//	TR1  = 1; 
}

/*****************************************************
*	    步进马达1							         *
******************************************************/
 
unsigned char SetMotor1(unsigned int step,unsigned int Speed ) //step 马达的步数  Speed马达速度
{
   static unsigned int Circle; 	  //马达步数
   static unsigned char ret ;
   MotorDelay1 = 70-Speed;
    if (Circle <= step)
   {  
       if (TickOut(&MotorTimer1 , MotorDelay1))
	   { static char i;
	   	 ret = (FFW_L[i]&0x0f);	  //查表取马达相序
		 if (i++ == 7) i = 0;
		 ++Circle;
	   }
    }

   return ret;
}

/*****************************************************
*	    步进马达2							         *
******************************************************/
unsigned char SetMotor2(unsigned int step,unsigned int Speed )	 //step 马达的步数  Speed马达速度
{  static unsigned int Circle ;  //马达步数
   static unsigned char ret ;
   MotorDelay2 = 70-Speed;
  if (Circle <= step)
  {   if(TickOut(&MotorTimer2 , MotorDelay2))
     { static char i;
	    ret = (FFW_H[i]&0xF0);	 //查表取马达相序
	  	if (i++ == 7) i = 0;
	    ++Circle;
	 }
  }

  return  ret;
}

 /*****************************************************
*	    步进马达3							         *
******************************************************/
unsigned char SetMotor3(unsigned int step,unsigned int Speed)	 //step 马达的步数  Speed马达速度
{  static unsigned int Circle ;   //马达步数
   static unsigned char ret ;
   MotorDelay3 = 70-Speed;
   if(Circle <= step)
   { if (TickOut(&MotorTimer3 , MotorDelay3))
     { static char i;
	   ret = FFW_L[i]&0x1f;	   //查表取马达相序
	   if (i++ == 7) i = 0;
	    ++Circle;
     }
   }
   return ret;
}

/*****************************************************
*	    步进马达4							         *
******************************************************/
unsigned char SetMotor4(unsigned int step,unsigned int Speed)   //step 马达的步数  Speed马达速度
{  static unsigned int Circle ;   //马达步数
   static unsigned char ret ;
   MotorDelay4 = 70-Speed;
    if(Circle <= step)
	{   if (TickOut(&MotorTimer4 , MotorDelay4))
	   { static char i;
		 ret = FFW_H[i]&0xf0;	//查表取马达相序
		 if (i++ == 7) i = 0;
		  ++Circle;
	   }
	}
   return ret;
}

/*****************************************************
*	    步进马达5							         *
******************************************************/
unsigned char SetMotor5(unsigned int step,unsigned int Speed)	 //step 马达的步数  Speed马达速度
{  static unsigned int Circle ;	 //马达步数
   static unsigned char ret ; 	 
   MotorDelay5 = 70-Speed;
   if(Circle <= step)
   {    if (TickOut(&MotorTimer5 , MotorDelay5))
	   { static char i;
		 ret = FFW_L[i]&0x0f;	 //查表取马达相序
		if (i++ == 7) i = 0;
		++Circle;
	  }
	  
	}
   return ret;
}

main()
{ 
  system_Ini();
  
  while (1) 
  { unsigned char ret1 , ret2 , ret3 , ret4 , ret5;
   
    //**************************步进马达1处理**************************
	Delay_Tim1 = 50;
	if(speed1 == 61)  Acc_Flg1 = 2;
	if(speed1 == 8)	  Acc_Flg1 = 1 ;
	Acc_Flg1 = 1;
	Acceleration(&Base_T1 , Delay_Tim1, &Acc_Flg1  , &speed1);
    ret1 = SetMotor1(8*5000 , speed1 ) ;	 //参数设置步数  8个脉冲30度*100=3000度

	//**************************步进马达2处理**************************
	Delay_Tim2 = 250;
	Acc_Flg2 = 1;
	Acceleration(&Base_T2 , Delay_Tim2, &Acc_Flg2  , &speed2);
 	ret2 = SetMotor2(8*1000 , speed2) ;	 //马达2
	P1 = ret1 | ret2 ;

	//**************************步进马达3处理**************************
	Delay_Tim3 = 500;
	Acc_Flg3 = 1;
	ret3 = SetMotor3(8*1500 , Acceleration(&Base_T3 , Delay_Tim3, &Acc_Flg3  , &speed3)) ;	 //马达3
	
	//**************************步进马达4处理**************************
	Delay_Tim4 = 750;
	Acc_Flg4 = 1;
	ret4 = SetMotor4(8*2000 , Acceleration(&Base_T4 ,  Delay_Tim4 ,&Acc_Flg4  , &speed4)) ;	 //马达4
 	P0 =  ret3 | ret4 ;
	
	//**************************步进马达5处理**************************
	Delay_Tim5 = 1000;
	Acc_Flg5 = 1;
	ret5 = SetMotor5(8*2500 , Acceleration(&Base_T5 , Delay_Tim5, &Acc_Flg5  , &speed5));    //马达5
	P2 = ret5; 

	}
 
}

/**************************************************************************
*																		  *
*               各个马达的基时（相当于多个定时器）				          *
* 注意：体会指针的用法 ，多工定义任务同时工作，不相干扰                   *
**************************************************************************/
unsigned char TickOut(unsigned int * tick, unsigned int tickCnt)
{
     if(Tick_10ms - *tick >= tickCnt)  
     {
        *tick = Tick_10ms;
        return 1;
     }
     return 0;
}


/*************************************************************************
*																		 *
*                        调整加速度										 *
* 注意：体会指针的用法                                                   *
**************************************************************************/
unsigned char  Acceleration(unsigned int *Base_Tim, unsigned int Delay_Tim,  unsigned int * Flag , unsigned char* Speed)
{
   	if (TickOut(*&Base_Tim , Delay_Tim))	 
	{  
	  if((*Flag == 1) && (*Speed <= 61)) 
	  (*Speed)++ ;

	  if((*Flag == 2) && (*Speed >= 8)) 
	  (*Speed)--;
	 }
	  
   	return  *Speed;
}


/************************************************
*												*
*	      定时中断延时						    *
************************************************/
  void Tzd(void) interrupt 1
{
   TH0 = 0xfe;    //11.0592
   TL0 = 0x33;

   Tick_10ms++;	   //10MS时钟

}
