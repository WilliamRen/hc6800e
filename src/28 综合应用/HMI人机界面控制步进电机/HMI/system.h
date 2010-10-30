#include <reg52.h>
#include <intrins.h>

#define  AT24C02 0xa0  //AT24C02 µÿ÷∑

sbit Ch_Focus = P2^4;
sbit Inc_num = P2^3;
sbit Dec_num = P2^2;

void changefocus(unsigned char index);
void ChangeNumber();
void handle_ASCII(unsigned char index);
void Focus_Flash(unsigned char x, unsigned char y);
void SaveData();
void ReadData();


extern unsigned int Tick_10ms;
extern void MotorRun(unsigned int *pD);