/**********************************************************************
*                                                                     *
*           TFT液晶矩阵键盘键值显示程序                               *
*              （  普中科技  ）                                       *
*                                                                     *
* 实验目的：1.掌握TFT彩屏驱动程序的编写、							  *
            2.学会运用取字模软件应用(取字符和汉字数组)				  *
			3.学会怎样处理字模数组数据                                *
            4.可变的ASCII的数字在TFT中显示和矩阵键盘原理。            *
*                                                                     *
*实验现象：彩屏中显示矩阵键盘的键值                                   *
*                                                                     *
***********************************************************************/

#include <reg52.h>
#include <intrins.h>

#include "lcd.h"
#include "4x3.h"

unsigned char Val[14];

/*********************************************8
*
* 将字符赋值给制定的数组中
************************************************/
void char_Initi()
{
   Val[0]  = 'V';
   Val[1]  = 'a';
   Val[2]  = 'l';
   Val[3]  = 'u';
   Val[4]  = 'e';
   Val[5]  = ':';
   Val[6]  = ' ';
   dis_buf  = '0';   // 对此变量初始化，以便刚开机时，没有按键输入时，显示为0
   Val[8]  = ' ';
   Val[9]  = '-';
   Val[10]  = 'H';
   Val[13]  = '\0';


}


/**********************************************************

  主函数

**********************************************************/
void main(void)
{  
   P2 = 0xff;
   P0 = 0xff;
   
   char_Initi();
   LCD_init();
   LCD_clear(5);
  
    ChineseChar(14,10,2,colors[2],colors[7],1);	//普
    ChineseChar(38,10,2,colors[2],colors[7],2);	//中
    ChineseChar(62,10,2,colors[2],colors[7],3);	//科
    ChineseChar(86,10,2,colors[2],colors[7],4);	//技
     
   
    ChineseChar(32,60,1,colors[4],colors[7],1);	//矩
	ChineseChar(48,60,1,colors[4],colors[7],2);	//阵
	ChineseChar(64,60,1,colors[6],colors[7],3);	//键
	ChineseChar(80,60,1,colors[6],colors[7],4);	//盘
	
    ChineseChar(24,80,1,colors[2],colors[7],5);	//键
    ChineseChar(40,80,1,colors[2],colors[7],6);	//值
	ChineseChar(56,80,1,colors[2],colors[7],7);	//读
    ChineseChar(72,80,1,colors[2],colors[7],8); //取
    ChineseChar(88,80,1,colors[2],colors[7],9); //器


   while(1)	
  {
        keydown();
        Val[7] = dis_buf; //dis_buf 已经转化成了ASCII值
	    LCD_ShowString(20,100,colors[7],colors[2],Val); //将字符送到彩屏中显示
	   
  }  
}

/*********************************************************/
