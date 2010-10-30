;/********************************************************************************
;*  描述:                                                                       *
;*         1602字符型LCD显示演示程序                                            *
;*         在第一行显示  welcome                                                *
;*         在第二行显示  www.PRECHIN.com                                        *
;*                                                                              *
;********************************************************************************/
; 端口定义
RS	EQU	P2.6
RW	EQU	P2.5
EP	EQU	P2.7

	ORG	0000H
	LJMP	MAIN
MAIN:
	LCALL	LCD_INIT		; 初始化LCD
	MOV	A,#15
	LCALL	DELAY_MS		;

MAIN_LOOP:	
;  在第一行显示字符串"welcome!"
	MOV	A,#4
	LCALL	SET_LCD_POS		; 设置LCD光标到第一行的第5个字符
	
	MOV	DPTR,#TAB_WELCOME	; "welcome!"字串表格地址
	LCALL	DISPLAY_STRING		; 显示字符串
; 在第二行显示字符串"www.PRECHIN.com"	
	MOV	A,#41H			; 
	LCALL	SET_LCD_POS		; 设置LCD光标到第二行第二个字符
	
	MOV	DPTR,#TAB_WILLAR
	LCALL	DISPLAY_STRING

; 闪烁显示内容	
	MOV	A,#200			; 
	LCALL	DELAY_MS		; 
	
	LCALL	LCD_TURN_OFF	
	MOV	A,#200			; 
	LCALL	DELAY_MS		; 
	
	LCALL	LCD_TURN_ON
	
	MOV	A,#200			; 
	LCALL	DELAY_MS		; 
	
	LCALL	LCD_TURN_OFF
		
	MOV	A,#200			; 
	LCALL	DELAY_MS		; 
	
	LCALL	LCD_TURN_ON
	
	MOV	A,#200			; 
	LCALL	DELAY_MS		; 
;清屏
	LCALL	LCD_CLEAR
	MOV	A,#1
	LCALL	DELAY_MS
; 重新显示	
	JMP	MAIN_LOOP	

;显示字符串函数
;传入参数：DPTR(字符串表格地址)
;返回值：无
DISPLAY_STRING:	
	CLR	A
	MOVC	A,@A+DPTR		; 
	JZ	END_DISPLAY_STRING	; 如果遇到00H表示表格结束
	LCALL	LCD_WRITE_DATA		; 写数据到LCD
	INC	DPTR			; 指向表格的下一字符
	MOV	A, #200			; 
	LCALL	DELAY_MS		; 
	SJMP	DISPLAY_STRING		; 循环直到字符串结束
END_DISPLAY_STRING:
	RET
	
	
; 初始化LCD
LCD_INIT:

; 设置显示格式---
	MOV	A,#38H			; 38H --- 16*2行显示,5*7点阵,8位数据接口
	LCALL	LCD_WRITE_COMMAND
	MOV	A,#1
	LCALL	DELAY_MS
;开显示
	LCALL	LCD_TURN_ON
;读写后指针加1
	MOV	A,#06H			; 06H --- 读写后指针加1
	LCALL	LCD_WRITE_COMMAND
	MOV	A,#1
	LCALL	DELAY_MS
; 清除LCD屏幕
	LCALL	LCD_CLEAR
	RET


;开显示
LCD_TURN_ON:
	MOV	A,#0CH			; 0CH --- 开显示,无光标
	LCALL	LCD_WRITE_COMMAND
	MOV	A,#1
	LCALL	DELAY_MS
	RET
	
; 关显示
LCD_TURN_OFF:
	MOV	A,#08H			; 08H --- 关显示
	LCALL	LCD_WRITE_COMMAND
	MOV	A,#1
	LCALL	DELAY_MS
	RET

; 清除LCD屏幕
LCD_CLEAR:
	MOV	A,#01H			; 01H清屏指令			
	LCALL	LCD_WRITE_COMMAND
	MOV	A,#1
	LCALL	DELAY_MS
	RET

;设置LCD当前光标的位置

SET_LCD_POS:
	ORL	A,#80H		; 
	LCALL	LCD_WRITE_COMMAND
	RET

; 写入控制指令到LCD
; 传入参数: ACC(要写入的命令)
; 返回值: 无
LCD_WRITE_COMMAND:
	LCALL	CHECK_LCD_BUSY
	CLR	RS
	CLR	RW
	CLR	EP
	NOP
	NOP
	MOV	P0,A			; 写入数据到LCD端口
	NOP
	NOP
	NOP
	NOP
	SETB	EP
	NOP
	NOP
	NOP
	NOP
	CLR	EP
	RET
	
; 写入显示数据到LCD
; 传入参数: ACC(要写入的数据)
; 返回值: 无
LCD_WRITE_DATA:
	LCALL	CHECK_LCD_BUSY
	SETB	RS
	CLR	RW
	CLR	EP
	NOP
	NOP
	MOV	P0,A			; 写入数据到LCD端口
	NOP
	NOP
	NOP
	NOP
	SETB	EP
	NOP
	NOP
	NOP
	NOP
	CLR	EP
	RET
	
CHECK_LCD_BUSY:
	CLR	RS
	SETB	RW
	SETB	EP
	NOP
	NOP
	NOP
	NOP
	MOV	C,P0.7			; 读取忙碌位
	NOP
	NOP
	CLR 	EP
	NOP
	NOP
	JC	CHECK_LCD_BUSY		; 等待LCD空闲(P0.7=0)
	
	RET

; 延时子程序
; 传入参数: ACC(延时时间,单位毫秒)
; 返回值: 无
DELAY_MS:
	MOV	R7,A
DELAY_LOOP1:
	MOV	R6,#0E8H
DELAY_LOOP2:
	NOP
	NOP
	DJNZ	R6,DELAY_LOOP2
	DJNZ	R7,DELAY_LOOP1
	RET
		

TAB_WILLAR:
	DB	"www.PRECHIN.com"
	DB	00			; 字符结束标志
	
TAB_WELCOME:
	DB	"Welcome!"
	DB	00			; 字符结束标志

	END

