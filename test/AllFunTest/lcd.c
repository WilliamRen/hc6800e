/*******************************************************************/
/*  LCD1602显示控制                                                */
/*  连接方法：JP10 （P0）与LCD1602接口连接                         */
/*  用573锁存器和138译码器控制和数码管                             */
/*******************************************************************/
#include "common.h"

uchar code cdis1[ ] = {" REN WEN LONG   "} ;
uchar code cdis2[ ] = {" WENDU:    .  C "} ;
uchar code cdis3[ ] = {" DS18B20  ERR0R "} ;
uchar code cdis4[ ] = {"  PLEASE CHECK  "} ;
uchar code cdis5[ ] = {" REN WEN LONG   "} ;
uchar code cdis6[ ] = {" IR CODE: --H "} ;
uchar code cdis7[ ] = {"Welcome To Here!"} ;

unsigned char display[5]        = {0x00,0x00,0x00,0x00,0x00} ;
unsigned char code mytab[8]     = {0x0C,0x12,0x12,0x0C,0x00,0x00,0x00,0x00} ;

/******************************************************************/
/*检查LCD忙状态                                                   */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。   */
/******************************************************************/ 
bit lcd_busy()
{                          
    bit result ;
    LCD_RS = 0 ;
    LCD_RW = 1 ;
    LCD_EN = 1 ;
    delayNOP() ;
    result = (bit)(P0&0x80) ;
    LCD_EN = 0 ;
    return(result) ; 
}
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。          */
/*******************************************************************/
void lcd_wcmd(uchar cmd)
{                          
   while(lcd_busy()) ;
    LCD_RS = 0 ;
    LCD_RW = 0 ;
    LCD_EN = 0 ;
    _nop_() ;
    _nop_() ; 
    P0 = cmd ;
    delayNOP() ;
    LCD_EN = 1 ;
    delayNOP() ;
    LCD_EN = 0 ;  
}
/*******************************************************************/
/*写显示数据到LCD                            */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。       */
/*******************************************************************/
void lcd_wdat(uchar dat)
{                          
   while(lcd_busy()) ;
    LCD_RS = 1 ;
    LCD_RW = 0 ;
    LCD_EN = 0 ;
    P0 = dat ;
    delayNOP() ;
    LCD_EN = 1 ;
    delayNOP() ;
    LCD_EN = 0 ; 
}

/*  LCD初始化设定                                                  */
/*******************************************************************/
void lcd_init()
{ 
    delay_ms(15) ;   
    lcd_wcmd(0x01) ;      //清除LCD的显示内容            
    lcd_wcmd(0x38) ;      //16*2显示，5*7点阵，8位数据
    delay_ms(5) ;
    lcd_wcmd(0x38) ;         
    delay_ms(5) ;
    lcd_wcmd(0x38) ;         
    delay_ms(5) ;
    lcd_wcmd(0x0c) ;      //显示开，关光标
    delay_ms(5) ;
    lcd_wcmd(0x06) ;      //移动光标
    delay_ms(5) ;
    lcd_wcmd(0x01) ;      //清除LCD的显示内容
    delay_ms(5) ;
}
/*  设定显示位置                                    */
/*******************************************************************/
void lcd_pos(uchar pos)
{                          
  lcd_wcmd(pos | 0x80) ;  //数据指针=80+地址变量
}
/*自定义字符写入CGRAM                                   */
/*******************************************************************/
void  writetab()  
{  
    unsigned char i ;
    lcd_wcmd(0x40) ;            //写CGRAM
    for (i = 0 ; i< 8 ; i++)       
    lcd_wdat(mytab[ i ]) ;        
}


//通用程序向共阴极数码管写入数据
void lcd_display(unsigned char value, unsigned char pos, unsigned char is_decimal )
{ 
	if ( is_decimal & 0xFF )
	{
		display[1] = (value/10) + 0x30;	 //十位
		display[0] = (value%10) + 0x30;	 //个位
	}
	else
	{
		display[1] = (value>>4) & 0x0F;
		if (display[1]>9)
		  display[1] = display[1] + 0x37;
		else
		  display[1] = display[1] + 0x30;
		display[0] = value & 0x0F;
		if (display[0]>9)
		  display[0] = display[0] + 0x37;
		else
		  display[0] = display[0] + 0x30;
	}

    /*if(display[1]==0x30)        //高位为0，不显示
	{ 
		display[1] = 0x20 ;              
	}*/
	lcd_pos( (pos&0xFF) );
	lcd_wdat(display[1]);//高八位
	lcd_wdat(display[0]);//低八位
}

void lcd_temp_menu(void)
{
    uchar  m ;
    lcd_init() ;                //初始化LCD 
            
    lcd_pos(0) ;                //设置显示位置为第一行的第1个字符
    m = 0 ;
    while(cdis1[m] != '\0')
    {                           //显示字符
       lcd_wdat(cdis1[m]) ;
       m++ ;
    }
    lcd_pos(0x40) ;             //设置显示位置为第二行第1个字符
    m = 0 ;
    while(cdis2[m] != '\0')
    {
       lcd_wdat(cdis2[m]) ;     //显示字符
       m++ ;
    }
    writetab() ;                //自定义字符写入CGRAM
    delay_ms(5) ;
    lcd_pos(0x4d) ;
    lcd_wdat(0x00) ;            //显示自定义字符
}

void lcd_ir_menu(void)
{
    uchar  m ;
    lcd_init() ;                //初始化LCD 
            
    lcd_pos(0) ;                //设置显示位置为第一行的第1个字符
    m = 0 ;
    while(cdis5[m] != '\0')
    {                           //显示字符
       lcd_wdat(cdis5[m]) ;
       m++ ;
    }
    lcd_pos(0x40) ;             //设置显示位置为第二行第1个字符
    m = 0 ;
    while(cdis6[m] != '\0')
    {
       lcd_wdat(cdis6[m]) ;     //显示字符
       m++ ;
    }
}

void lcd_welcome_menu(void)
{
    uchar  m ;
    lcd_init() ;                //初始化LCD 
    lcd_pos(0) ;                //设置显示位置为第一行的第1个字符
    m = 0 ;
    while(cdis5[m] != '\0')
    {                           //显示字符
       lcd_wdat(cdis5[m]) ;
       m++ ;
    }
    lcd_pos(0x40) ;             //设置显示位置为第二行第1个字符
    m = 0 ;
    while(cdis7[m] != '\0')
    {
       lcd_wdat(cdis7[m]) ;     //显示字符
       m++ ;
    }
}

/* 主函数     */
/************************************/
void test_1602()
{
	lcd_ir_menu();
	while(is_quit())
	{
		if ( ir_key_last != ir_key_value )
		{
			lcd_display( ir_key_value, 0x4A, 0x00 );
			ir_key_last = ir_key_value ;
		}
		else
		{
			delay_ms(10);
		}
	}
}
