
;/********************************************************************************
;*  描述:                                                                       *    
;*         矩阵键盘1602液晶显示键值                                             *    
;*         矩阵键盘定义：                                                       *
;*         P1.1-P1.3为列线,P1.4-P1.7为行线                                      *    
;*        喇叭接P1.5口  矩阵键盘P1口，1602LCD                                   *                     
;********************************************************************************/
;--------------------------------------------------------------
           RELAY  EQU  P1.4
           BEEP   EQU  P1.5
;--------------------------------------------------------------
           RS    EQU  P2.6
           RW    EQU  P2.5
           EN    EQU  P2.7
           X     EQU  26H       ;LCD 地址变量
;--------------------------------------------------------------
           ORG  0000H
           JMP  MAIN
;--------------------------------------------------------------
MAIN:      MOV  SP,#30H
           CLR   EN
		   CALL  SET_LCD
           CALL  MENU1
LOOP0:
           MOV  P1,#0F0H
           MOV  A,P1
           CJNE  A,#0F0H,LOOP1  ;有键按下转LOOP1
           JMP   LOOP0
LOOP1:
           CALL  KEY_IN
           CALL  CONV
           CALL  BEEP_BL
           JMP  LOOP0
;-----------------------------------------------------
;  LCD 初始化设置
;-----------------------------------------------------
SET_LCD:
          CLR  EN
          CALL  INIT_LCD     ;初始化 LCD
          CALL  DELAY1
          MOV  DPTR,#INFO1   ;指针指到显示信息1
          MOV  A,#1          ;显示在第一行
          CALL  LCD_SHOW
          MOV  DPTR,#INFO2   ;指针指到显示信息2
          MOV  A,#2          ;显示在第二行
          CALL  LCD_SHOW
          RET
;-----------------------------------------------------
INIT_LCD:                 ;8位I/O控制 LCD 接口初始化
          MOV  A,#38H     ;双列显示，字形5*7点阵
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#38H     ;双列显示，字形5*7点阵
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#38H     ;双列显示，字形5*7点阵
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#0CH     ;开显示，关光标，
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#01H     ;清除 LCD 显示屏
          CALL  WCOM
          CALL  DELAY1
          RET
;----------------------------------------------------
LCD_SHOW:       ;在LCD的第一行或第二行显示信息字符

          CJNE  A,#1,LINE2  ;判断是否为第一行
  LINE1:  MOV  A,#80H       ;设置 LCD 的第一行地址
          CALL  WCOM        ;写入命令
          CALL  CLR_LINE    ;清除该行字符数据
          MOV  A,#80H       ;设置 LCD 的第一行地址
          CALL  WCOM        ;写入命令
          JMP  FILL

  LINE2:  MOV  A,#0C0H      ;设置 LCD 的第二行地址
          CALL  WCOM        ;写入命令
          CALL  CLR_LINE    ;清除该行字符数据
          MOV  A,#0C0H      ;设置 LCD 的第二行地址
          CALL  WCOM
  FILL:   CLR  A            ;填入字符
          MOVC  A,@A+DPTR   ;由信息区取出字符
          CJNE  A,#0,LC1    ;判断是否为结束码
          RET
  LC1:    CALL  WDATA       ;写入数据
          INC  DPTR         ;指针加1
          JMP  FILL         ;继续填入字符
          RET
;---------------------------------------------------
CLR_LINE:                  ;清除该行 LCD 的字符
          MOV  R0,#24
   CL1:   MOV  A,#' '
          CALL  WDATA
          DJNZ  R0,CL1
          RET
;----------------------------------------------------
MENU1:                      ;LCD 显示工作菜单信息
         MOV   DPTR,#MENU2
         MOV   A,#1         ;在第一行显示信息
         CALL  LCD_SHOW
         RET
MENU2:  DB  "   KEY NUMBER   ",0
;-----------------------------------------------------
INFO1:  DB  "                ",0  ;LCD 第一行显示信息
INFO2:  DB  " KEY-CODE: -- H ",0  ;LCD 第二行显示信息
;-----------------------------------------------------
; 写指令、数据使能子程序
;-----------------------------------------------------
WCOM:                      ;写指令使能
          MOV  P0,A
          CLR RS           ;RS=L,RW=L,D0-D7=指令码，E=高脉冲
          CLR RW
          SETB EN
          CALL  DELAY0
          CLR EN
          RET
WDATA:                    ;写数据使能
          MOV  P0,A
          SETB  RS        ;RS=H,RW=L,D0-D7=数据，E=高脉冲
          CLR   RW
          SETB  EN
          CALL  DELAY0
          CLR   EN
          RET

DELAY0:   MOV  R7,#250      ;延时500微秒
          DJNZ  R7,$
          RET
;---------------------------------------------------
;在 LCD 第二行显示字符
;A=ASC DATA, B=LINE X POS
;---------------------------------------------------
LCDP2:                    ;在LCD的第二行显示字符
         PUSH  ACC        ;
         MOV  A,B         ;设置显示地址
         ADD  A,#0C0H     ;设置LCD的第二行地址
         CALL  WCOM       ;写入命令
         POP  ACC         ;由堆栈取出A
         CALL  WDATA      ;写入数据
         RET
;-----------------------------------------------------
;矩阵键盘键值读取子程序
;-----------------------------------------------------
KEY_IN:    MOV  P1,#0F0H    ;高四位作为输入  置行线为高电平 , 列线为低电平
           MOV  A,P1
           ANL  A,#0F0H		;屏蔽低四位
           MOV  B,A
           MOV  P1,#0FH     ;高四位作为输入  ;置列线为高电平，行线为低电平
           MOV  A,P1
           ANL  A,#0FH
           ORL  A,B               ;高四位与低四位重新组合
           CJNE  A,#0FFH,KEY_IN1  ;0FFH为末按键
           RET
KEY_IN1:   MOV  B,A
           MOV  DPTR,#KEYTABLE
           MOV  R3,#0FFH
KEY_IN2:   INC  R3
           MOV  A,R3
           MOVC  A,@A+DPTR
           CJNE  A,B,KEY_IN3
           MOV  A,R3              ;找到，取顺序码
           MOV  R5,#08H           ;延时
           RET
KEY_IN3:   CJNE  A,#00H,KEY_IN2  ;末完，继续查
           RET                    ;00H为结束码
;-------------------------------------------------
CONV:
          MOV   X,#11        ;设置显示起始位置
          MOV   A,R3
          ANL   A,#0F0H      ;取出高四位二进制数
          SWAP  A            ;高四位与低四位互换
          PUSH  ACC          ;压入堆栈
          CLR   C            ;C=0
          SUBB  A,#0AH       ;减10
          POP   ACC          ;弹出堆栈
          JC    ASCII0       ;该数小于10，转ASCII0
          ADD   A,#07H       ;大于10的数加上37H
ASCII0:   ADD   A,#30H       ;小于10的数加上30H
          MOV   B,X
          CALL  LCDP2

          MOV   A,R3
          ANL   A,#0FH        ;取出低四位二进制数
          PUSH  ACC
          CLR   C
          SUBB  A,#0AH        ;减10
          POP   ACC
          JC    ASCII1        ;该数小于10，转ASCII0
          ADD   A,#07H        ;大于10的数加上37H
ASCII1:   ADD   A,#30H        ;小于10的数加上30H
          INC   X
          MOV   B,X
          CALL  LCDP2
          RET
;--------------------------------------------------------
;蜂鸣器响一声子程序
;--------------------------------------------------------
BEEP_BL:
         MOV  R6,#200
  BL1:   CALL  DEX1
         CPL   BEEP
         DJNZ  R6,BL1
         MOV  R5,#50
         CALL  DELAY
         RET
 DEX1:   MOV  R7,#220
 DEX2:   NOP
         DJNZ  R7,DEX2
         RET
DELAY:                    ;延时R5×10MS
         MOV  R6,#03H
  D1:    MOV  R7,#100
         DJNZ  R7,$
         DJNZ  R6,D1
         DJNZ  R5,DELAY
         RET
;-----------------------------------------------------
DELAY1:                    ;延时5MS
         MOV  R6,#25
  D2:    MOV  R7,#100
         DJNZ  R7,$
         DJNZ  R6,D2
         RET
;-----------------------------------------------------
KEYTABLE:
//;	   0   1	2	   3	 4	 5	   6	 7	   8	 9	10    11	结束标示  
DB   0EDH,0DDH,0BDH,07Dh,  0EBh,0DBh ,0BBh,07Bh, 0E7h, 0D7h,0B7h,077h,  00H
    
;-----------------------------------------------------
    END


