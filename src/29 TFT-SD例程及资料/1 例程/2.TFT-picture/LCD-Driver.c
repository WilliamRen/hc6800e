
/**********************************************************
*                   									  *
*				   彩屏驱动程序							  *
*														  *
**********************************************************/

#include <reg52.h>
#include <intrins.h>
#include "LCD.h"
#include "picture.h"

#define  NOP()   _nop_()   /* 定义空指令 */

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
	DATA 	= (uchar)value;
	LCD_WR 	= 1;
	LCD_CS = 1;
}

/**********************************************************

 写8位数据子函数

**********************************************************/
void LCD_Write_Data8(uchar value)    // color data
{
	LCD_CS = 0;
	LCD_RS 	= 1;
	LCD_WR  = 0;
	DATA 	= value;
	LCD_WR  = 1;
   	LCD_CS = 1;
}



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




/**********************************************************
*														  *
*       液晶屏初始化									  *
*														  *
************************************************************/
void LCD_init(void)
{   uint num;
   
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

 图片显示子函数

**********************************************************/
void  pic_play(uint Start_X, uint End_X,uint Start_Y,uint End_Y)
{
   uint num,m;
   uint dx,dy;

   dx = (End_X+1)-Start_X;                  //计算写入数据的总数
   dy = (End_Y+1)-Start_Y;
   num = dx*dy<<1;

   LCD_CS = 0;
   LCD_SetRamAddr(Start_X,End_X-1,Start_Y,End_Y-1) ;
   LCD_RS 	= 1; 


   for(m=0; m<7128; m++)	
   {		
      LCD_Write_Data8(Image_pic[m]); 

   }
   LCD_CS = 1;

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

