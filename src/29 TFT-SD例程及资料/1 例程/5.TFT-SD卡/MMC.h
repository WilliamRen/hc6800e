
#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int
#define ulong  unsigned long

sbit SPI_DO =P1^0;
sbit SPI_CLK=P1^1;
sbit SPI_DI =P1^2;
sbit SPI_CS =P1^3;

// Error define

#define INIT_CMD0_ERROR     0x01
#define INIT_CMD1_ERROR		0x02
#define WRITE_BLOCK_ERROR	0x03
#define READ_BLOCK_ERROR   	0x04 



extern bit  Init_Flag;    
extern bit  card_type;   


extern  void delay(unsigned int time);
extern  void delayNOP();
extern  void MMC_Port_Init();
extern  void Write_Byte_MMC(uchar value);
extern  unsigned char Read_Byte_MMC();
extern  unsigned char Write_Command_MMC(unsigned char *CMD);
extern  unsigned char MMC_Init();
extern  unsigned char MMC_Read_Block(uchar *CMD,uchar *Buffer,uint Bytes);
extern  unsigned char Read_CSD_MMC(unsigned char *Buffer);
extern  unsigned char Read_CID_MMC(unsigned char *Buffer);
