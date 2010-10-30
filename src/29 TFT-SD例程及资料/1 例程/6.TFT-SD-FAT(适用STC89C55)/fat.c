
#include "fat.h"


xdata  uchar buffer[512];

uint8   BPBSecPerClust;   // 每簇的扇区数
uint16  FirstFATSector;   // 首FAT起始扇区
uint16  FirstDirSector;   // 根目录起始扇区
uint16  FirstDataSector;  // 数据区起始扇区
uint32  FatClusSec;       // 簇号所在的扇区
uint32  FirstDircluster;  // FAT32根目录首簇号
uint8   DirSecitemCunt;   // FDT每扇区目录项基数 FAT16=8 FAT32=7

xdata   uint16 BPBbytesPerSec;		// 每扇区的字节数
xdata	uint16 BPBRevdFATSec;		// FAT1前的保留扇区数
xdata	uint8  ParNumberFATs;		// 该分区上的FAT份数
//	uint16 BPBRootDirCunt;			// 根目录项数
xdata	uint16 BPBTotSector16;		// 16位扇区数
xdata	uint16 BPBFATPerSec16;		// 16bit每FAT占用的扇区数
xdata	uint16 BPBhiddsector;		// 该分区系统隐藏扇区数
xdata	uint32 BPBTotSector32;		// 32位扇区数
xdata	uint32 DataClusterCount;	// 数据区簇数
xdata	uint32 Sector;				// 扇区变量

uint8 FAT_init()
{
/*
xdata  uint16 BPBbytesPerSec;		// 每扇区的字节数
xdata	uint16 BPBRevdFATSec;		// FAT1前的保留扇区数
xdata	uint8  ParNumberFATs;		// 该分区上的FAT份数
//	uint16 BPBRootDirCunt;			// 根目录项数
xdata	uint16 BPBTotSector16;		// 16位扇区数
xdata	uint16 BPBFATPerSec16;		// 16bit每FAT占用的扇区数
xdata	uint16 BPBhiddsector;		// 该分区系统隐藏扇区数
xdata	uint32 BPBTotSector32;		// 32位扇区数
xdata	uint32 DataClusterCount;	// 数据区簇数
xdata	uint32 Sector;				// 扇区变量
*/
	mmc_read_sector(0,buffer);		// 读SD卡0扇区

	if((0xEB != buffer[0])&&(0xE9 != buffer[0]))			// 如果0扇区不是DBR
	{
		if((0x55 != buffer[510])||(0xAA != buffer[511]))
		 return MBR_ERR;	// MBR错误！

		Sector = ((uint32)buffer[454])|			// 载入MBR里DPT中的DBR偏移量,只用低2字节
				 ((uint32)buffer[455]<<8);
		mmc_read_sector(Sector,buffer);			// 读DBR所在扇区

		if((0xEB != buffer[0])&&(0xE9 != buffer[0]))
			return DBR_ERR;					// 如果不是DBR则报错：磁盘未格式化！
	}
//-----------------------------------------------------------------

	BPBbytesPerSec = ((uint16)buffer[11])|		// 每扇区字节数，通常为512
					 ((uint16)buffer[12]<<8);

	BPBSecPerClust = buffer[13];				// 每簇扇区数

	BPBRevdFATSec  = ((uint16)buffer[14])|		// FAT1前的保留扇区数
					 ((uint16)buffer[15]<<8);

	ParNumberFATs  = buffer[16];				// 该分区上的FAT份数，通常为2

//	BPBRootDirCunt = ((uint16)buffer[17])|		// 根目录项数,如果为0则可能是FAT32系统
//					 ((uint16)buffer[18]<<8);

    BPBTotSector16 = ((uint16)buffer[19])|		// 16位扇区数
                     ((uint16)buffer[20]<<8);

	BPBFATPerSec16 = ((uint16)buffer[22])|		// 每FAT占用的扇区数,16bit
					 ((uint16)buffer[23]<<8);

	BPBhiddsector  = ((uint16)buffer[28])|		// 系统隐藏扇区数,只用低2字节
					 ((uint16)buffer[29]<<8);

    BPBTotSector32 = ((uint32)buffer[32])|		// 32位扇区数
                     ((uint32)buffer[33]<<8)|
                     ((uint32)buffer[34]<<16)|
                     ((uint32)buffer[35]<<24);

//-------------------------------------_-------------------------------
    // 获得逻辑总扇区数
    if(BPBTotSector32 == 0)
	{
        if(BPBTotSector16 == 0) 
		return SECTORS_ERR;			//扇区数报错：磁盘错误！
        else 
		BPBTotSector32 = BPBTotSector16;
    }
		
	FirstFATSector  = BPBhiddsector + BPBRevdFATSec;		
		// 首FAT起始扇区=系统隐藏扇区数+FAT1前的保留扇区数

	FirstDirSector  = FirstFATSector + BPBFATPerSec16 * ParNumberFATs;
		// 根目录起始扇区=首FAT起始扇区 +(每FAT占用的扇区数 × 该分区上的FAT份数)

	FirstDataSector = FirstDirSector + 32;
		// 数据区起始扇区=根目录起始扇区 + 根目录占用扇区
	DataClusterCount= (BPBTotSector32 - FirstDataSector + BPBhiddsector)/BPBSecPerClust;
	    // 数据区簇数=(BPB中磁盘总数-数据区起始扇区+系统隐藏扇区数)/每簇的扇区数
	
	//按照Microsoft的方法判断簇数，
    //小于4085的就是FAT12，小于65525的就是FAT16，否则为FAT32；
	if(DataClusterCount<4085)
	return CLUSTER_COUNT_ERR;			//报错：FAT12文件系统不被支持！
	else 
	if(DataClusterCount<=65525)
	{
		DirSecitemCunt = FAT16;		   // FAT16目录项数的基数
		return FAT16;			       // 文件系统:FAT16
	}
	else if(DataClusterCount> 65525)
	{
		DirSecitemCunt = FAT32;					 // FAT32目录项数的基数
		BPBFATPerSec16 = ((uint16)buffer[36])|		// FAT32每FAT占用的扇区数,只用低2字节
						 ((uint16)buffer[37]<<8);

		FirstDirSector  = FirstFATSector + BPBFATPerSec16 * ParNumberFATs;
			// 根目录起始扇区=首FAT起始扇区+(每FAT占用的扇区数×该分区上的FAT份数)

		FirstDataSector = FirstDirSector + BPBSecPerClust;	
			// 数据区起始扇区=根目录起始扇区 + 每簇扇区数(单个根目录占用扇区)

		FirstDircluster = ((uint32)buffer[44])|		// FAT32根目录首簇号
						  ((uint32)buffer[45]<<8)|
						  ((uint32)buffer[46]<<16)|
						  ((uint32)buffer[47]<<24);
		return FAT32;								// 文件系统:FAT32
	}
}