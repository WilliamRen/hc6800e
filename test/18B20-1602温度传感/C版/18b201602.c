#include < reg51.h >
#include < intrins.h >

#define uchar unsigned char
#define uint  unsigned int

sbit DQ = P3^7 ;  //定义DS18B20端口DQ  
sbit BEEP=P1^5  ; //蜂鸣器驱动线

bit presence  ;

sbit LCD_RS = P2^6 ;             
sbit LCD_RW = P2^5 ;
sbit LCD_EN = P2^7 ;

uchar code  cdis1[ ] = {" REN WEN LONG   "} ;
uchar code  cdis2[ ] = {" WENDU:    .  C "} ;
uchar code  cdis3[ ] = {" DS18B20  ERR0R "} ;
uchar code  cdis4[ ] = {"  PLEASE CHECK  "} ;

unsigned char data  temp_data[2] = {0x00,0x00} ;
unsigned char data  display[5]   = {0x00,0x00,0x00,0x00,0x00} ;
unsigned char code  ditab[16]    = {0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,
                                    0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09} ;
void beep() ; 
unsigned char code  mytab[8]     = {0x0C,0x12,0x12,0x0C,0x00,0x00,0x00,0x00} ;

#define delayNOP() ; {_nop_() ;_nop_() ;_nop_() ;_nop_() ;} ;

/*******************************************************************/
void delay1(int ms)
{
 unsigned char y ;
 while(ms--)
 {
  for(y = 0 ; y<250 ; y++)
  {
   _nop_() ;
   _nop_() ;
   _nop_() ;
   _nop_() ;
  }
 }
}

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
    delay1(15) ;   
    lcd_wcmd(0x01) ;      //清除LCD的显示内容            
    lcd_wcmd(0x38) ;      //16*2显示，5*7点阵，8位数据
    delay1(5) ;
    lcd_wcmd(0x38) ;         
    delay1(5) ;
    lcd_wcmd(0x38) ;         
    delay1(5) ;

    lcd_wcmd(0x0c) ;      //显示开，关光标
    delay1(5) ;
    lcd_wcmd(0x06) ;      //移动光标
    delay1(5) ;
    lcd_wcmd(0x01) ;      //清除LCD的显示内容
    delay1(5) ;
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

/*us级延时函数                                        */
/*******************************************************************/

void Delay(unsigned int num)
{
  while( --num ) ;
}

/*初始化ds1820                                      */
/*******************************************************************/
Init_DS18B20(void)
{  
     DQ = 1 ;      //DQ复位
     Delay(8) ;    //稍做延时

     DQ = 0 ;      //单片机将DQ拉低
     Delay(90) ;   //精确延时 大于 480us

     DQ = 1 ;       //拉高总线
     Delay(8) ;

     presence = DQ ;    //如果=0则初始化成功 =1则初始化失败
     Delay(100) ;
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
    Delay(4) ;
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
    Delay(5) ;

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

     temp_data[0] = ReadOneChar() ;   //温度低8位
     temp_data[1] = ReadOneChar() ;   //温度高8位 
}

/* 数据转换与温度显示                                              */
/*******************************************************************/
Disp_Temperature()
{
  display[4]=temp_data[0]&0x0f ;
  display[0]=ditab[display[4]]+0x30 ;     //查表得小数位的值
  
  display[4]=((temp_data[0]&0xf0)>>4)|((temp_data[1]&0x0f)<<4) ;
  display[3]=display[4]/100+0x30 ;
  display[1]=display[4]%100 ;
  display[2]=display[1]/10+0x30 ;
  display[1]=display[1]%10+0x30 ;

  if(display[3]==0x30)        //高位为0，不显示
  { 
     display[3]=0x20 ;              
     if(display[2]==0x30)      //次高位为0，不显示
     display[2]=0x20 ;
  }

  lcd_pos(0x48) ;             
  lcd_wdat(display[3]) ;        //百位数显示 
  lcd_pos(0x49) ;             
  lcd_wdat(display[2]) ;        //十位数显示 
  lcd_pos(0x4a) ;             
  lcd_wdat(display[1]) ;        //个位数显示 
  lcd_pos(0x4c) ;             
  lcd_wdat(display[0]) ;        //小数位数显示 
}  

/*******************************************************************/
/* 蜂鸣器响一声                 */
/*******************************************************************/
void beep()			        
{
    unsigned char y ;
    for (y=0 ;y<100 ;y++)
    {
      Delay(60) ;
      BEEP=!BEEP ;                //BEEP取反
    } 
    BEEP=1 ;                      //关闭蜂鸣器
    Delay(40000) ;
}

/* DS18B20 OK 显示菜单                                             */
/*******************************************************************/
void  Ok_Menu ()
{ 
    uchar  m ;
    lcd_init() ;                //初始化LCD 
            
    lcd_pos(0) ;                //设置显示位置为第一行的第1个字符
    m = 0 ;
    while(cdis1[m] != '\0')
    {                         //显示字符
       lcd_wdat(cdis1[m]) ;
       m++ ;
    }

    lcd_pos(0x40) ;             //设置显示位置为第二行第1个字符
    m = 0 ;
    while(cdis2[m] != '\0')
    {
       lcd_wdat(cdis2[m]) ;      //显示字符
       m++ ;
    }

    writetab() ;               //自定义字符写入CGRAM
    delay1(5) ;
    lcd_pos(0x4d) ;
    lcd_wdat(0x00) ;          //显示自定义字符
}


/* DS18B20 ERROR 显示菜单                             */
/*******************************************************************/
void  Error_Menu ()
{
    uchar  m ;
    lcd_init() ;                //初始化LCD 
 
    lcd_pos(0) ;                //设置显示位置为第一行的第1个字符
    m = 0 ;
    while(cdis3[m] != '\0')
    {                         //显示字符
       lcd_wdat(cdis3[m]) ;
       m++ ;
    }

    lcd_pos(0x40) ;             //设置显示位置为第二行第1个字符
    m = 0 ;
    while(cdis4[m] != '\0')
    {
       lcd_wdat(cdis4[m]) ;      //显示字符
       m++ ;
    }
}


/* 主函数     */
/************************************/
void main()
{
 Ok_Menu () ;

 do
 {
   Read_Temperature() ;
   Disp_Temperature() ;
 }
 while(!presence)  ;

 Error_Menu () ;

 do
 {  
   Init_DS18B20() ;
   beep() ;
 }
 while(presence) ;
}

