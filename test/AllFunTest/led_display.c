/*******************************************************************/
/*  LED数码管显示控制（译码器位选 共阴极）                         */
/*  连接方法：JP8 （P1）与J12 用8PIN排线连接                       */
/*  用573锁存器和138译码器控制和数码管                             */
/*******************************************************************/
#include "common.h"

unsigned long  LedOut[8]={0x3F},LedNumVal;
//此表为 LED 的字模, 共阴数码管 0-9  - 
unsigned char code Disp_Tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

//通用程序向共阴极数码管写入数据
void led_display(unsigned char value)
{  
	unsigned char i; 
    /********以下将键值送到LED数码管显示*************/
	LedNumVal=value;      //把键值送到LedNumVal变量中
	LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
    LedOut[1]=Disp_Tab[LedNumVal%1000/100];
    LedOut[2]=Disp_Tab[LedNumVal%100/10];	 //十位
    LedOut[3]=Disp_Tab[LedNumVal%10];        //个位

	LedOut[4]=Disp_Tab[LedNumVal%10000/1000];
    LedOut[5]=Disp_Tab[LedNumVal%1000/100];
    LedOut[6]=Disp_Tab[LedNumVal%100/10];	 //十位
    LedOut[7]=Disp_Tab[LedNumVal%10];        //个位
  	  
    for(i=0; i<8; i++) 
	{		
		P1 = LedOut[i] ;
		switch(i)					  
        {	//138译码
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
            case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
            case 2:LS138A=0; LS138B=1; LS138C=0; break; 
            case 3:LS138A=1; LS138B=1; LS138C=0; break;
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; break; 
        }
		delay_us(100);
	}
    P1 = 0;  
}


