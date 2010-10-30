;********************************************************************************
;             1602液晶6位计数器:本程序调试通过                                  *
;      硬件： 插上1602液晶							                        	*

;********************************************************************************

	RS EQU P2.6	   //液晶屏读操作定义
	RW EQU P2.5	   //液晶屏写操作定义
	E EQU P2.7
	org 00h
	a_bit equ 30h         	;个位数存放处
	b_bit equ 31h         	;十位数存放处
	c_bit equ 32h         	;百位数存放处
	d_bit equ 33h         	;千位数存放处
	e_bit equ 34h         	;万位数存放处
	f_bit equ 35h         	;十万位数存放处
	mov	a,#30h  	;30H的ASCIC码为0
	mov	a_bit,a
	mov	b_bit,a
	mov	c_bit,a
	mov	d_bit,a
	mov	e_bit,a
	mov	f_bit,a
	org	0030h
	MOV P0,#00000001B	;清屏
	ACALL ENABLE
	MOV P0,#00111000B	;显示功能
        ACALL ENABLE
	MOV P0,#00001100B	;显示开关控制
 	ACALL ENABLE
	MOV P0,#06H		;输入模式
	ACALL ENABLE
        MOV p0,#80H		;第一行的开始位置
	cALL ENABLE
 	MOV P0,#00000001B	;清屏
	ACALL ENABLE
	MOV DPTR,#TABLE1	;显示PASS WORD
	CALL WRITE1		;到TABLE1取码?
stlop:  acall display    	;显示
        inc a_bit     		;加1
        mov	a,a_bit
        cjne	a,#3AH,stlop	;如果A的值到3AH那么继续否则重新
        mov	a_bit,#30h      ;显示数送30H（就是显示0)
        inc b_bit
        mov	a,b_bit
        cjne	a,#3AH,stlop
        mov	b_bit,#30h
        inc c_bit
        mov	a,c_bit
        cjne	a,#3AH,stlop
        mov	c_bit,#30h
        inc d_bit
        mov	a,d_bit
        cjne	a,#3AH,stlop
        mov	d_bit,#30h
        inc e_bit
        mov	a,e_bit
        cjne	a,#3AH,stlop
        mov	e_bit,#30h
        inc f_bit
        mov	a,f_bit
        cjne	a,#3AH,stlop
        mov	f_bit,#30h
	ajmp	stlop
display:			;显示子程序
	MOV P0,#0C9H
        ACALL ENABLE
        MOV A,A_BIT
 	ACALL WRILTE2
	MOV P0,#0C8H		;显示的位置
	ACALL ENABLE
     	MOV A,B_BIT      	;显示10位数
	ACALL WRILTE2    	;显示
	MOV P0,#0C7H		;数据存贮器地址
	ACALL ENABLE
 	MOV A,C_BIT
	ACALL WRILTE2
	MOV P0,#0C6H		;数据存贮器地址
	ACALL ENABLE
 	MOV A,D_BIT
	ACALL WRILTE2
	MOV P0,#0C5H		;数据存贮器地址
	ACALL ENABLE
 	MOV A,E_BIT
	ACALL WRILTE2
	MOV P0,#0C4H		;数据存贮器地址
	ACALL ENABLE
  	MOV A,F_BIT
	ACALL WRILTE2
				;1MS延时(按12MHZ算)
;====================================================
D1MS:	MOV R7,#2
      	DJNZ R7,$
      	RET
WRILTE2:MOV p0,A  ;显示
	SETB RS
	CLR RW
	CLR E
	CALL DELAY
	CALL DELAY3
	SETB E
	RET
 WRITE1: MOV R1,#00H   		;显示table中的值
	A1: MOV A,R1		;到table取码
	MOVC A,@A+DPTR
	call  WRILTE2   	;显示到lcd
	INC R1
	CJNE A,#00H,A1  	;是否到00h
	RET
ENABLE: CLR RS 			;送命令
	CLR RW
	CLR E
	ACALL DELAY
	SETB E
	RET
DELAY: MOV P0,#0FFH
	CLR RS
	SETB RW
	CLR E
	NOP
	SETB E
	JB P0.7,DELAY		;判断忙标志
	RET
DELAY3: 			; 延时程序
	MOV  R7,#255
	D1:MOV R6,#25
	D2: DJNZ R6,D2
	DJNZ R7,D1
RET
table1: db"   Counter      ",00H
end

