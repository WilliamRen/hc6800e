;********************************************************************************
;P0.0小键盘按下P2.0亮再按0.5秒灭。
;连接方法：连接方法：JP10（P2） 与 JP1 （LED灯）连接，   					    *
;                    JP11（P0） 与 JP5（按键接口）连接                          *
;********************************************************************************
;********************************************************************************


; FLAG 20H EXPRESS THE 0.5S TIME ARRIVED.
; FLAG 21H EXPRESS THE LED LIGHTED.
	ORG	0000h
	AJMP	main0
	ORG	0030h
MAIN0:
	CLR	21H		;清标志
main:
	MOV	C,P0.0		;检测按键
	JC	main
	MOV	R6,#200		;设定延时
	MOV	R7,#200
TT0:
	JNB	21H,TT2		;检测灯状态
tt:
	MOV	C,P0.0		;检测按键
	JNC	tt1

	JNB	20H,MAIN	;检测时间标志
	CLR	20H	
	AJMP	TT3	
TT2:
	MOV	c,P0.0		;检测按键
	JNC	tt2
TT3:
	CPL	21H		;改变灯状态
	CPL	p2.0
	AJMP	main
TT1:	NOP			;延时但不是子程序
	NOP
	NOP
	NOP
	NOP
	NOP
	DJNZ	R6,TT
	MOV	R6,#200
	DJNZ	R7,TT
	MOV	R7,#200
	SETB	20H
	AJMP	TT	
	END
