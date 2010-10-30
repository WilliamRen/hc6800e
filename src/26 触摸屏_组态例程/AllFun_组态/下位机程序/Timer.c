#include "system.h"

tab_1302[7]={0,11,1,29,7,2,8};     //初始化时时间日期设置

unsigned char l_tmpdisplay[8];

bit ReadRTC_Flag;//定义读DS1302标志

code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //秒分时日月周年 最低位读写位
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  
code unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 
			         //共阴数码管 0-9  '-' '熄灭‘表

 void Write_Ds1302_byte(unsigned char temp); 
 void Write_Ds1302( unsigned char address,unsigned char dat );
 unsigned char Read_Ds1302 ( unsigned char address );
 void Read_RTC(void);//read RTC 
 void InitTimer(void); //set RTC 
 //void InitTIMER0(void);//inital timer0

/******************************************************************/
/*                   写一个字节                                   */
/******************************************************************/
void Write_Ds1302_Byte(unsigned  char temp) 
{
 xdata unsigned char i;
 for (i=0;i<8;i++)     	//循环8次 写入数据
  { 
   SCK=0;
     SData=temp&0x01;     //每次传输低字节 
     temp>>=1;  		//右移一位
     SCK=1;
   }
}   
/******************************************************************/
/*                  写入DS1302                                    */
/******************************************************************/
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
   	_nop_();                    //启动
 	Write_Ds1302_Byte(address);	//发送地址
 	Write_Ds1302_Byte(dat);		//发送数据
 	RST=0;  		            //恢复
}
/******************************************************************/
/*                   读出DS1302数据                               */
/******************************************************************/
unsigned char Read_Ds1302 ( unsigned char address )
{
  xdata	 unsigned  char i,temp=0x00;
 	RST=0;
	_nop_();
	_nop_();
 	SCK=0;
	_nop_();
	_nop_();
 	RST=1;
	_nop_();
	_nop_();
 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 		//循环8次 读取数据
 	{		
 		if(SData)
 		temp|=0x80;			//每次传输低字节
		SCK=0;
		temp>>=1;			//右移一位
		_nop_();
	   _nop_();
	   _nop_();
 		SCK=1;
	} 
 	RST=0;
	_nop_();	          	//以下为DS1302复位的稳定时间
 	_nop_();
	RST=0;
	SCK=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCK=1;
	_nop_();
	_nop_();
	SData=0;
	_nop_();
	_nop_();
	SData=1;
	_nop_();
	_nop_();
	return (temp);			//返回
}
/******************************************************************/
/*                   读时钟数据                                   */
/******************************************************************/
void Read_RTC(void)	        //读取 日历
{
 xdata unsigned char i,*p;
 p=read_rtc_address; 	    //地址传递
 for(i=0;i<7;i++)		    //分7次读取 秒分时日月周年
 {
  D[6+i]=Read_Ds1302(*p);
  p++;
 }
}
/******************************************************************/
/*                  设定时钟数据                                  */
/******************************************************************/
void InitTimer(void)		    //设定 日历
{
 xdata unsigned char i,*p;//tmp;
/*	for(i=0;i<7;i++){       //BCD处理
		tmp=l_tmpdate[i]/10;
		l_tmpdate[i]=l_tmpdate[i]%10;
		l_tmpdate[i]=l_tmpdate[i]+tmp*16;
	}	 */
 	Write_Ds1302(0x8E,0X00);   //允许写入
	
 	p=write_rtc_address;	//传地址	
 	for(i=0;i<7;i++)		//7次写入 秒分时日月周年
 	{
		 // Write_Ds1302(*p,l_tmpdate[i]);
		 Write_Ds1302(*p,tab_1302[i]);
		 
 		 p++;  
	 }
	 Write_Ds1302(0x8E,0x80);  //禁止写入

}
