/*******************************************************************
*
*  SD卡和MMC卡驱动程序
********************************************************************/

#include <reg52.h>
#include <intrins.h>


#define uchar unsigned char
#define uint  unsigned int
#define ulong  unsigned long

sbit SPI_DO =P1^0;
sbit SPI_SCL=P1^1;
sbit SPI_DI =P1^2;
sbit SPI_CS =P1^3;

#define INIT_CMD0_ERROR     0x01
#define INIT_CMD1_ERROR		0x02
#define WRITE_BLOCK_ERROR	0x03
#define READ_BLOCK_ERROR   	0x04 

bit  Init_Flag;    //Set it to 1 when Init is processing.
bit  card_type;  

unsigned char CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};//CMD0

/**********************************************************

 延时子函数

**********************************************************/
void delay(unsigned int time)
{
 while(time--);
}

/**********************************************************

 us延时子程序  (4.34us)

**********************************************************/
void delayNOP()
{
  _nop_();
  _nop_();
  _nop_();
  _nop_();
}

/************************************************************

 卡端口初始化

************************************************************/
void MMC_Port_Init()
{
   SPI_SCL=1;
   SPI_DO =1;
   SPI_CS=1;             
}

//****************************************************************************
//Routine for sending a byte to MMC/SD-Card
//write a byte

//****************************************************************************
void Write_Byte_MMC(uchar value)
{ 
   unsigned char i;

   for (i=0;i<8;i++)
   { 
     if (value&0x80)
	  SPI_DI=1;      //Send bit by bit(MSB First)
     else 
	  SPI_DI=0;

     SPI_SCL=0;         //Clock low
     if(Init_Flag)
	  delayNOP();
     SPI_SCL=1;        //Clock High
     if(Init_Flag)
	  delayNOP();
     value <<=1;
   }
}

//****************************************************************************
//Routine for reading a byte from MMC/SD-Card
  //Software SPI
//****************************************************************************
unsigned char Read_Byte_MMC()
{ 
   unsigned char temp=0;
   unsigned char i;
   SPI_DO=1;

   for (i=0;i<8;i++)    //MSB First
   {
     SPI_SCL=0;         //Clock Low
     if(Init_Flag)
	 delayNOP();   
     SPI_SCL=1;         //Clock High
     if(Init_Flag) 
	 delayNOP();	

	 temp=(temp<<1)|SPI_DO; 	  //read mmc data out pin 
   }
   return (temp);
}

//****************************************************************************
//Send a Command to MMC/SD-Card
//Return: the second byte of response register of MMC/SD-Card

//****************************************************************************
unsigned char Write_Command_MMC(unsigned char *CMD)
{
   unsigned char tmp;
   unsigned char i;

   //set MMC_Chip_Select to high (MMC/SD-Card disable) 
   SPI_CS=1;
   //send 8 Clock Impulse
   Write_Byte_MMC(0xFF);
   //set MMC_Chip_Select to low (MMC/SD-Card active)
   SPI_CS=0;

   //send 6 Byte Command to MMC/SD-Card
   for (i=0;i<0x06;i++) 
   { 
      Write_Byte_MMC(*CMD++);
   }
   i = 0;
   //get 16 bit response
   Read_Byte_MMC(); //read the first byte,ignore it. 
   do 
   {  //Only last 8 bit is used here.Read it out. 
      tmp = Read_Byte_MMC();
      i++;
   }
   while((tmp==0xff)&&(i<100)); 
   return(tmp);
}

/**********************************************************

 卡初始化子函数

**********************************************************/

unsigned char MMC_Init()
{  
   unsigned char temp;
   unsigned char i;

   MMC_Port_Init(); //Init SPI port  

   delay(200);
   
   Init_Flag=1; //Set the init flag

   for (i=0;i<0x0f;i++) 
   {
      Write_Byte_MMC(0xff); //send 74 clock at least!!!
   }
	
   //Send Command CMD0 to MMC/SD Card
   i = 0;
   do
   { //retry 200 times to send CMD0 command 
     temp=Write_Command_MMC(CMD);
     i++;
     if(i >= 200) 
     { //time out
       return(INIT_CMD0_ERROR);  //CMD0 Error!
     }
   } 
   while(temp!=1);
   
   i = 0;
   do{
       CMD[0] = 0x77;                   //CMD55命令
       temp = Write_Command_MMC(CMD);   //先发送 CMD55

       if(temp == 0x01)                 //如果有反应 
       {
         CMD[0] = 0x69;                 //ACMD41命令
         temp = Write_Command_MMC(CMD); //发送CMD41进行激活
         if(temp == 0x00)               //激活成功就是SD卡 
		 {
		  card_type =1;                 //SD
		  }
       }
       else                             //如果发送CMD55无反应，改发送CMD1 
       {
         CMD[0] = 0x41;                 //CMD1命令
         CMD[5] = 0xFF; 
         temp = Write_Command_MMC(CMD);    //发送CMD1进行激活
         if(temp == 0x00)               //激活成功就是MMC卡 
		 { 
		  card_type =0;                 //MMC
		  }
       }
       i++;
       if(i >= 200)
       {
         return(INIT_CMD1_ERROR);      //CMD1 Error!
       }
     }while(temp != 0x00);      // MMC和SD卡激活后的返回值均为0x00

   Init_Flag=0; //Init is completed,clear the flag 
   
   SPI_CS=1;  //set MMC_Chip_Select to high 

 //  wr_string(0,2,"  INIT SUCCESS  ");
   
   return(0); //All commands have been taken.
} 

/**********************************************************

 读SD卡寄存器子函数

**********************************************************/
unsigned char MMC_Read_Block(uchar *CMD,uchar *Buffer,uint Bytes)
{  
   unsigned int i; 
   unsigned  char temp;
    
   //Send Command CMD to MMC/SD-Card
   i = 0;
   do
   {  //Retry 100 times to send command.
      temp=Write_Command_MMC(CMD);
      i++;
      if(i == 100) 
      {
        return(READ_BLOCK_ERROR); //block write Error!
      }
   }
   while(temp!=0); 
   			
   //Read Start Byte form MMC/SD-Card (FEh/Start Byte)
   while (Read_Byte_MMC()!=0xfe) ;
	
   //Write blocks(normal 512Bytes) to MMC/SD-Card
   for (i=0;i<Bytes;i++)
   {
      *Buffer++ = Read_Byte_MMC();
   }
   
   Read_Byte_MMC();//CRC - Byte 
   Read_Byte_MMC();//CRC - Byte
	
   SPI_CS=1;
   return(0);
}

/**********************************************************

 读CSD寄存器子函数

**********************************************************/
unsigned char Read_CSD_MMC(unsigned char *Buffer)
{	   
   unsigned char temp;
   CMD[0]=0X49;
   CMD[5]=0XFF;

   temp=MMC_Read_Block(CMD,Buffer,16); //read 16 bytes
   return(temp);
}

/**********************************************************

 读CID寄存器子函数

**********************************************************/
unsigned char Read_CID_MMC(unsigned char *Buffer)
{
   unsigned char temp;
   CMD[0]=0X4A;
   CMD[5]=0XFF;

   temp=MMC_Read_Block(CMD,Buffer,16); //read 16 bytes
   return(temp);
}

/**********************************************************

 读扇区子函数

**********************************************************/
uchar mmc_read_sector (ulong addr,uchar *Buffer)
{	
	uchar cmd[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 

	addr = addr << 9; //addr = addr * 512

	cmd[1] = ((addr & 0xFF000000) >>24 );
	cmd[2] = ((addr & 0x00FF0000) >>16 );
	cmd[3] = ((addr & 0x0000FF00) >>8 );

    MMC_Read_Block(cmd,Buffer,512);

	return(0);
}

/**********************************************************/