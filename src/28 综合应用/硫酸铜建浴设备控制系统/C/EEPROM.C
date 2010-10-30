/*******************************************************************************
*	此为STC89C52 的内部 EEPROM 读写操作										   *
*																			   *
*																			   *
********************************************************************************/
#include "system.h"

unsigned int W_buffer[10];
/*****************************************
STC单片机EEPROM操作
每个扇区512字节的空间, 只是用低128字节
58系列起始地址 EEPROM 0x8000	单片机有58个扇区
52系列起始地址 EEPROM 0X2000
******************************************/
#ifndef 	__DATAFLASH_C__
#define		__DATAFLASH_C__
#endif


//命令
#define READ_BYTE		0x01
#define WRITE_BYTE		0x02
#define EARSE_SECTOR	0x03
#define WAIT_TIME		0x01
//#define MAX_BYTE_USED	        512  //每扇区最大存储空间

#define DEBUG_STC89C_LE52RC

#ifdef DEBUG_STC89C_LE58RD                        //STC89C58RD+,  89LE58RD+
	#define EEPROM_start       0x8000 //58单片机的eepeom的起始地址
#endif
#ifdef DEBUG_STC89C_LE52RC                        //STC89C52RC,        89LE52RC
	#define EEPROM_start       0x2000 //58单片机的eepeom的起始地址
#endif
#ifdef DEBUG_STC89C_LE54RC                        //STC89C54RC,        89LE54RC
	#define EEPROM_start       0x8000 //58单片机的eepeom的起始地址
#endif
 

/************************/
sfr ISP_DATA    = 0xe2;
sfr ISP_ADDR_H 	= 0xe3;
sfr ISP_ADDR_L 	= 0xe4;
sfr ISP_CMD  	= 0xe5;
sfr ISP_TRIG	= 0xe6;
sfr ISP_CONTR	= 0xe7;
/************************/
Uint8 rdByte(Uint16 addr)
{
	  //addr = addr+EEPROM_start;
		ISP_ADDR_H = (Uint8)(addr>>8);
		ISP_ADDR_L = (Uint8)(addr&0x00ff);
		ISP_CMD  = ISP_CMD & 0xf8;   // 1111,1000 
		ISP_CMD = ISP_CMD | READ_BYTE;

		ISP_CONTR = ISP_CONTR & 0x18;        // 0001,1000 
		ISP_CONTR = (ISP_CONTR | WAIT_TIME );
		ISP_CONTR = ISP_CONTR | 0x80; 

		ISP_TRIG = 0x46;
		ISP_TRIG = 0xb9;
		_nop_();_nop_();_nop_();
		ISP_CONTR &= 0x7f;
		ISP_TRIG = 0x00;
		return(ISP_DATA);
}		

/*addr-字节的地址，buffer[]-读的数据所在的数组，len-读的数据的长度*/			
void rdData(unsigned int addr, unsigned int len)
{
		Uint16 i,temp;
		EA = 0;
		addr = addr+EEPROM_start;
		for(i=0; i<len; i++)
		{
				D[i] = rdByte(addr+(i*2));
				D[i] <<=8;
				temp=rdByte(addr+(i*2)+1);
				D[i] = D[i] | (unsigned int)temp;
		}
		EA = 1;
}				

void earseSector(unsigned int addr)
{
	  EA = 0;
		addr = addr+EEPROM_start;
		addr &= 0xfe00;										/*块的起始地址*/
		ISP_ADDR_H = (Uint8)(addr>>8);
		ISP_ADDR_L = (Uint8)(addr&0x00ff);

		ISP_CMD = ISP_CMD & 0xf8;       // 1111,1000
		ISP_CMD = ISP_CMD |EARSE_SECTOR;


		ISP_CONTR = ISP_CONTR & 0x18;        // 0001,1000 
		ISP_CONTR = (ISP_CONTR | WAIT_TIME );
		ISP_CONTR = ISP_CONTR | 0x80; 

		ISP_TRIG = 0x46;
		ISP_TRIG = 0xb9;
		_nop_();_nop_();_nop_();

		ISP_CONTR &= 0x7f;
		ISP_TRIG = 0x00;
		EA = 1;
}	
	
void wrByte(Uint16 addr, Uint8 byte)
{		
		ISP_ADDR_H = (Uint8)(addr>>8);
		ISP_ADDR_L = (Uint8)(addr&0x00ff);

		ISP_CMD  = ISP_CMD & 0xf8;         // 1111,1000 
		ISP_CMD = ISP_CMD |WRITE_BYTE;
		ISP_DATA =  byte;

		ISP_CONTR = ISP_CONTR & 0x18;        // 0001,1000 
		ISP_CONTR = (ISP_CONTR | WAIT_TIME );
		ISP_CONTR = ISP_CONTR | 0x80; 

		ISP_TRIG = 0x46;
		ISP_TRIG = 0xb9;
		_nop_(); _nop_(); _nop_();

		ISP_CONTR &= 0x7f;
		ISP_TRIG = 0x00;
}	

/*addr-字节的地址，buffer[]-要写的数据的数组，len-写的数据的长度*/	
void wrData(unsigned int addr,  unsigned int len)
{
		Uint8 i,temp;
		EA = 0;
		addr = addr+EEPROM_start;
		for(i=0; i<len; i++)
		{
				
				temp = (Uint8)W_buffer[i]>>8;
				wrByte(addr+(i*2), temp);			/*写入整个数组*/

				W_buffer[i] = W_buffer[i] & 0x00ff;
				wrByte(addr+(i*2+1), (Uint8)W_buffer[i]);

		}		
		EA = 1;
}






	 