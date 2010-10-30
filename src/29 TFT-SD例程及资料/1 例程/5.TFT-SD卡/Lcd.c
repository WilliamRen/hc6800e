/**********************************************************
*                   									  *
*				   彩屏驱动程序							  *
*														  *
**********************************************************/

#include <reg52.h>
#include "Lcd.h"
#include "char.h"
#include "chinese.h"


uint colors[]=
{
  0xf800,0x07e0,0x001f,0xffe0,0x0000,0x07ff,0xf81f,0xffff
};

/**********************************************************

 延时子函数

**********************************************************/
void delay_ms(uint ms)
{
  unsigned char k;

  while (ms--)
  {
    for (k = 0; k < 228; k++)
      ;
  }
}

/**********************************************************

 写命令与数据子函数

**********************************************************/
void LCD_Write(uchar type, uint value)  
{
	LCD_CS = 0;
	LCD_RS 	= type;    		// 0: command    1: data
	
	LCD_WR  = 0;
	DATA 	= (uchar)value;;
	LCD_WR 	= 1;
	LCD_CS = 1;
}

/**********************************************************

 写8位数据子函数

**********************************************************/
/*void LCD_Write_Data8(uchar value)    // color data
{
	LCD_CS = 0;
	LCD_RS 	= 1;
	LCD_WR  = 0;
	DATA 	= value;
	LCD_WR  = 1;
   	LCD_CS = 1;
}*/


/**********************************************************

 写16位数据子函数

**********************************************************/
void LCD_Wirte_Data16(uint value)    // color data
{
	LCD_CS = 0;
    LCD_RS 	= 1;
	LCD_WR  = 0;
	DATA 	= (uchar)value;
	LCD_WR  = 1;
	LCD_WR  = 0;
	DATA 	= (uchar)(value>>8)	;
	LCD_WR 	= 1;
	LCD_CS = 1;
}

/*********************************************************

 写寄存器子函数

**********************************************************/
void Reg_Write(uint reg,uint value) 
{
   LCD_Write(TYPE_LCD_COMMAND,reg);
   LCD_Write(TYPE_LCD_DATA,value);
}


/**********************************************************

设置显示窗口子函数

**********************************************************/
void LCD_SetRamAddr(uint xStart, uint xEnd, uint yStart, uint yEnd) 
{
   uint VerPos,HorPos,StartAddr;

   HorPos    = (uint)(xStart | (xEnd<<8));
   VerPos    = (uint)(yStart | (yEnd<<8));
   StartAddr = (uint)(xStart | (yStart<<8));



    Reg_Write(0x09, xStart);
    Reg_Write(0x10,yStart);
    Reg_Write(0x11,xEnd);
    Reg_Write(0x12,yEnd);

    Reg_Write(0x18, xStart); 
	Reg_Write(0x19, yStart); 
    LCD_Write(TYPE_LCD_COMMAND,0x22);		// 0x22
}


/**********************************************************/
void LCD_init(void)
{	uint num;
   	Reg_Write(0x0001,0x0002); 	//MODE_SEL1
	Reg_Write(0x0002,0x0012);   //MODE_SEL2
	Reg_Write(0x0003,0x0000);   //MODE_SEL3
	Reg_Write(0x0004,0x0010);	//MODE_SEL3
	LCD_SetRamAddr(0,127, 0,159); 
	for(num=20480;num>0;num--)
   	LCD_Wirte_Data16(0xffff);
   
	Reg_Write(0x0005,0x0008);   //VCO_MODE	
	Reg_Write(0x0007,0x007f);	//VCOMHT_CTRL
	Reg_Write(0x0008,0x0017);   //VCOMLT_CTRL    	
	Reg_Write(0x0009,0x0000);   //write SRAM window start X point
	Reg_Write(0x0010,0x0000);   //write SRAM window start y point
	Reg_Write(0x0011,0x0083);   //write SRAM window end x point
	Reg_Write(0x0012,0x009f);   //write SRAM window end y point
	Reg_Write(0x0017,0x0000);   //SRAM contrl
	Reg_Write(0x0018,0x0000);   //SRAM x position
	Reg_Write(0x0019,0x0000);   //SRAM y position
	Reg_Write(0x0006,0x00c5);   //DAC_OP_CTRL2
	delay_ms(10); //延时

}


/**********************************************************

 清屏子函数

**********************************************************/
void  LCD_clear(uchar n)
{
   uint num;
   
   LCD_SetRamAddr(0,127, 0,159);

   for(num=20480;num>0;num--)          //160*128=20480
   {
     LCD_Wirte_Data16(colors[n]);
   }
}


/**********************************************************

  字符显示子函数

**********************************************************/
void LCD_ShowChar(uchar x,uchar y,uint For_color,uint Bk_color, char ch)
{       
   uchar temp;
   uchar pos,t;
   uchar  CHAR_W,CHAR_H;
   
   CHAR_W = 8;         //8*16
   CHAR_H = 16;

   if(x>(LCD_SIZE_X-CHAR_W)||y>(LCD_SIZE_Y-CHAR_H))
   return;
   
   LCD_SetRamAddr(x, x+CHAR_W-1, y,y+CHAR_H-1);
   ch = ch-32;                       //按照ASCII编码顺序的到相应字母的编码

   for(pos=0;pos<CHAR_H;pos++)  
   {
      temp= Font8x16[ch][pos];
      
      for(t=0;t<CHAR_W;t++)
      {                 
	    if(temp&0x80)
	    LCD_Wirte_Data16(For_color);  //textcolor
	    else 
	    LCD_Wirte_Data16(Bk_color);  //backcolor
        temp<<=1; 
      }
	  
   }
}  

/**********************************************************
 显示字符串子函数

 x,y:起点坐标  
 *p:字符串起始地址

**********************************************************/
void LCD_ShowString(uchar x,uchar y,uint For_color,uint Bk_color,char *p)
{         
   while(*p!='\0')
   {       
     LCD_ShowChar(x,y,For_color,Bk_color,*p);
     x+=8;
     p++;
   }
}

/**********************************************************

  写汉字子函数

**********************************************************/
void ChineseChar(uint x,uint y,int size,uint For_color,uint Bk_color ,char c)
{
   int e=0,i,j;
   int  ed;
   
   uint  ncols;
   uint  nrows;
   uint  nbytes;

   uchar *pfont;
   uchar *pchar;
   uchar *fonttable[]={(uchar *)hz12,(uchar *)hz16,(uchar *)hz24};
   
   pfont=(uchar *)fonttable[size];

   if(size==0)
   {
     ncols =12-1;
     nrows =12-1;
     nbytes =24;	 //(12*12)/72
   }
   else
   if(size==1)
   {
     ncols =16-1;
     nrows =16-1;
     nbytes =32;  //(16*16)/72
   }
   else
   if(size==2)
   {
     ncols =24-1;
     nrows =24-1;
     nbytes =72;   //(24*24)/72
   }
   
   pchar=pfont + (nbytes*(c-1));

   LCD_SetRamAddr(x, x+nrows, y,y+ncols);
   
   for(i=0;i<nbytes;i++)
   {
     ed=*pchar++;
	 if(size==0)
	 {
       if(i%2==0)
       e=8;
       else
       e=4;
	 }
	 else
	 e=8;
     
     for(j=0;j<e;j++)
     if((ed>>j)&0x01)
     {
       LCD_Wirte_Data16(For_color);  //textcolor
	 }
     else
     {
       LCD_Wirte_Data16(Bk_color);  //backcolor
	 }
  }
}

/*********************************************************/

