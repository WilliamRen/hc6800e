/******************************************************************/
/* 	      按键状态显示试验										  *
/*	             												  *
/*  连接方法：连接JP10（P0)与JP5（按钮接口)          			  *
/*			      JP11 (P2)与JP1(LED接口) 						  *
																  *
/*按键则点亮LED灯，8路指示灯接p0口         						  *
/******************************************************************/
        K1  BIT  P0.0	  ;LED 灯
        K2  BIT  P0.1
        K3  BIT  P0.2
        K4  BIT  P0.3
		K5  BIT  P0.4	 ;喇叭开关
		K6  BIT  P0.5    ;继电器      
;*********************************************************
        ORG  0000H
        JMP  MAIN
        ORG  0030H        
;*********************************************************
MAIN:
        MOV  SP,#60H
		
LOOP:
  	   	JNB  K1,K1_Prog
        JNB  K2,K2_Prog
        JNB  K3,K3_Prog
        JNB  K4,K4_Prog
		MOV  P2,#0FFH       ;关闭所有LED
		JNB  K5,K5_Prog
		SETB P1.5			;关闭风铃器
		JNB  K6,K6_Prog
		SETB P1.4			 ;关闭继电器

        JMP  LOOP

K1_Prog:
        MOV  P2,#0FEH       ;D00灯亮
        JMP  LOOP
K2_Prog:
        MOV  P2,#0FDH       ;D01灯亮
        JMP  LOOP
K3_Prog:
        MOV  P2,#0FBH       ;D02灯亮
        JMP  LOOP
K4_Prog:
        MOV  P2,#0F7H       ;D03灯亮
        JMP  LOOP

K5_Prog:CLR  P1.5           ;喇叭
        JMP  LOOP

K6_Prog:CLR  P1.4           ;继电器
        JMP  LOOP

;*********************************************************
        END
