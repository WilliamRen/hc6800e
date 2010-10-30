 /*******************************************************************************
*  标题:                  试验数码管上显示温度                                  *
*																			    *
*********************************************************************************
*  1.通过本例程了解 DLASS18b20的基本原理和使用 ,理解并掌握18B20驱动程序的编写	*
*  2.了解掌握I2C总线接口的工作原理及一般编程方法。 								*
*    插上18B20 观察数码管的实际温度显示
*    用排线将JP10(P0口) 与 J12 连接	在数码管上可以看温度显示					*
* 																				*
* 注意：18B20程序只能在12T模式下工作 6T模式自己修改相应时序                     *
* 请学员认真消化本例程，懂DLASS18b20在C语言中的操作                       	    *
********************************************************************************/
#include <reg52.H>

extern GetTemp();									   //声明引用外部函数
extern unsigned int  idata Temperature;	              // 声明引用外部变量
void delay(unsigned int i);

//else IO
sbit    LS138A=P2^2;       //管脚定义
sbit    LS138B=P2^3;
sbit    LS138C=P2^4; 


//此表为 LED 的字模, 共阴数码管 0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 
unsigned long  LedOut[5],LedNumVal;


void system_Ini()
{
    TMOD|= 0x11;
    TH1=0xDC; //11.0592M
    TL1=0x00;	
	IE = 0x8A;	
    TR1  = 1;

}

main()
{ unsigned char i; 
  system_Ini();
    while(1)
   {	
	  GetTemp();

   	  /********以下将读18b20的数据送到LED数码管显示*************/
       LedNumVal=Temperature;      //把实际温度送到LedNumVal变量中
	  LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
      LedOut[1]=Disp_Tab[LedNumVal%1000/100];
      LedOut[2]=Disp_Tab[LedNumVal%100/10];	 //十位
      LedOut[3]=Disp_Tab[LedNumVal%10];    //个位
  	  
    for(i=0; i<4; i++) 
	  {		
		 P0 = LedOut[i] ;
	  			 	 	  	
		switch(i)					  
         {	    	  //138译码
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
            case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
            case 2:LS138A=0; LS138B=1; LS138C=0; break; 
            case 3:LS138A=1; LS138B=1; LS138C=0; break; 
         }
		 
		delay(150);
	  }

	  P0 = 0;  
  
    }
}

 //延时程序
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}



