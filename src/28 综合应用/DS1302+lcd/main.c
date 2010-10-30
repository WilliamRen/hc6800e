/**************************************************************************************
*																					  *
*  将P1口和JP5(独立按键)排线连接													  *
*																					  *
****************************************************************************************/

#include "SYSTEM.H"

/*--------------------------------------------------------------*/
SYSTEMTIME CurrentTime;

unsigned char k = 0;	//键值
unsigned char K1,K2,K3,M1,M2,M3,K1J,K2J,K3J,K4J;	//键值
unsigned char Set;	//调整值
char Set_Mode = 0;

void Keysan(void);
void delay200ms(void);


//主函数
void main(void)
{	
	lcd_init();
	//lcd_sef_chr();						//写入自定义字符号
	DS1302_Initial();

	 lcd_pos(0,0);	   //设置 Date:显示位置
	 prints("Date:");  //显示 Date:字符
	 lcd_pos(0,1);	   //设置 Time:显示位置
   	 prints("Time:");  
	k = 6; 	//不改变时间  
    while(1)
	{
	  Keysan();	   //按键检测
	 
	    DS1302_GetTime(&CurrentTime);  //获得DS1302时钟数据
		DateToStr(&CurrentTime);	   //日期转化成字符
		TimeToStr(&CurrentTime);	   //时间转化成字符
	  
	  
		lcd_pos(6,0);  //控制显示位置
		prints(CurrentTime.DateString);
		lcd_pos(6,1);  //控制显示位置
		prints(CurrentTime.TimeString);

		//lcd_pos(7,0);
	    //lcd_wcmd(0x0E);

		delay200ms();	//延时子程序
		
	}
}


/*--------------------------------------------------------------*/
//键盘扫描
void Keysan(void)
{
	
    if(~P1_2&K1J)   { k++;  }
 	 K1J=~(~P1_2);

    if(~P1_3&K2J)    { Set++; }
 	  K2J=~(~P1_3);

	if(~P1_4&K3J)    { Set--; }
 	 K3J=~(~P1_4);
	
   	if(k == 7)	{k = 0; }

	switch(k)
	{
			case 0:	DS1302_SetTime(DS1302_SECOND, Set&0x3f); break;	  //设置秒
			case 1: DS1302_SetTime(DS1302_MINUTE, Set&0x3f); break;	  //设置分
			case 2:	DS1302_SetTime(DS1302_HOUR  , Set&0x1f); break;	  //设置时
			case 3:	DS1302_SetTime(DS1302_DAY   , Set&0x0f); break;	  //设置日
			case 4:	DS1302_SetTime(DS1302_MONTH , Set&0x0f); break;	  //设置月
			case 5:	DS1302_SetTime(DS1302_YEAR  , Set&0x7f); break;	  //设置年
			default: break;
	}
}


/*--------------------------------------------------------------*/
//延时200000+0us 函数定义
void delay200ms(void)
{
  	unsigned char i,j,k;
  	for(i=201;i>0;i--)
  	for(j=32;j>0;j--)
  	for(k=14;k>0;k--);
}


/*--------------------------------------------------------------*/


