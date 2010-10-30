/*******************************************************************************
*  标题:                  试验数码管上显示外部中断计数                         *
*																			   *
*  连接方法：JP10 （P0）与J12  JP9(P3)与JP5  用8PIN排线连接	   				   *
   P3.2和P3.3外部中断输入  由两个四位数码管分别显示计数                        *
   注意：P3.2接红外接收头，请把J1断开，以免影响P3.2中断效果					   *
********************************************************************************
*                         	       										       *
* 请学员认真消化本例程，用单片机脚直接控制数码管和外部中断使用                 *
********************************************************************************/


#include < reg51.h >
#include <intrins.h>

sbit LS138A=P2^2;  
sbit LS138B=P2^3;
sbit LS138C=P2^4; 

unsigned int LedNumVal_1,LedNumVal_2,LedOut[8];

//此表为 LED 的字模, 共阴数码管 0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 


void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


void main(void)
{  unsigned char i;

   P0=0xff;
   P1=0xff;
   P2=0xff;


// IT0=0;       //低电平触发
   IT0=1;       //下降沿触发
   EX0=1;


   
// IT1=0;       //低电平触发
   IT1=1;       //下降沿触发
   EX1=1;
   EA=1;


   while(1)
   {    
     LedOut[0]=Disp_Tab[LedNumVal_1%10000/1000];
     LedOut[1]=Disp_Tab[LedNumVal_1%1000/100]|0x80;	  //0x7f小数点
     LedOut[2]=Disp_Tab[LedNumVal_1%100/10];
     LedOut[3]=Disp_Tab[LedNumVal_1%10];

	 LedOut[4]=Disp_Tab[LedNumVal_2%10000/1000];
     LedOut[5]=Disp_Tab[LedNumVal_2%1000/100];
     LedOut[6]=Disp_Tab[LedNumVal_2%100/10];
     LedOut[7]=Disp_Tab[LedNumVal_2%10];


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

}



/********************************************************
* INT0中断函数                                          *
********************************************************/
void  counter0(void) interrupt 0
{
  
   EX0=0;
   delay(100);
   LedNumVal_1++;
   EX0=1;
}
/********************************************************
* INT1中断函数                                          *
********************************************************/
void  counter1(void) interrupt 2 
{
   
   EX1=0;
   delay(100);
   LedNumVal_2++;
   EX1=1;
}
/********************************************************/


