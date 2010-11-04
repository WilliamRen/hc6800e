#include "common.h"

//内部函数声明


//各个子模块中对外函数声明
extern void beep(void);
extern void ledflash(void);
extern void ledon(void);
extern void ledoff(void);
extern void test_ir(void);
extern void paomadeng(void);
extern void test_1602(void);
extern void test_18B20(void);

//主程序
void main(void)
{
	IRInit();
	//lcd_init();
	lcd_welcome_menu();
	while(1)
	{
		process_message();
	}
}

void process_message(void)
{
	switch (ir_key_value)
	{
	case  MY_KEY_ESC  : break; 
	case  MY_KEY_MODE : lcd_init();break; 
	case  MY_KEY_MUTE : break; 
	case  MY_KEY_PLAY : break; 
	case  MY_KEY_PREV : break; 
	case  MY_KEY_NEXT : break; 
	case  MY_KEY_EQ   : lcd_welcome_menu();break; 
	case  MY_KEY_VOLD : break; 
	case  MY_KEY_VOLU : break; 
	case  MY_KEY_RPT  : ledon();break; 
	case  MY_KEY_RSD  : ledoff();break; 
	case  MY_KEY_0    : ledflash();break;
	case  MY_KEY_1    : paomadeng();break;
	case  MY_KEY_2    : test_ir();break;
	case  MY_KEY_3    : test_1602();break; 
	case  MY_KEY_4    : test_18B20();break; 
	case  MY_KEY_5    : beep();break; 
	case  MY_KEY_6    : break; 
	case  MY_KEY_7    : break; 
	case  MY_KEY_8    : break; 
	case  MY_KEY_9    : break;
	}
	//delay_us(1000);	 
}







