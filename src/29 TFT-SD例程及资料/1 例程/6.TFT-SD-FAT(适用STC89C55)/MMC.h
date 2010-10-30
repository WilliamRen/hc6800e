#ifndef  __MMC_H__             //条件编译指令
#define  __MMC_H__             //避免重复调用

#define uchar unsigned char
#define uint  unsigned int
#define ulong  unsigned long

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
extern  uchar mmc_read_sector (ulong addr,uchar *Buffer);

#endif                      //条件编译结束

/*********************************************************/