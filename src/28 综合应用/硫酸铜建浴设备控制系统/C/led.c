/***********************************************************************************
*  标题:                 硫酸铜建浴设备控制系统                                    *
*																			       *
*  连接方法：P1与J12 用8PIN排线连接											       *
*            P3与JP5按钮连接                                                       *
             P0与JP1(LED灯连接)												       *
			 P3.4为 + 按钮   P3.5为 - 按钮    P3.7为光标按钮				       *
			 P3.0 搅拌机开关 P3.1 水泵快关    P3.2 报警解除					       *
具体控制要求: 1.按下 P3.4和P3.5不放 按下 P3.7 即可进入设置模式，再次按下退出设置模式 			
			  左边四位数码管最左边的开始闪烁，后面的四位数码管不属于此控制系统     *
			  按下加或减 有 A b C 三档 分别代表三个参数。					       *
			  第一个参数(A)：搅拌机运行时间									       *
			  第二个参数(b)：液位信号保持时间								       *
			  第三个参数(C): 为定义											       *
																			       *
	         各参数保存在STC89C52单片机的内部EEPROM中						       *
																			       *
更详细控制要求参考 <<控制功能表>>											       *			
************************************************************************************/

#include "system.h"

//此表为 LED 的字模, 共阴数码管 0-9  - 
unsigned char code Disp_Tab[] = {~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x6f,~0x40}; 


							   //
unsigned char code Disp_alph[] ={~0x88,~0x83,~0xC6,~0xA1,~0x86,~0xbf,~0xc7,~0x8c,~0xc1};
								//P     R/A	  E	    S
unsigned char code alarm_Tab[] ={~0x0c ,~0x88 ,~0x86 ,~0x92 }; 

unsigned int  D[10] , LedOut[8] , LedNumVal , user_T1 , temp_val;
unsigned char user_T , pos,val  , pos_alph , monitor_Number;
unsigned int temp,Tick_10ms,Tick1_10ms,Tick_1min,Tick2_10ms ;
bit M0, M1,M3,M4,SET_J ,Jiao_ban_J, shui_beng_J,Flag_Tim,Flag_C,Flag_set,alarm_flag,liq_alarm;
char val2;
void delay(unsigned int i);      //函数声名
void Display();
void set_mode();
void Act_Contrl();
void Run_mode(unsigned char run_monitor);
unsigned int distribute_Data(unsigned char pos_alph );
void Data_Change(unsigned int lednmval, unsigned char position);
void MUL_display(unsigned char one ,unsigned char two,unsigned char three,unsigned char four);


main()
{   TMOD|= 0x11;
    TH1=0xD8;   //12.000M
    TL1=0xF0;	
	IE = 0x88;	
    TR1  = 1; 
	
 	rdData(0x0000, 10);

	pos = 0;
	val = 0;
	Flag_set = 0;
  
  while(1)
  {	 
   	char i;
    
	  //实现脉冲输入
    if(~SET&SET_J)  
	{   
   	  if (!ADD & !SUB) { M0 =~M0 ; pos = 1;}   //同时按下 加 、减、按钮SET按钮进入设置模式 
	   
	  if (Flag_set == 1 & ADD & SUB ) pos ++;
   	} 
  	SET_J=~(~SET);

    if (pos > 4) pos = 1; 	 //控制光标循环移动

	 if (M0) { 	set_mode();	  Flag_set = 1;	}  //进入设置模式
	 
     if (Flag_set & ~M0)   //退出设置模式
	 {
	   Flag_set = 0; //清除设置标志位
	   pos = 0; 	 //清除光标位置
	   val = 0;		 //清除改变参数设置变量
	   temp = 0 ;
	   for (i = 0 ; i<4; i++) W_buffer[i] = D[i];
	   earseSector(0x0000);            //擦除EEPROM
	   wrData(0x0000, 10);             //写入EEPROM

	 }

   	Act_Contrl();  //调用动作控制程序
   	Display();	   //调用显示程序
  }	 

}

/*************************************************************************************
*          各动作控制程序															 *				 
*																					 *
**************************************************************************************/
void Act_Contrl()
{
    
	P0 = 0xff;
	P2 = 0xff;
	monitor_Number = 9;
   
	if(~liq_level_low)
	{
		if(Tick2_10ms >= D[1]*100)	{liq_alarm = 1;}
	}
	else
	{Tick2_10ms = 0;
	liq_alarm = 0;
	}
   

   //搅拌脉冲处理
   if(~Jiao_ban&Jiao_ban_J)  
	{   
   		 M3 = ~M3;
   	} 
  	Jiao_ban_J=~(~Jiao_ban);


	//水泵脉冲处理 
	 if(~shui_beng&shui_beng_J)  
	{   
   	     M4 = ~M4;
   	} 
  	shui_beng_J=~(~shui_beng);


		//系统待机
	if(jiao_relay & shui_relay) {monitor_Number =9;}


	 //搅拌机运行
	if(M3) {jiao_relay = 0; monitor_Number =1; }

	

	 //搅拌机故障
	if(~jiao_ban_Err) {alarm_flag = 1; M3 = 0; monitor_Number =2; }
		
     //水泵运行
	if(M4) {shui_relay = 0; monitor_Number =3;}

	  //水泵故障
	if(~shui_beng_Err) {alarm_flag = 1; M4 = 0; monitor_Number =4;}
   	
	//搅拌机和水泵同时运行
	if(M3 & M4 ) {monitor_Number =5;}

	//搅拌机和水泵同时故障
	if(~jiao_ban_Err & ~shui_beng_Err)	{monitor_Number =6;}

	 //储罐液位低
	if(liq_alarm ) {alarm_flag = 1; M4 = 0; monitor_Number =7;}

	 //电源故障
	if(~power_Err)	{alarm_flag = 1; monitor_Number =8;}
  
	 //搅拌机及水泵同时动作 并显示当前时间
	 if(M3)
	 { if (Tick_10ms >= 6000 )  {Tick_1min++;Tick_10ms = 0;}
		   if (D[0]!=0)
		   { if (Tick_1min == D[0])  {M3 = 0;Tick_1min = 0;Tick_10ms = 0;}
			 if (M3&~M4) monitor_Number =10;
		     if (M3&M4)  monitor_Number =11;
		   }
		   else{// 以防工作时改变参数
		    Tick_1min = 0;
		    Tick_10ms = 0;
		   }
	 }
	 else
	 {
	   Tick_10ms = 0;
	   Tick_1min = 0;
	 }

	  // 报警解除
	  if(!alarm_remove & alarm_flag) alarm_flag = 0;
   	  if(jiao_ban_Err & shui_beng_Err & power_Err & liq_level_low & alarm_flag)
	  {
	    alarm_flag = 0;
	  }
	  if(alarm_flag)	beep = 0;

	  //测试报警器
	  if(!alarm_remove) beep = 0; 

	 if (~Flag_set) Run_mode(monitor_Number);   //调用运行监视显示
	 

}

/*************************************************************************************
*          运行模式（显示当前运行情况												 *				 
*																					 *
**************************************************************************************/
void Run_mode(unsigned char run_monitor)	 //运行监视显示
{

     switch (run_monitor)
	{
	  case 1:  //搅拌机运行中  PR01
	  {MUL_display(alarm_Tab[0] , alarm_Tab[1] , Disp_Tab[0] , Disp_Tab[1]) ;
	   break;
	  }

	  case 2:  //搅拌机故障	   PE01
	  {MUL_display(alarm_Tab[0] , alarm_Tab[2] , Disp_Tab[0] , Disp_Tab[1]) ;
	   break;
	  }
 
	  case 3:  //搅拌水泵运行中	 PR02
	  {MUL_display(alarm_Tab[0] , alarm_Tab[1] , Disp_Tab[0] , Disp_Tab[2]) ;
	   break;
	  }

	  case 4:  //搅拌水泵故障	 PE02
	  {MUL_display(alarm_Tab[0] , alarm_Tab[2] , Disp_Tab[0] , Disp_Tab[2]) ;
	   break;
	  }

	  case 5:  //搅拌机和水泵同时在运行	   PR12
	  {MUL_display(alarm_Tab[0] , alarm_Tab[1] , Disp_Tab[1] , Disp_Tab[2]) ;
	   break;
	  }
										  
	  case 6:  //搅拌机和水泵同时故障  PE12
	  {MUL_display(alarm_Tab[0] , alarm_Tab[2] , Disp_Tab[1] , Disp_Tab[1]) ;
	   break;
	  }

	  case 7:  //储罐低液位		 E001
	  {MUL_display(alarm_Tab[2] , Disp_Tab[0] , Disp_Tab[0] , Disp_Tab[1]) ;
	   break;
	  }

	  case 8:  //电源故障	   E002
	  {MUL_display(alarm_Tab[2] , Disp_Tab[0] , Disp_Tab[0] , Disp_Tab[2]) ;
	   break;
	  }

	  case 9:  //系统待机状态 S000
	  {MUL_display(alarm_Tab[3] ,Disp_Tab[0] , Disp_Tab[0] , Disp_Tab[0]) ;
	   break;
	  }

	  case 10:  //搅拌机运行中 显示当前时间
	  {MUL_display(alarm_Tab[0], Disp_Tab[(D[0]-Tick_1min)%1000/100], Disp_Tab[(D[0]-Tick_1min)%100/10], Disp_Tab[(D[0]-Tick_1min)%10]);
	   break;
	  }

	  case 11:  //搅拌机和水泵在运行，并启动定时搅拌
	  {MUL_display(alarm_Tab[1], Disp_Tab[(D[0]-Tick_1min)%1000/100], Disp_Tab[(D[0]-Tick_1min)%100/10], Disp_Tab[(D[0]-Tick_1min)%10]);
	   break;
	  }

	}  
}


/*************************************************************************************
*          设定模式																	 *				 
*																					 *
**************************************************************************************/
void set_mode()		//设定模式
{   
   switch (pos)
	{
	   case 1:
	   {pos_alph = val;  
	    temp_val = distribute_Data (pos_alph);	   //分发数据
	   	LedNumVal = temp_val ;
	    break;
	    }
	 	    
	   case 2:
	   {LedNumVal = temp_val + (val2*100); 
	    Data_Change(LedNumVal , pos_alph);
        break;
	   }

	   case 3:
	   {LedNumVal = temp_val + (val2*10);    
	   	Data_Change(LedNumVal , pos_alph);
		break;
	   }

	   case 4:
	   {LedNumVal = temp_val + val2;
	    Data_Change(LedNumVal , pos_alph);
	   break;
	   }
	}

   MUL_display(0, 0, 0, 0);
}

void Data_Change(unsigned int lednmval, unsigned char position)
{
	 if(lednmval >= 999) lednmval = 999;
     D[position] = lednmval;
	 temp_val = lednmval;
	 val2 = 0;
	 

}

unsigned int distribute_Data(unsigned char pos_alph )
{
  unsigned int temp_val;	
	switch (pos_alph)
   {
	 case 0:
	 { temp_val = D[0];
	  
	 break;
	 }

	 case 1:
	 { temp_val = D[1];
	  
	  break;
	 }

	 case 2:
	 { temp_val = D[2];
	   
	  break;
	 }
   }

    return temp_val;
}


/*************************************************************************************
*          多重过程报警显示处理子程序												 *				 
*																					 *
**************************************************************************************/
void MUL_display(unsigned char one ,unsigned char two,unsigned char three,unsigned char four)
{
    if(Flag_set)  
  {	 
	 
	 LedOut[0]=Disp_alph[pos_alph];	   //千位
     LedOut[1]=Disp_Tab[LedNumVal%1000/100];      //百位带小数点
     LedOut[2]=Disp_Tab[LedNumVal%100/10]&0x7f;   //十位
     LedOut[3]=Disp_Tab[LedNumVal%10];            //个位

	 LedOut[4]=Disp_Tab[LedNumVal%10000/1000];	   //千位
     LedOut[5]=Disp_Tab[LedNumVal%1000/100]&0x7f;
     LedOut[6]=Disp_Tab[LedNumVal%100/10];
     LedOut[7]=Disp_Tab[LedNumVal%10];
  }
  else
  {
     		
	LedOut[0]=one;      //Disp_Tab[monitor_Number];	 
    LedOut[1]=two;
    LedOut[2]=three;
    LedOut[3]=four;	 

	LedOut[4]=Disp_Tab[Tick2_10ms%10000/1000];	   //千位
    LedOut[5]=Disp_Tab[Tick2_10ms%1000/100]&0x7f;
    LedOut[6]=Disp_Tab[D[1]%100/10];
    LedOut[7]=Disp_Tab[D[1]%10];
  }

}

/*************************************************************************************
*          数码管显示处理															 *				 
*																					 *
**************************************************************************************/

void Display()
{   
  	unsigned int i; 
	 for( i=0; i<8; i++) 
	 {	
			
	  switch(i)					  
	     {	    
			case 0:
			if ((pos == 1 )&Flag_Tim & (ADD&SUB))
			{LedOut[0] = 0xff;
			break;
			}
			else
			{LS138A=0; LS138B=0; LS138C=0; 
			 //L1 = 0;  L2 = 1;  L3 = 1; L4 = 1;
			break; 
			} 
			       
	        case 1:
			if ((pos == 2 )&Flag_Tim& (ADD&SUB))
			{LedOut[1] = 0xff;
			break;
			}
			else
			{LS138A=1; LS138B=0; LS138C=0; 
			 //L1 = 1;  L2 = 0;  L3 = 1; L4 = 1;
			break;}
			             	
	        case 2:
			if ((pos == 3 )&Flag_Tim& (ADD&SUB))
			{LedOut[2] = 0xff;
			break;
			}
			else
			{LS138A=0; LS138B=1; LS138C=0; 
			 //L1 = 1;  L2 = 1;  L3 = 0; L4 = 1;
			break;} 

	        case 3:
			if ((pos == 4 )&Flag_Tim& (ADD&SUB))
			{LedOut[3] = 0xff;
			break;
			}
			else 
			{LS138A=1; LS138B=1; LS138C=0; 
			 //L1 = 1;  L2 = 1;  L3 = 1; L4 = 0;
			break;}
			 
			case 4:
			if ((pos == 1 )&Flag_Tim& (ADD&SUB))
			{LedOut[4] = 0xff;
			break;
			}
			else
			{LS138A=0; LS138B=0; LS138C=1; 
			 //L1 = 1;  L2 = 0;  L3 = 0; L4 = 0;
			 break;}

			case 5:
			if ((pos == 2 )&Flag_Tim& (ADD&SUB))
			{LedOut[5] = 0xff;
			break;
			}
			else
			{LS138A=1; LS138B=0; LS138C=1; 
			// L1 = 0;  L2 = 1;  L3 = 0; L4 = 0;
			break;}

			case 6:
			if ((pos == 3 )&Flag_Tim& (ADD&SUB))
			{LedOut[6] = 0xff;
			break;
			}
			else
			{LS138A=0; LS138B=1; LS138C=1; 
			 //L1 = 0;  L2 = 0;  L3 = 1; L4 = 0;
			break;}

			case 7:
			if ((pos == 4  )&Flag_Tim& (ADD&SUB))
			{  LedOut[7] = 0xff;
		    	break;
			}
			else
			{LS138A=1; LS138B=1; LS138C=1; 
			 //L1 = 0;  L2 = 0;  L3 = 0; L4 = 1;
			break; }  	
			
	     }
		   P1 = LedOut[i];
		  
		delay(150);
	  }
	     P1 = 0xff;
}
	
/*************************************************************************************
*          延时程序														       	     *				 
*																					 *
**************************************************************************************/
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


 /*************************************
 [ t1 (10ms)中断] 中断

*************************************/
void T1zd(void) interrupt 3      //3定时器1的中断号  1定时器0的中断号 0外部中断1 2外部中断2  4串口中断
{

	 TH1 = 0xD8;    //12.000
	 TL1 = 0xF0;

	 Tick_10ms++;
	 Tick1_10ms++;
	 Tick2_10ms++;

 	if (++user_T == 30)  
	{Flag_Tim = ~Flag_Tim; 
	 user_T = 0;
	 }

   if (++user_T1 == 30) 
   {   
 
	if (Flag_set)  
	{   if (pos == 1)
	   {
	   	if (!ADD &SUB)  val = (++temp)%3;
        if (!SUB &ADD ) val = (--temp)%3;
	   }
  	   else  
	   {
	   	if (!ADD & SUB & LedNumVal <999)  val2++ ;
        if (!SUB & ADD & val2 >=-1 ) val2-- ;
	   }
	 }	

	  user_T1 = 0;
	}

}