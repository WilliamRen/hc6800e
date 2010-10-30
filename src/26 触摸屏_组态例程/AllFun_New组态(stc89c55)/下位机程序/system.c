#include <system.h>


//此表为 LED 的字模, 共阴数码管 0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 
unsigned char code LED7Code[] = {~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};

 
unsigned char idata LedUnit;
unsigned int  idata LedNumVal;
unsigned char idata LedOut[10]; 





unsigned char TickOut(unsigned int * tick, unsigned int tickCnt)
{
     if(Tick_10ms - *tick >= tickCnt)  
     {
        *tick = Tick_10ms;
        return 1;
     }
     return 0;
}


/*********************************************************************************************************
** 函数名称: ReHC74165
** 功能描述: 
*********************************************************************************************************/
unsigned char ReHC74165(void)
{  
  unsigned char i;
  unsigned char indata;
		
   IN_PL=0;
   NOP();
   IN_PL=1;
   NOP();            
   
   indata=0;     
   for(i=0; i<9; i++)
    { 
	  indata=indata<<1;
      if(IN_Dat==1)indata=indata+1;
	  S_CLK=1;
      NOP();
      S_CLK=0;	  
	} 
   
   return(~indata);
 
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
	if((SendVal<<i)&0x80) MOSIO=1; // set dataline high
	else MOSIO=0;
 
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


