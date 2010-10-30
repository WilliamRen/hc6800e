/*****************************************************************************************
*																                         *
*	                 LED点阵实验 (左右上下流动显示)								         *
*	说明 通过P0 和 P2 作为点阵接口								                         *
*要求学员掌握 LED点阵的工作原理和 各点阵脚的定义及接法，具体接线请参考接线说明		     *
*我们采用的LED点阵式是低功耗，在做实验时可以用单片机脚直接驱动	                         *
******************************************************************************************/

#include <REG51.H>

unsigned char code taba[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned char code tabb[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};


void delay1(void)
{
unsigned char i,j,k;

for(k=10;k>0;k--)
for(i=20;i>0;i--)
for(j=248;j>0;j--);

}

void main(void)
{
unsigned char i,j;

while(1)
{
for(j=0;j<3;j++)//　　//from left to right 3 time
{
for(i=0;i<8;i++)
{
P2=taba[i];
P0=0xff;
delay1();
}
}

for(j=0;j<3;j++)//　　//from right to left 3 time
{
for(i=0;i<8;i++)
{
P2=taba[7-i];
P0=0xff;
delay1();
}
}

for(j=0;j<3;j++)//　　//from top to bottom 3 time
{
for(i=0;i<8;i++)
{
P2=0x00;
P0=tabb[7-i];

delay1();
}
}

for(j=0;j<3;j++)//　　//from bottom to top 3 time
{
for(i=0;i<8;i++)
{
P2=0x00;
P0=tabb[i];

delay1();
}
}
}
}

