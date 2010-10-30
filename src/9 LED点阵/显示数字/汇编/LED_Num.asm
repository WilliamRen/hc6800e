;/*****************************************************************************************
;*																                         *
;*	                 LED点阵实验(流动显示1 2 3 4 5 6 7 8 9)								 *
;*	说明 通过P0 和 P2 作为点阵接口								                         *
;*要求学员掌握 LED点阵的工作原理和 各点阵脚的定义及接法，具体接线请参考接线说明		     *
;*我们采用的LED点阵式是低功耗，在做实验时可以用单片机脚直接驱动	                         *
;******************************************************************************************/	

	TIM EQU 30H
	
	CNTA EQU 31H
	
	CNTB EQU 32H
	
	ORG 0000H
	
	LJMP START
;	
	ORG 0BH        ;//定时中断0 中断向量地址
;	
	LJMP T0_Inter;
	
	ORG 30H
	
START: MOV TIM,#00H
	
	MOV CNTA,#00H
	
	MOV CNTB,#00H
	
	MOV TMOD,#01H
	
	MOV TH0,#(65536-4000)/256
	
	MOV TL0,#(65536-4000) MOD 256
	
	SETB TR0
				    ;开启T0中断
	SETB ET0		; 允许T0中断
	
	SETB EA			;开总中断
	
	SJMP $			;原地踏步

;*****************定时中断服务程序*******************	
T0_Inter:
	
	MOV TH0,#(65536-4000)/256
	
	MOV TL0,#(65536-4000) MOD 256
	
	MOV DPTR,#TAB
	
	MOV A,CNTA
	
	MOVC A,@A+DPTR
	
	MOV P2,A
	
	MOV DPTR,#DIGIT
	
	MOV A,CNTB
	
	MOV B,#8
	
	MUL AB
	
	ADD A,CNTA
	
	MOVC A,@A+DPTR
	
	MOV P0,A
	
	INC CNTA
	
	MOV A,CNTA
	
	CJNE A,#8,NEXT
	
	MOV CNTA,#00H
	
	NEXT: INC TIM
	
	MOV A,TIM
	
	CJNE A,#250,NEX
	
	MOV TIM,#00H
	
	INC CNTB
	
	MOV A,CNTB
	
	CJNE A,#10,NEX
	
	MOV CNTB,#00H
	
	NEX: RETI 
	
	TAB: DB 0FEH,0FDH,0FBH,0F7H,0EFH,0DFH,0BFH,07FH
	
	DIGIT: DB 00H,00H,3EH,41H,41H,41H,3EH,00H
	
	DB 00H,00H,00H,00H,21H,7FH,01H,00H
	
	DB 00H,00H,27H,45H,45H,45H,39H,00H
	
	DB 00H,00H,22H,49H,49H,49H,36H,00H
	
	DB 00H,00H,0CH,14H,24H,7FH,04H,00H
	
	DB 00H,00H,72H,51H,51H,51H,4EH,00H
	
	DB 00H,00H,3EH,49H,49H,49H,26H,00H
	
	DB 00H,00H,40H,40H,40H,4FH,70H,00H
	
	DB 00H,00H,36H,49H,49H,49H,36H,00H
	
	DB 00H,00H,32H,49H,49H,49H,3EH,00H
	
	END
	
