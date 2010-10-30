# include "SYSTEM.H"													 /*--------------------------------------------------------------*/

/*--------------------------------------------------------------*/
//#ifndef __LCD1602_8_H__
//#define __LCD1602_8_H__



/*--------------------------------------------------------------*/
//LCD接口定义					
sfr		 io	= 0x80;				//P0-0x80,P1-0x90,P2-0xA0,P3-0xB0;
sbit	 rs = P2^6;				//LCD数据/命令选择端(H/L)
sbit	 rw = P2^5;				//LCD读/写选择端(H/L)
sbit	 ep = P2^7;				//LCD使能控制
sbit     bz = io^7;				//LCD忙标志位


/*--------------------------------------------------------------*/
//函数声明		
void lcd_busy(void);					//测试LCD忙碌状态程序
void lcd_wcmd(unsigned char cmd);		//写入指令到LCD程序
void lcd_wdat(unsigned char dat);		//写入数据到LCD程序
void lcd_pos (unsigned char x, bit y);	//LCD数据指针位置程序
void lcd_sef_chr();
//void printc(unsigned char CHAR);		//显示字符

void lcd_init(void);					//LCD初始化设定程序


/*--------------------------------------------------------------*/
//测试LCD忙碌状态
void lcd_busy(void)
{	
	do{
		ep = 0;
		rs = 0;		//指令
		rw = 1;		//读出
		io = 0xff;
		ep = 1;
		_nop_();	//高电平读出	1us	
	}while(bz);		//bz=1表示忙,bz=0表示空闲
	ep = 0;		
}


/*--------------------------------------------------------------*/
//写入指令到LCD
void lcd_wcmd(unsigned char cmd)
{							
	lcd_busy();	//检测忙
	rs = 0;		//指令
	rw = 0;		//写入
	ep = 1;	
	io = cmd;	//指令
	ep = 0;		//下降沿有效	
}


/*--------------------------------------------------------------*/
//写入数据函数
void lcd_wdat(unsigned char Data)
{
	lcd_busy();  //检测忙
	rs = 1;		 //数据
	rw = 0;		 //写入
	ep = 1;
	io = Data;	 //数据
	ep = 0;		 //下降沿有效
}


/*--------------------------------------------------------------*/
//LCD数据指针位置程序
void lcd_pos(unsigned char x, bit y)
{						
	if(y)lcd_wcmd(x|0xc0);	//y=1,第二行显示;y=0,第一行显示		0<=x<16
	else lcd_wcmd(x|0x80);	//数据指针=80+地址码(00H~27H,40H~67H)
		
}


/*--------------------------------------------------------------*/
//显示字符
/*
void printc(unsigned char CHAR)
{
	lcd_wdat(CHAR);
}
*/


/*--------------------------------------------------------------*/
//显示字符串
void prints(unsigned char *string)
{
	/*unsigned char i;
	//do{lcd_wdat(string[i]);i++;}while(string[i]!='\0');//生成代码比for循环大
	for(i=0;string[i]!='\0';i++)lcd_wdat(string[i]);	 //有时用for循环也好哈*/

	while(*string) {lcd_wdat(*string);string++;}

}

 


/*--------------------------------------------------------------*/
//LCD初始化设定
void lcd_init()
{						
	lcd_wcmd(0x38);		//设置LCD为16X2显示,5X7点阵,八位数据接口
	lcd_wcmd(0x06);		//LCD显示光标移动设置(光标地址指针加1,整屏显示不移动)
	lcd_wcmd(0x0c);		//LCD开显示及光标设置(光标不闪烁,不显示"_")
	lcd_wcmd(0x01);		//清除LCD的显示内容


	


}


/*--------------------------------------------------------------*/
//#endif
