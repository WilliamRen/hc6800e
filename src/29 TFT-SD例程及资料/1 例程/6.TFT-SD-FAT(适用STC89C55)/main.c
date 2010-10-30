/*******************************************************************
*                                                                  *
*                  SD卡配置文件系统                                *
*                                                                  *
* 实验目的：1.掌握SD卡的驱动程序                                   *
            2. 组成简单的文件系统                                  *
*实验现象： SD卡的属性参数                                         *
*******************************************************************/

#include <reg52.h>

#include "fat.h"
#include "lcd.h"

unsigned char  data_temp[16];

char code reserve[3]_at_ 0x3b; //保留0x3b开始的3个字节

/**********************************************************

 卡信息显示子函数

**********************************************************/
void   card_info(void)
{
   unsigned char  by;
   unsigned int   c_size, c_size_mult, read_bl_len;
   unsigned long  drive_size;
   unsigned long  size_mb;

   LCD_ShowString(12,40,colors[0],colors[7],"INIT SUCCESS");
   LCD_ShowString(2,60,colors[0],colors[7],"TYPE:");
   if(card_type)
   {LCD_ShowString(52,60,colors[2],colors[7],"SD");
    ChineseChar(68,60,1,colors[2],colors[7],1);	//卡
   }
   else
   { LCD_ShowString(52,60,colors[2],colors[7],"MMC");
     ChineseChar(76,60,1,colors[2],colors[7],1);	//卡
   }
   
   Read_CID_MMC(data_temp);

   LCD_ShowString(2,76,colors[0],colors[7],"NAME:");
   LCD_ShowChar(50,76,colors[2],colors[7],data_temp[3]);
   LCD_ShowChar(58,76,colors[2],colors[7],data_temp[4]);
   LCD_ShowChar(66,76,colors[2],colors[7],data_temp[5]);
   LCD_ShowChar(74,76,colors[2],colors[7],data_temp[6]);
   LCD_ShowChar(82,76,colors[2],colors[7],data_temp[7]);
     
   Read_CSD_MMC(data_temp);

   c_size=data_temp[6] & 0x03; //bits 1..0
   c_size<<=10;
   c_size+=(unsigned int)data_temp[7]<<2;
   c_size+=data_temp[8]>>6;

   by= data_temp[5] & 0x0F;
   read_bl_len=1;
   read_bl_len<<=by;	        //2^by
   by=data_temp[9] & 0x03;      //获取multiply
   by<<=1;
   by+=data_temp[10] >> 7;

   c_size_mult=1;
   c_size_mult<<=(2+by);	  //2^(c_size_mult+2)

   drive_size=(ulong)(c_size+1) * (ulong)c_size_mult * (ulong)read_bl_len;

   size_mb= drive_size>>20;    //1024*1024=1048576=2^20; 
   LCD_ShowString(2,92,colors[0],colors[7],"SIZE:");
   LCD_ShowChar(52,92,colors[2],colors[7],size_mb/1000|0x30); 
   LCD_ShowChar(60,92,colors[2],colors[7],size_mb%1000/100|0x30);
   LCD_ShowChar(68,92,colors[2],colors[7],size_mb%100/10|0x30); 
   LCD_ShowChar(76,92,colors[2],colors[7],size_mb%10|0x30); 
   LCD_ShowChar(84,92,colors[2],colors[7],'M');
   LCD_ShowChar(92,92,colors[2],colors[7],'B');
}

/**********************************************************

 数据转换子函数1

**********************************************************/
void  data_change1(uint number)
{
   data_temp[0] = (number/10000)|0x30;
   data_temp[1] = (number%10000/1000)|0x30;
   data_temp[2] = (number%1000/100)|0x30;
   data_temp[3] = (number%100/10)|0x30;
   data_temp[4] = (number%10)|0x30;
   
   if(data_temp[0] == 0x30)
   {
     data_temp[0] = 0x20;
     if(data_temp[1] == 0x30)
     data_temp[1] = 0x20;
     if(data_temp[2] == 0x30)
     data_temp[2] = 0x20;
   }
}

/**********************************************************

 数据转换子函数2

**********************************************************/
void  data_change2(uint number)
{
   data_temp[0] = (number/100)|0x30;
   data_temp[1] = (number%100/10)|0x30;
   data_temp[2] = (number%10)|0x30;
   
   if(data_temp[0] == 0x30)
   {
     data_temp[0] = 0x20;
     if(data_temp[1] == 0x30)
     data_temp[1] = 0x20;
   }
}
/**********************************************************

 首FAT起始扇区显示子函数

**********************************************************/
/*
void  F_FATSector(void)
{
   data_change1(FirstFATSector);

   LCD_ShowString(2,124,colors[0],"FirstFATSector:");

   LCD_ShowChar( 2,140,colors[2],data_temp[0]);
   LCD_ShowChar(10,140,colors[2],data_temp[1]);
   LCD_ShowChar(18,140,colors[2],data_temp[2]);
   LCD_ShowChar(26,140,colors[2],data_temp[3]);
   LCD_ShowChar(34,140,colors[2],data_temp[4]);
}
*/
/**********************************************************

 根目录起始扇区显示子函数

**********************************************************/
/*
void  F_DirSector(void)
{
   data_change1(FirstDirSector);
   
   LCD_ShowString(2,124,colors[0],"FirstDirSector:");

   LCD_ShowChar( 2,140,colors[2],data_temp[0]);
   LCD_ShowChar(10,140,colors[2],data_temp[1]);
   LCD_ShowChar(18,140,colors[2],data_temp[2]);
   LCD_ShowChar(26,140,colors[2],data_temp[3]);
   LCD_ShowChar(34,140,colors[2],data_temp[4]);
}
*/
/**********************************************************

 数据区起始扇区显示子函数

**********************************************************/
/*
void  F_DataSector(void)
{
   data_change1(FirstDataSector);
   
   LCD_ShowString(2,124,colors[0],"FirstDataSector");

   LCD_ShowChar( 2,140,colors[2],data_temp[0]);
   LCD_ShowChar(10,140,colors[2],data_temp[1]);
   LCD_ShowChar(18,140,colors[2],data_temp[2]);
   LCD_ShowChar(26,140,colors[2],data_temp[3]);
   LCD_ShowChar(34,140,colors[2],data_temp[4]);
}
*/
/**********************************************************

  BPB信息显示子函数

**********************************************************/
void  BPB_INFO(void)
{
   data_change2(BPBbytesPerSec);  //每个扇区的字节数
   LCD_ShowString(2,24,colors[0],colors[7],"BPBbytesPerSec");
   LCD_ShowChar( 2,40,colors[2],colors[7],data_temp[0]);
   LCD_ShowChar(10,40,colors[2],colors[7],data_temp[1]);
   LCD_ShowChar(18,40,colors[2],colors[7],data_temp[2]);

   data_change2(BPBSecPerClust);  //每个簇的扇区数
   LCD_ShowString(2,56,colors[0],colors[7],"BPBSecPerClust");
   LCD_ShowChar( 2,72,colors[2],colors[7],data_temp[0]);
   LCD_ShowChar(10,72,colors[2],colors[7],data_temp[1]);
   LCD_ShowChar(18,72,colors[2],colors[7],data_temp[2]);

   data_change2(BPBRevdFATSec);	  //保留扇区数
   LCD_ShowString(2,88,colors[0],colors[7],"BPBRevdFATSec");
   LCD_ShowChar( 2,104,colors[2],colors[7],data_temp[0]);
   LCD_ShowChar(10,104,colors[2],colors[7],data_temp[1]);
   LCD_ShowChar(18,104,colors[2],colors[7],data_temp[2]);

/* data_change2(BPBhiddsector);	  //隐藏扇区数
   LCD_ShowString(2,120,colors[0],"BPBhiddsector");
   LCD_ShowChar( 2,136,colors[2],data_temp[0]);
   LCD_ShowChar(10,136,colors[2],data_temp[1]);
   LCD_ShowChar(18,136,colors[2],data_temp[2]);	*/

   data_change1(BPBFATPerSec16);  //每个FAT的扇区数
   LCD_ShowString(2,120,colors[0],colors[7],"BPBFATPerSec");
   LCD_ShowChar( 2,136,colors[2],colors[7],data_temp[0]);
   LCD_ShowChar(10,136,colors[2],colors[7],data_temp[1]);
   LCD_ShowChar(18,136,colors[2],colors[7],data_temp[2]);
   LCD_ShowChar(26,136,colors[2],colors[7],data_temp[3]);
   LCD_ShowChar(34,136,colors[2],colors[7],data_temp[4]);
}

/**********************************************************

  主函数

**********************************************************/
void main(void)
{  
   P2 = 0xff;
   P0 = 0xff;

   LCD_init();                 //液晶初始化
 //  LCD_color_H();              //水平彩条显示
//   delay_ms(2000);
   LCD_clear(7);
   LCD_ShowString(16,4,colors[4],colors[7],"SD&MMC Card");
   LCD_ShowString(12,20,colors[4],colors[7],"Test Process");

   MMC_Init();                 //卡初始化
   FAT_init();                  //FAT初始化

   if(Init_Flag == 0)          //卡初始化成功
   {
     card_info();              //卡信息显示
   }
  
   ChineseChar(14,120,1,colors[2],colors[7],2);	//文
   ChineseChar(38,120,1,colors[2],colors[7],3);	//件
   ChineseChar(62,120,1,colors[2],colors[7],4);	//系
   ChineseChar(86,120,1,colors[2],colors[7],5);	//统
 
   delay_ms(3000);
   LCD_clear(7);
   
   if(DirSecitemCunt == 8)
   {
     LCD_ShowString(2,4,colors[0],colors[7],"FORMAT:");
     LCD_ShowString(66,4,colors[2],colors[7],"FAT16");
   }
   else
   if(DirSecitemCunt == 7)
   {
     LCD_ShowString(2,4,colors[0],colors[7],"FORMAT:");
     LCD_ShowString(66,4,colors[2],colors[7],"FAT32");
   }
   
   BPB_INFO();                 //BPB信息显示

   while(1);
}

/*********************************************************/
