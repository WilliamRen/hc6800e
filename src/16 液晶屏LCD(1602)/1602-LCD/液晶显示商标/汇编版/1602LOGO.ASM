;********************************************************************************
;																				*
;																				*
;                 1602液晶显示                                                  *
;	p2.6 读操作	  p2.5 写操作	 p2.7 使能控制端							    *
;                                                                               *
;																				*
;********************************************************************************
RS EQU p2.6;液晶接口的定义
RW EQU p2.5
E  EQU p2.7
	org	0000h
	MOV SP ,#70H;设置堆栈
	MOV p0,#01H	;清屏
	CALL ENABLE
	MOV p0,#38H	;显示功能
    	CALL ENABLE
	MOV p0,#0FH	;显示开关控制
 	CALL ENABLE
	MOV p0,#06H	;+1
	CALL ENABLE
	MOV p0,#80H;第一行的开始位置
	cALL ENABLE
	MOV DPTR,#TABLE1;显示
	CALL WRITE1;到TABLE1取码?
	MOV p0,#0C0H;第二行的位置
	CALL ENABLE
	MOV DPTR,#TABLE2;显示
	CALL WRITE1;到TABLE2 取码
	;MOV p0,#0C2H;光标定位0C0H
	CALL ENABLE
     	JMP $
ENABLE: CLR RS 			;送命令
	CLR RW
	CLR E
	CALL DELAY
	SETB E
	RET
WRITE1: MOV R1,#00H   ;显示table中的值
	A1: MOV A,R1;到table取码
	MOVC A,@A+DPTR
	call  wRITE2   ;显示到lcd
	INC R1
	CJNE A,#00H,A1  ;是否到00h
	RET
WRITE2:MOV p0,A  ;显示
	SETB RS
	CLR RW
	CLR E
	CALL DELAY
	SETB E
	RET
DELAY: MOV R4,#05
	D1:MOV R5,#0FFH
	DJNZ R5,$
	DJNZ R4,D1
	RET
table1: db"SHEN ZHEN PUZHONG ",00H
table2: db"G TECHNOLOGY    ",00H
end

