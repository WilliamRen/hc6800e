/********************************************************************************
;*         	        跑马灯实验(二极管LED)                                       *
;																				*
;     延时实现LED流水灯效果   p2口八个灯作跑马灯。采用了寄存器存中间数	        *
;     连接方法： JP11（P2）和JP1(LED灯) 用8PIN排线连接起来					    *
;********************************************************************************/
#include "common.h"

void ledflash(void)			        
{
	LED = 0;
	do 
	{
    	LED = ~LED;
    	delay_ms(300) ;
	}while( is_quit());
}

void ledon(void)
{
	do 
	{
    	LED = 0;
    	delay_ms(300) ;
	}while( is_quit());
}

void ledoff(void)
{
	do 
	{
    	LED = 1;
    	delay_ms(300) ;
	}while( is_quit());
}

void paomadeng(void)
{
	unsigned char i;
	char LED_STATUS = 0xFE;
	do 
	{
		LED_STATUS = 0xFE;
		for (i = 0 ;i < 8 ; i++)
		{ 
			P2  = LED_STATUS;
			delay_ms(100);
			LED_STATUS = LED_STATUS << 1;	 // 左移
			LED_STATUS = LED_STATUS | 0x01;	 // 移位后，后面的位为高电平
			if (LED_STATUS == 0x7f)	 break;	 //提前退出 FOR 循环
		}
		for (i = 0 ;i < 8 ; i++)
		{
			P2  = LED_STATUS;
			delay_ms(100);
			LED_STATUS = LED_STATUS >> 1;	 // 右移
			LED_STATUS = LED_STATUS | 0x80;	 // 移位后，后面的位为高电平
		}
	}while(is_quit());
}

