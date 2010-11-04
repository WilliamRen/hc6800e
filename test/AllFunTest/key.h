#ifndef ___IR_REMOTE_KEY_H_H___
#define ___IR_REMOTE_KEY_H_H___

#define IR_REMOTE_ONE   // 默认使用第一个遥控器

//遥控器按键定义
#ifdef IR_REMOTE_ONE
//遥控器1每个按键值的声明（共21个按键）0xH 0D
#define MY_KEY_ESC  0x45	//01  1
#define MY_KEY_MODE 0x46	//02  2
#define MY_KEY_MUTE 0x47	//03  3
#define MY_KEY_PLAY 0x44	//04  4
#define MY_KEY_PREV 0x40	//05  5
#define MY_KEY_NEXT 0x43	//06  6
#define MY_KEY_EQ   0x07	//07  7
#define MY_KEY_VOLD 0x15	//08  8
#define MY_KEY_VOLU 0x09	//09  9
#define MY_KEY_RPT  0x19	//0A  10
#define MY_KEY_RSD  0x0D	//0B  11

#define MY_KEY_0    0x16	//0C  12
#define MY_KEY_1    0x0C	//0D  13
#define MY_KEY_2    0x18	//0E  14 
#define MY_KEY_3    0x5E	//0F  15
#define MY_KEY_4    0x08	//10  16
#define MY_KEY_5    0x1C	//11  17
#define MY_KEY_6    0x5A	//12  18
#define MY_KEY_7    0x42	//13  19
#define MY_KEY_8    0x52	//14  20
#define MY_KEY_9    0x4A	//15  21

#else
//遥控器2每个按键值的声明（共21个按键）0xH 0D
#define MY_KEY_ESC  0x45	//01  1
#define MY_KEY_MODE 0x46	//02  2
#define MY_KEY_MUTE 0x47	//03  3
#define MY_KEY_PLAY 0x44	//04  4
#define MY_KEY_PREV 0x40	//05  5
#define MY_KEY_NEXT 0x43	//06  6
#define MY_KEY_EQ   0x07	//07  7
#define MY_KEY_VOLD 0x15	//08  8
#define MY_KEY_VOLU 0x09	//09  9
#define MY_KEY_RPT  0x19	//0A  10
#define MY_KEY_RSD  0x0D	//0B  11

#define MY_KEY_0    0x16	//0C  12
#define MY_KEY_1    0x0C	//0D  13
#define MY_KEY_2    0x18	//0E  14 
#define MY_KEY_3    0x5E	//0F  15
#define MY_KEY_4    0x08	//10  16
#define MY_KEY_5    0x1C	//11  17
#define MY_KEY_6    0x5A	//12  18
#define MY_KEY_7    0x42	//13  19
#define MY_KEY_8    0x52	//14  20
#define MY_KEY_9    0x4A	//15  21
#endif

#endif
