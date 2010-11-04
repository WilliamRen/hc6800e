/*************************此部分为18B20的驱动程序*************************************/
#include "common.h"

bit presence  ;
unsigned char data  temperture_data[2]        = {0x00,0x00} ;
unsigned char data  temperture_display[5]     = {0x00,0x00,0x00,0x00,0x00} ;
unsigned char code  myditab[16]               = {0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,
                                                 0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09} ;
extern void lcd_pos(uchar pos);
extern void lcd_wdat(uchar dat);
/*初始化ds1820                                      */
/*******************************************************************/
Init_DS18B20(void)
{  
     DQ = 1 ;           //DQ复位
     delay_us(8) ;      //稍做延时
     DQ = 0 ;           //单片机将DQ拉低
     delay_us(90) ;     //精确延时 大于 480us
     DQ = 1 ;           //拉高总线
     delay_us(8) ;
     presence = DQ ;    //如果=0则初始化成功 =1则初始化失败
     delay_us(100) ;
     DQ = 1 ; 
     return(presence) ; //返回信号，0=presence,1= no presence
}

/* 读一个字节                     */
/*******************************************************************/
ReadOneChar(void)
{
	unsigned char i = 0 ;
	unsigned char dat = 0 ;
	for (i = 8 ; i > 0 ; i--)
	{
    	DQ = 0 ; // 给脉冲信号
    	dat >>= 1 ;
    	DQ = 1 ; // 给脉冲信号
		if(DQ)
			dat |= 0x80 ;
		delay_us(4) ;
	}
    return (dat) ;
}

/* 写一个字节                                         */
/*******************************************************************/
WriteOneChar(unsigned char dat)
{
	unsigned char i = 0 ;
	for (i = 8 ; i > 0 ; i--)
	{
    	DQ = 0 ;
    	DQ = dat&0x01 ;
    	delay_us(5) ;
	    DQ = 1 ;
    	dat>>=1 ;
  	}
}

/* 读取温度                            */
/*******************************************************************/
Read_Temperature(void)
{
     Init_DS18B20() ;
  
     WriteOneChar(0xCC) ;  // 跳过读序号列号的操作
     WriteOneChar(0x44) ;  // 启动温度转换

     Init_DS18B20() ;
     WriteOneChar(0xCC) ;  //跳过读序号列号的操作
     WriteOneChar(0xBE) ;  //读取温度寄存器

     temperture_data[0] = ReadOneChar() ;   //温度低8位
     temperture_data[1] = ReadOneChar() ;   //温度高8位 
}

/* 数据转换与温度显示                                              */
/*******************************************************************/
Disp_Temperature()
{
	temperture_display[4]=temperture_data[0]&0x0f ;
  	temperture_display[0]=myditab[temperture_display[4]]+0x30 ;     //查表得小数位的值
  
  	temperture_display[4]=((temperture_data[0]&0xf0)>>4)|((temperture_data[1]&0x0f)<<4) ;
  	temperture_display[3]=temperture_display[4]/100+0x30 ;
  	temperture_display[1]=temperture_display[4]%100 ;
  	temperture_display[2]=temperture_display[1]/10+0x30 ;
  	temperture_display[1]=temperture_display[1]%10+0x30 ;

    if(temperture_display[3]==0x30)        //高位为0，不显示
   	{ 
     	temperture_display[3]=0x20 ;              
     	if(temperture_display[2]==0x30)      //次高位为0，不显示
     	temperture_display[2]=0x20 ;
   	}

    lcd_pos(0x48) ;             
    lcd_wdat(temperture_display[3]) ;        //百位数显示 
    lcd_pos(0x49) ;             
    lcd_wdat(temperture_display[2]) ;        //十位数显示 
    lcd_pos(0x4a) ;             
    lcd_wdat(temperture_display[1]) ;        //个位数显示 
    lcd_pos(0x4c) ;             
    lcd_wdat(temperture_display[0]) ;        //小数位数显示 
}

/* 主函数     */
/************************************/
void test_18B20()
{
    lcd_temp_menu();
	do
	{
		Read_Temperature() ;
   		Disp_Temperature() ;
	}while(is_quit()&&!presence);
	do
	{
		Init_DS18B20() ;
    	beep() ;
	}while(is_quit()&&presence);

}

