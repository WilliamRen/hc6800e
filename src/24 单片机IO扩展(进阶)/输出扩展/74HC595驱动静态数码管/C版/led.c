/*******************************************************************************
*  标题:                  试验74HC595驱动数码管上显示数字（C语言）             *
* 																			   *
*   连接方法：JP12用条线冒短接  JP3和JP2 用8PIN排线连接					       *													   *
********************************************************************************
* 通过本例程了解 74HC595（串入并出）基本原理和使用  		                   *
*                         	       										       *
* 请学员认真消化本例程，懂74C595在C语言中的操作                                *
********************************************************************************/


#include <reg51.h> 
#include <intrins.h>
#define  NOP() _nop_()  /* 定义空指令 */

//SPI IO
sbit MOSIO =P3^4;
sbit R_CLK =P3^5;
sbit S_CLK =P3^6;



void delay(unsigned int i);      //函数声名
void HC595SendData(unsigned char SendVal);
 
// 此表为 LED 的字模            // 0    1    2    3    4    5    6   7    8    9    A    b    c    d    E    -    L   P    U    Hidden  _ (20)
unsigned char code Disp_Tab[] = { 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0xbf,0xc7,0x8c,0xc1, 0xff,  0xf7 }; 
unsigned char code LED7Code[] = {~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};

 
main()
{  unsigned char  HC595SendVal;
   unsigned int LedNumVal=1;
   
  while(1)
  {	 
	 	LedNumVal++;
		HC595SendVal = LED7Code[LedNumVal%16];     //LED7; 	显示0-F  LedNumVal%10 显示0-9
	    HC595SendData(HC595SendVal);	
  	  		 
		delay(200);
	 
  }   
}	

void delay(unsigned int i)
{
    unsigned int j;
    for(i; i > 0; i--)
        for(j = 300; j > 0; j--);
}


/*********************************************************************************************************
** 函数名称: HC595SendData
** 功能描述: 向SPI总线发送数据
*********************************************************************************************************/
void HC595SendData(unsigned char SendVal)
{  
  unsigned char i;
		
  for(i=0;i<8;i++) 
   {
	if((SendVal<<i)&0x80) MOSIO=1; //set dataline high  0X80  最高位与SendVal左移的最高位 进行逻辑运算
	else MOSIO=0;				   // 如果为真 MOSIO = 1  
 
	S_CLK=0;
	NOP();
	NOP();
	S_CLK=1;	
   }
   
	
  R_CLK=0; //set dataline low
  NOP();
  NOP();
  R_CLK=1; //片选

}

