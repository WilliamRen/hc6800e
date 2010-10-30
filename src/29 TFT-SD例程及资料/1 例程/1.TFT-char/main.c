 /*********************************************************************
*                                                                     *
*           TFT液晶屏字符和汉字显示程序                               *
*              （  普中科技  ）                                       *
*                                                                     *
* 实验目的：1.掌握TFT彩屏驱动程序的编写、							  *
            2.学会运用取字模软件应用(取字符和汉字数组)				  *
			3.学会怎样处理字模数组数据                                *
*                                                                     *
*实验现象：彩屏中显示汉字、数字、字母！                               *
*                                                                     *
***********************************************************************/

#include <reg52.h>
#include <intrins.h>

#include "lcd.h"


/**********************************************************

  主函数

**********************************************************/
void main(void)
{  
   P2 = 0xff;
   P0 = 0xff;

   LCD_init();
   LCD_clear(5);
  
   ChineseChar(14,10,2,colors[2],colors[7],1);	//普
   ChineseChar(38,10,2,colors[2],colors[7],2);	//中
   ChineseChar(62,10,2,colors[2],colors[7],3);	//科
   ChineseChar(86,10,2,colors[2],colors[7],4);	//技
     
   
   LCD_ShowString(12,40,colors[0],colors[7],"-");
   ChineseChar(20,40,0,colors[0],colors[7],1);	//带
   ChineseChar(32,40,0,colors[0],colors[7],2);	//您
   ChineseChar(44,40,0,colors[0],colors[7],3);	//进
   ChineseChar(56,40,0,colors[0],colors[7],4);	//入
   ChineseChar(68,40,0,colors[0],colors[7],5);	//单
   ChineseChar(80,40,0,colors[0],colors[7],6);	//片
   ChineseChar(92,40,0,colors[0],colors[7],7);	//机
   ChineseChar(104,40,0,colors[0],colors[7],8);	//世
   ChineseChar(116,40,0,colors[0],colors[7],9);	//界 

   LCD_ShowString(4,58,colors[0],colors[5],"www.PRECHIN.com");


    ChineseChar(0,80,1,colors[4],colors[7],1);	//专
	ChineseChar(16,80,1,colors[4],colors[7],2);	//业
	ChineseChar(32,80,1,colors[6],colors[7],3);	//技
	ChineseChar(48,80,1,colors[6],colors[7],4);	//术
	ChineseChar(60,80,1,colors[2],colors[7],5);	//论
	ChineseChar(76,80,1,colors[2],colors[7],6);	//坛
	LCD_ShowString(92,80,colors[4],colors[7],":");

	LCD_ShowString(4,100,colors[7],colors[2],"www.PRECHIN.net");

	
   	   
   LCD_ShowString(12,124,colors[0],colors[7],"TELE:");
   LCD_ShowString(12,142,colors[2],colors[7],"0755-61190227");

   while(1)	;
}

/*********************************************************/
