/*******************************************************************************
*  标题:                  试验4位数码管上显示外部中断计数                      *
*																			   *
*  连接方法：P0与J12  P3与JP5  用8PIN排线连接	   P3.2外部中断输入            *
             JP15和JP16用8个短路冒连上			                               *
********************************************************************************
*                         	       										       *
* 请学员认真消化本例程，用单片机通过译码器控制数码管和外部中断使用             *
********************************************************************************/

#include < reg51.h >
#include <intrins.h>

sbit LS138A=P2^2;  
sbit LS138B=P2^3;
sbit LS138C=P2^4; 


unsigned int LedNumVal = 0 ,LedOut[8];

// 此表为 LED 的字模            // 0    1    2    3    4    5    6   7    8    9    A    b    c    d    E    -    L   P    U    Hidden  _ (20)
unsigned char code Disp_Tab[] = { ~0xC0,~0xF9,~0xA4,~0xB0,~0x99,~0x92,~0x82,~0xF8,~0x80,~0x90,~0x88,~0x83,~0xC6,~0xA1,~0x86,~0xbf,~0xc7,~0x8c,~0xc1, ~0xff,  ~0xf7 }; 



void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


display()
{  unsigned char i;

     LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
     LedOut[1]=Disp_Tab[LedNumVal%1000/100]&0x7f;
     LedOut[2]=Disp_Tab[LedNumVal%100/10];
     LedOut[3]=Disp_Tab[LedNumVal%10];


	 for( i=0; i<8; i++) 
	 {	 P0 = LedOut[i];
			
	  switch(i)					  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; break;
			
	     }

		delay(150);
	 }
 }



/********************************************************
* 主程序                                               *
********************************************************/
void main(void)
{  
   P0=0xff;
   P1=0xff;
   P2=0xff;   
   
// IT0=0;       //低电平触发
   IT0=1;       //下降沿触发
   EA=1;
   EX0=1;

   while(1)
   {    
     display();
   }
}
/********************************************************
* INT0中断函数                                          *
********************************************************/
void  counter(void) interrupt 0 
{
//   unsigned char  x;
   EX0=0;
   LedNumVal++;     //中断计数  
   EX0=1;
}
/********************************************************/



