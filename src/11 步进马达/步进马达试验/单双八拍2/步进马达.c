/*************************************************************************************
*  标题:  步进电机试验二															 *
* 																					 *
*    通过本例程了解步进马达使用及驱动程序编写										 *
*;   单双八拍工作方式：                                                           	 *
*;   A-AB-B-BC-C-CD-D-DA (即一个脉冲,转 3.75 度)      								 *
*    J14短路冒需断开                  	                                             *
*  请学员一定要消化掉本例程                       	                            	 *
                    	                           *								 *
**************************************************************************************/    
 

 #include "reg52.h"

unsigned char code FFW[8]={0xfe,0xfc,0xfd,0xf9,0xfb,0xf3,0xf7,0xf6};
void delay(unsigned int t);

 //Motor
sbit F1 = P1^0;
sbit F2 = P1^1;
sbit F3 = P1^2;
sbit F4 = P1^3;

 ///////////////////////////////////////
//步进电机驱动
void  motor_ffw()
 { 
   unsigned char i;
  
      for (i=0; i<8; i++)       //一个周期转30度
        {
          P1 = FFW[i]&0x1f;     //取数据
          delay(5);            //调节转速
        }
     } 


void delay(unsigned int t)
{                           
   unsigned int k;
   while(t--)
   {
     for(k=0; k<60; k++)
     { }
   }
}


main() 
 { 
    while(1)
	{
	 motor_ffw();

	 }
}

