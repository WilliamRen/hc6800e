/****************************************************************************************
                          PS2 读电脑键盘实验											*
连接方法 ： P0口与J12连接 拔开跳线帽用杜邦线连接左针与P3.2  右针与P1.6  连接 		    *
使用方法：  用外部中断读取PS2键盘的数据 并在数码管上显示，此程序只处理数字键盘。	    *
 																						*
其余按键功能用户可以自行扩展，为了程序简单便于阅读和学习，程序中没有使用奇偶校验，	    *
校验程序请自行添加。           	*														*
														         	                    *
*****************************************************************************************/

#include<reg52.h>       

sbit LS138A=P2^2;  
sbit LS138B=P2^3;
sbit LS138C=P2^4; 

sbit BEEP = P1^5;

sbit Key_Data = P1^6 ;   //定义键盘引脚
sbit Key_CLK = P3^2;     //使用外部中断

/******************************************************************/
/*                    函数声明                                    */
/******************************************************************/
void Decode(unsigned char ScanCode);  //解码子程序
void delay(unsigned int i);      //函数声名
void display();
void beep();


// 此表为 LED 的字模            // 0    1    2    3    4    5    6   7    8    9    A    b    c    d    E    -    L   P    U    Hidden  _ (20)
unsigned char code Disp_Tab[] = { ~0xC0,~0xF9,~0xA4,~0xB0,~0x99,~0x92,~0x82,~0xF8,~0x80,~0x90,~0x88,~0x83,~0xC6,~0xA1,~0x86,~0xbf,~0xc7,~0x8c,~0xc1, ~0xff,  ~0xf7 }; 
bit BF;          			
unsigned char KeyV;
unsigned char LedNumVal  ;
int KEY_val,IntNum;

/******************************************************************/
/*                    主函数                                      */
/******************************************************************/
void main()
{

IT1 = 0;                              //设外部中断1为低电平触发
EA = 1;                               //外部中断开
EX0 = 1;                              //开中断

  while(1)
 {
   if (BF)
    {  
	if (KeyV != 240)  beep();  
	  
       Decode(KeyV);	//解码
	   
	  }
    else
      EA = 1; //开中断	 
	  display();
 }

}

/******************************************************************/
/*                    外部中断读入信息                            */
/******************************************************************/
void Keyboard_out(void) interrupt 0
{
if ((IntNum > 0) && (IntNum < 9))
   { 
	KeyV = KeyV >> 1;                //因键盘数据是低>>高，结合上一句所以右移一位
	if (Key_Data)
    	KeyV = KeyV | 0x80;          //当键盘数据线为1时到最高位
	}
	IntNum++;
	while (!Key_CLK);                //等待PS/2CLK拉高

	if (IntNum > 10)
	   { 
 		IntNum = 0;                  //当中断11次后表示一帧数据收完，清变量准备下一次接收
 		BF = 1;                      //标识有字符输入完了
		EA = 0;                      //关中断等显示完后再开中断 
		}

  }

/******************************************************************/
/*                    解码信息                                    */
/******************************************************************/
void Decode(unsigned char ScanCode) //注意:如SHIFT+G为12H 34H F0H 34H F0H 12H，也就是说shift的通码+G的通码+shift的断码+G的断码
{
    
  	switch (ScanCode)
			{
			case 0x70 :             // 当收到0xF0，Key_UP置1表示断码开始
				KEY_val = 0;
				break;

			case 0x69 :             
				KEY_val = 1;
				break;

			case 0x72 :             
				KEY_val = 2;
				break;

			case 0x7A :             
			    KEY_val = 3;
				break;

			case 0x6B :             
				KEY_val = 4;
				break;

			case 0x73 :             
				KEY_val = 5;
				break;

			case 0x74 :             
				KEY_val = 6;
				break;

			case 0x6c :             
				KEY_val = 7;
				break;

			case 0x75 :             
				KEY_val = 8;
				break;

			case 0x7d :             
				KEY_val = 9;
				break;

		   case 0x71 :
		                
			//	dot = 0X7F;
				break;
	
	}
 	
  
BF = 0; //标识字符处理完了 	
} 


/***************************************************************************************/
void display( )
{
   unsigned char i;
   unsigned int LedOut[8];
 
   LedNumVal = KEY_val ;


	 LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
     LedOut[1]=Disp_Tab[LedNumVal%1000/100];
     LedOut[2]=Disp_Tab[LedNumVal%100/10];
     LedOut[3]=Disp_Tab[LedNumVal%10];
	 
	 LedOut[4]=Disp_Tab[LedNumVal%10000/1000];	   //千位
     LedOut[5]=Disp_Tab[LedNumVal%1000/100];  //百位带小数点
     LedOut[6]=Disp_Tab[LedNumVal%100/10];		   //十位
     LedOut[7]=Disp_Tab[LedNumVal%10];             //个位
	  
	
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
		 
		delay(50);
	  }
 
}

/***************************************************************************************
延时程序																			   *
****************************************************************************************/
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}

/***************************************************************************************
蜂铃器发声																			   *
****************************************************************************************/
void beep()
{
  unsigned char i;
  for (i=0;i<255;i++)
   {
   delay(5);
   BEEP=!BEEP;                 //BEEP取反
   } 
  BEEP=1;                      //关闭蜂鸣器
}

