/*******************************************************************************
*  标题:  学习板的按钮输入　控制对应的继电器输出和LED灯                        *
*  																			   *
* 连接方法：用8PIN排线将JP5与JP6连接 和 JP11（P2）与JP1（LED灯）连接		   *
*			   																   *
* 1. 通过本例程了解74HC165（并入串出） 的基本原理和使用  					   *
* 2.了解掌握SPI总线接口的工作原理及一般编程方法。  							   *
* 3. 懂的74HC165在C语言中是如何操作                         	     		   *
*                   	              										   *
********************************************************************************/
 #include <reg52.H>
 #include <intrins.h> 

 #define  NOP() _nop_()     /* 定义空指令 */
 #define  _Nop()  _nop_()   /*定义空指令*/

//SPI 接口
sbit    CLK    = P3^6;
sbit    IN_PL  = P1^6;    //74HC165  shift load    把数据加载到锁存器中
sbit    IN_Dat = P1^7;    //74HC165  output        数据通过P1.7脚移进单片机内处理

sbit    RELAY  = P1^4;
sbit    BEEP   = P1^5;

unsigned char bdata Key;
unsigned int Output;
sbit    K0=Key^0;	 //位定义
sbit    K1=Key^1;
sbit    K2=Key^2;
sbit    K3=Key^3;
sbit    K4=Key^4;
sbit    K5=Key^5;
sbit    K6=Key^6;
sbit    K7=Key^7;

bit  M0 ,K0J;  //位定义
 
unsigned long ReHC74165(void);	 //函数声名 
void beep();    


 /********************************************************
 *	主函数												 *
 *														 *
 ********************************************************/
 main()
  {	  
   	//M0=M1=M2=M3=0;
  while(1)
  { 
    unsigned long  Input=ReHC74165();
	 Key=Input&0xff;
	 RELAY = 1;
     P2 = 0xff;

	  //实现脉冲输入
    if(K0&K0J)M0=~M0;      
	   K0J=~K0;  
  
	if(M0) {RELAY = 0; P2 = 0x7f;	}	//实现脉冲输入
	if(K1) {beep();    P2 = 0xbf;   }
	if(K2) {beep();    P2 = 0xdf;   }
    if(K3) {beep();    P2 = 0xef;   }
	if(K4) {beep();    P2 = 0xf7;   }
	if(K5) {beep();    P2 = 0xfb;   }
	if(K6) {beep();    P2 = 0xfd;   }
	if(K7) {beep();    P2 = 0xfe;   }
  
   }    
   
 }


 /**********************************************************
 *														   *
 *	蜂鸣器 (让蜂鸣器发出动听声音)						   *
 ************************************************************/
void beep()
{
  unsigned char i , j;
  for (i=0;i<2;i++)
   {
  
   for (j = 0; j<255; j++) {_nop_();}
   BEEP=!BEEP;                 //BEEP取反
   } 
  BEEP=1;                      //关闭蜂鸣器
}
 
 /*************************此部分为74HC165的驱动程序使用SPI总线连接*************************************/


/*********************************************************************************************************
** 函数名称:  74HC165
** 功能描述: 
** 管脚描述：请参考相关的芯片资料(学习光盘中以配)
*********************************************************************************************************/
unsigned long ReHC74165(void)
{  
  unsigned char i;
  unsigned long indata;
		
   IN_PL=0;
   NOP();
   IN_PL=1;
   NOP(); 
              
   indata=0;     
   for(i=0; i<8; i++)
    { 
	  indata=indata<<1;
      if(IN_Dat==1)indata=indata+1;
 	  CLK=1;
      NOP();
      CLK=0;	  
	} 
   
   return(~indata);	 //
}



