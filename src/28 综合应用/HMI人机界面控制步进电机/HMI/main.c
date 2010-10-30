/***********************************************************************
*                                                                      *
*              HMI人机界面步进电机调速系统                             *
*                （  普中科技  ）                                      *
*                                                                      *
* 实验目的：1.掌握TFT彩屏驱动程序的编写、							   *
*           2.学会运用取字模软件应用(取字符和汉字数组)				   *
*			3.掌握通过TFT显示平台如何对步进电机调速和控制转速          *
*                                                                      *
*实验现象：彩屏中显示汉字、数字、字母、光标闪烁！                      *
*                                                                      *
*功能:1.可以设定转速和圈数                                             *
      2.可以保存设定数据到AT24CO2EEPROM中，(当光标离开时，数据自动保存)*
*                                                                      *
* 建议：学员可以试着用51的定时中断来处理 马达运行，效果又怎样呢？      *
***********************************************************************/
#include "system.h"
#include "lcd.h"
#include"Function.h"


bit flag,Ch_Focus_J , Inc_num_J , Dec_num_J ;
unsigned char Set_Data[5] = {0,0,0,0} ;
unsigned char Index = 0 , old_Index;
unsigned char *pold;
unsigned char xdata  pDat[16];
unsigned int  xdata D[32]; 

unsigned char xdata X[] = {0,28,94,28,94,28,94,28,94};    //寄存器X坐标
unsigned char xdata Y[] = {0,24,24,60,60,96,96,132,132};  //寄存器Y坐标
/***********************************************************
purpose: 系统初始化
/**********************************************************/
void system_Ini()
{ 
    TMOD|= 0x11;
   	IE = 0x8A;

    TH0=0xDC; //11.0592M
    TL0=0x00;
    TR0  = 1;
 
}

/**********************************************************

  主函数

**********************************************************/
void main(void)
{   char i;
    

   P2 = 0xff;
   P0 = 0xff;

   LCD_init();
   LCD_clear(5);

     
   ChineseChar(4,4,1,colors[0],colors[5],1);	//步
   ChineseChar(20,4,1,colors[0],colors[5],2);	//进
   ChineseChar(36,4,1,colors[0],colors[5],3);	//电
   ChineseChar(52,4,1,colors[0],colors[5],4);	//机
   ChineseChar(68,4,1,colors[0],colors[5],5);	//一
   
    //步进马达一
   ChineseChar(2,24,0,colors[0],colors[5],1);	//转
   ChineseChar(14,24,0,colors[0],colors[5],2);	//速
   ChineseChar(64,24,0,colors[0],colors[5],3);	//圈
   ChineseChar(76,24,0,colors[0],colors[5],4);	//数

/*****************************************************************/
   ChineseChar(4,40,1,colors[0],colors[5],1);	//步
   ChineseChar(20,40,1,colors[0],colors[5],2);	//进
   ChineseChar(36,40,1,colors[0],colors[5],3);	//电
   ChineseChar(52,40,1,colors[0],colors[5],4);	//机
   ChineseChar(68,40,1,colors[0],colors[5],6);	//二
 
   //步进马达二
   ChineseChar(2,60,0,colors[0],colors[5],1);	//圈 
   ChineseChar(14,60,0,colors[0],colors[5],2);	//数
   ChineseChar(64,60,0,colors[0],colors[5],3);	//转
   ChineseChar(76,60,0,colors[0],colors[5],4);	//速

/*****************************************************************/
   ChineseChar(4,76,1,colors[0],colors[5],1);	//步
   ChineseChar(20,76,1,colors[0],colors[5],2);	//进
   ChineseChar(36,76,1,colors[0],colors[5],3);	//电
   ChineseChar(52,76,1,colors[0],colors[5],4);	//机
   ChineseChar(68,76,1,colors[0],colors[5],7);	//三
   
    //步进马达三
   ChineseChar(2,96,0,colors[0],colors[5],1);	//圈 
   ChineseChar(14,96,0,colors[0],colors[5],2);	//数
   ChineseChar(64,96,0,colors[0],colors[5],3);	//转
   ChineseChar(76,96,0,colors[0],colors[5],4);	//速

/*****************************************************************/
   ChineseChar(4,112,1,colors[0],colors[5],1);	//步
   ChineseChar(20,112,1,colors[0],colors[5],2);	//进
   ChineseChar(36,112,1,colors[0],colors[5],3);	//电
   ChineseChar(52,112,1,colors[0],colors[5],4);	//机
   ChineseChar(68,112,1,colors[0],colors[5],8);	//四

    //步进马达四
   ChineseChar(2,132,0,colors[0],colors[5],1);	//圈 
   ChineseChar(14,132,0,colors[0],colors[5],2);	//数
   ChineseChar(64,132,0,colors[0],colors[5],3);	//转
   ChineseChar(76,132,0,colors[0],colors[5],4);	//速
  
    
    ReadData();  // 读取上次设定数据
      
    for (i = 1; i<9 ; i++)
   {
       handle_ASCII(i);
       LCD_ShowString(X[i],Y[i],colors[4],colors[7],Set_Data);
   }
 
   system_Ini();
  
   while(1)	
   {     
      
      ChangeNumber();
      changefocus(Index);
      MotorRun(D);
   }

}

/*********************************************************
*
*               改变光标和设定数据
*
**********************************************************/

void ChangeNumber()
{  static unsigned int C_Inc , C_Dec;
     //按钮脉冲处理
   if(~Ch_Focus & Ch_Focus_J)  
	{   
   	   old_Index = Index;  //保存上次光标的位置
       pold = Set_Data;    //保存上次光标数据
    
       Index++; //改变光标
       if(Index > 8) Index = 0;

       if (X[old_Index] != 0)  LCD_ShowString(X[old_Index],Y[old_Index],colors[4],colors[7],pold); // 显示前一个光标内容
     
       SaveData(); //光标改变时 保存数据
      
   	} 
     Ch_Focus_J=~(~Ch_Focus);

/**************************************************************************/
 
	 if(Index > 0)  
	 {    // 加数据 按钮脉冲处理
	   if(~Inc_num&Inc_num_J)  
		{   
	   		 //D[Index]++;
         if( D[Index]< 9999)  D[Index]++;
         if(D[Index]> 9999) D[Index] = 0;
	   	} //if
	  	Inc_num_J=~(~Inc_num);

         // 减数据 按钮脉冲处理
       if(~Dec_num&Dec_num_J)  
		{   
	   	//	 D[Index]--;
         if( D[Index]> 0 ) D[Index]--;
         if(D[Index]> 9999) D[Index] = 0;
	   	} //if
	  	Dec_num_J=~(~Dec_num);
	}//if
/**********************************************************/
//       快速加减调整数据   
   if(!Inc_num & Dec_num) 
   {
      if( C_Inc <= 30)   C_Inc++;
      else 
      {  if( D[Index]< 9999)  D[Index]++;
         if(D[Index]> 9999) D[Index] = 0;
      }
   }//if
   else   C_Inc = 0;

   if(!Dec_num & Inc_num) 
   {
      if(C_Dec <= 30) C_Dec++;
      else 
       { if( D[Index]> 0 ) D[Index]--;
         if(D[Index]> 9999) D[Index] = 0;
       }
   }//if  
   else C_Dec = 0;

  

/**********************************************************************************/



}



/********************************************************
*                                                       *
*                改变光标位置                           *
*                                                       *
*********************************************************/
void changefocus(unsigned char index)
{
  handle_ASCII(Index);
 switch(index)
 {
   case 1:
   Focus_Flash(X[1],Y[1]);  
   break;

   case 2:
   Focus_Flash(X[2],Y[2]);
   break;
   
   case 3:
   Focus_Flash(X[3],Y[3]);
   break;

   case 4:
   Focus_Flash(X[4],Y[4]);
   break;

   case 5:
   Focus_Flash(X[5],Y[5]);
   break;
   
   case 6:
   Focus_Flash(X[6],Y[6]);
 
   break;

   case 7:
   Focus_Flash(X[7],Y[7]);
  
   break;

   case 8:
   Focus_Flash(X[8],Y[8]);
   break;

   }

}


/*********************************************************
*
*      将数据分解ASCII码
*
**********************************************************/

void handle_ASCII(unsigned char index)
{    unsigned int i;
     
     i = D[index];

     Set_Data[0] = i%10000/1000  + 0x30;	   //千位+0x30 转换成ASCII编码
     Set_Data[1] = i%1000/100  + 0x30;         //百位+0x30 转换成ASCII编码
     Set_Data[2] = i%100/10 + 0x30;            //十位
     Set_Data[3] = i%10 + 0x30;                //个位
     Set_Data[4] = '\0';

}

/********************************************************
*                                                       *
*  光标闪烁程序                                         *
*                                                       *
*********************************************************/
void Focus_Flash(unsigned char x, unsigned char y)
{ 

 if(flag)
   
   LCD_ShowString(x,y,colors[4],colors[7],Set_Data);
 
 else

   LCD_ShowString(x,y,colors[7],colors[0],Set_Data);
 
}




/********************************************************
*                  保存数据                             *
*                                                       *
*********************************************************/
void SaveData()
{ char i ;
 
  for (i=0; i<8; i++)
 {
    pDat[i*2+0]=D[1+i];
    pDat[i*2+1]=D[1+i]>>8;
 }
   ISendStr(AT24C02, 0 , pDat, 8); 
   ISendStr(AT24C02+1, 0 , pDat+8, 8); 

}

/********************************************************
*                  读取设定数据                         *
*                                                       *
*********************************************************/
void ReadData()
{ unsigned char i;
    IRcvStr(AT24C02, 0 , pDat, 8);   
    IRcvStr(AT24C02+1, 0 , pDat+8, 8); 
    for (i=0; i<8; i++)
	{
	   D[1+i]=pDat[i*2+0] + pDat[i*2+1]*0x100;
 	}

}

/*************************************
 [ t1 (1ms)中断] 中断

*************************************/
void T1zd(void) interrupt 1      //3定时器1的中断号  1定时器0的中断号 0外部中断1 2外部中断2  4串口中断
{ static unsigned int LedNumVal;
	// TH1 = 0x1c;    //12.000
	// TL1 = 0x18;

    TH0 = 0xfe;    //11.0592
    TL0 = 0x33;

     // TH0 = 0x1c;    //11.0592
     // TL0 = 0x18;
    
    if(LedNumVal++ > 1100)  //改变闪烁频率
    {
     flag = ~flag;  //改变光标标志
     LedNumVal = 0 ;

      
    }

  
 Tick_10ms++;
	
 
}

