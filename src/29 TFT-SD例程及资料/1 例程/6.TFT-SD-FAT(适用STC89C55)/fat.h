/***************************************************************/

#ifndef  __fat_H__             //条件编译指令
#define  __fat_H__             //避免重复调用

#include "mmc.h"

#define	FAT16	8
#define	FAT32	7
#define MBR_ERR		0x01
#define	DBR_ERR		0x02
#define SECTORS_ERR		0x30
#define CLUSTER_COUNT_ERR	0x31

#define  uchar   unsigned char
#define  uint    unsigned int
#define  uint8   unsigned char
#define  int8    signed char
#define  uint16  unsigned int
#define  int16   signed int
#define  uint32  unsigned long
#define  int32   signed long

extern  xdata  uchar buffer[];


extern  uint8   BPBSecPerClust;   // 每簇的扇区数
extern  uint16  FirstFATSector;   // 首FAT起始扇区
extern  uint16  FirstDirSector;   // 根目录起始扇区
extern  uint16  FirstDataSector;  // 数据区起始扇区
extern  uint32  FatClusSec;       // 簇号所在的扇区
extern  uint32  FirstDircluster;  // FAT32根目录首簇号
extern  uint8   DirSecitemCunt;   // FDT每扇区目录项基数 FAT16=8 FAT32=7


extern xdata  uint16 BPBbytesPerSec;		// 每扇区的字节数
extern xdata	uint16 BPBRevdFATSec;		// FAT1前的保留扇区数
extern xdata	uint8  ParNumberFATs;		// 该分区上的FAT份数
//	uint16 BPBRootDirCunt;			// 根目录项数
extern xdata	uint16 BPBTotSector16;		// 16位扇区数
extern xdata	uint16 BPBFATPerSec16;		// 16bit每FAT占用的扇区数
extern xdata	uint16 BPBhiddsector;		// 该分区系统隐藏扇区数
extern xdata	uint32 BPBTotSector32;		// 32位扇区数
extern xdata	uint32 DataClusterCount;	// 数据区簇数
extern xdata	uint32 Sector;				// 扇区变量

extern  uint8 FAT_init();

#endif                      //条件编译结束

/****************************************************************/