
;/********************************************************************************
;*  描述:                LCD1602 移动显示                                       *
;* 显示方式：                                                                   *
;* 1、LCD第一行显示：  BIG BIG WORLD                                            *
;*      LCD第二行显示：I LOVE PUZHONG                                             *
;* 2、先将两行字符写入DDRAM中，然后从右至左逐字移出显示。                       *
;* 3、在指定位置停留1.6秒后，闪烁2次，清屏。                                    *
;* 4、反复循环上述显示方式。                                                    *                            
;********************************************************************************/
 
                                                                        
          LCD_RS  EQU P2.6                                              
          LCD_RW  EQU P2.5                                              
          LCD_EN  EQU P2.7                                              
                                                                        
;*******************************************************************    
             ORG  0000H                                                 
             AJMP  MAIN                                                 
             ORG  0030H                                                 
;*******************************************************************    
MAIN:                                                                   
             MOV SP,#60H                                                
             ACALL LCD_INIT           ;LCD初始化                        
MAIN1:                                                                  
             ACALL CLR_LCD           ;清LCD                             
             MOV A,#90H                ;在第一行第17列的位置            
             ACALL LCD_CMD                                              
             MOV DPTR,#LINE1       ;第一行字符串的起始地址送入DPTR。    
             ACALL WRITE                                                
             MOV A,#0D0H              ;在第二行第17列的位置             
             ACALL LCD_CMD                                              
             MOV DPTR,#LINE2       ;第二行字符串的起始地址送入DPTR。    
             ACALL WRITE                                                
                                                                        
             MOV  R3,#10H             ;向左移动16格                     
LOOPA:                                                                  
             MOV  A,#18H               ;字符同时左移一格                
             ACALL LCD_CMD                                              
             MOV  R5,#03H             ;延时375MS                        
             ACALL  DELAY125MS                                          
             DJNZ  R3,LOOPA                                             
                                                                        
             ACALL  DELAY4            ;延时1.6s                         
             ACALL  DELAY4                                              
             MOV  R4,#02H            ;设置闪烁次数                      
             ACALL  FLASH              ;开始闪烁                        
             AJMP  MAIN1                                                
                                                                        
LINE1:                                                                  
        DB " BIG BIG WORLD",00H                                       
LINE2:                                                                  
        DB " I LOVE PUZHONG",00H                                       
                                                                        
;***************************************************************        
;LCD初始化设定子程序                                                    
;***************************************************************        
LCD_INIT:                                                               
          ACALL DELAY5MS            ;延时15MS                           
          ACALL DELAY5MS            ;等待LCD电源稳定                    
          ACALL DELAY5MS                                                
                                                                        
          MOV A,#38H                   ;16*2显示，5*7点阵，8位数据      
          ACALL LCD_CMD_NC      ;不进行LCD忙检测                        
          ACALL DELAY5MS                                                
                                                                        
          MOV A,#38H                   ;16*2显示，5*7点阵，8位数据      
          ACALL LCD_CMD_NC      ;不进行LCD忙检测                        
          ACALL DELAY5MS                                                
                                                                        
          MOV A,#38H                   ;16*2显示，5*7点阵，8位数据      
          ACALL LCD_CMD_NC      ;不进行LCD忙检测                        
          ACALL DELAY5MS                                                
                                                                        
          MOV  A,#08H                 ;显示关                           
          ACALL LCD_CMD            ;进行LCD忙检测                       
                                                                        
          MOV A,#01H                  ;清除屏幕                         
          ACALL LCD_CMD            ;进行LCD忙检测                       
                                                                        
          MOV  A,#0CH                ;显示开，关光标                    
          ACALL LCD_CMD           ;进行LCD忙检测                        
                                                                        
          RET                                                           
;***************************************************************        
;清LCD子程序                                                            
;***************************************************************        
CLR_LCD:                                                                
           MOV A,#01H                ;清除屏幕                          
           ACALL LCD_CMD         ;进行LCD忙检测                         
           RET                                                          
                                                                        
;***************************************************************        
;写指令数据到LCD                                                        
;RS=L,RW=L,D0-D7=指令码，E=高脉冲                                       
;***************************************************************        
LCD_CMD:                                                                
          CALL  CHECKBUSY                                               
LCD_CMD_NC:                                                             
          CLR LCD_RS                                                    
          CLR LCD_RW                                                    
          MOV  P0,A                                                     
          SETB LCD_EN                                                   
          NOP                                                           
          NOP                                                           
          NOP                                                           
          NOP                                                           
          CLR  LCD_EN                                                   
          RET                                                           
;***************************************************************        
;写显示数据到LCD                                                        
;RS=H,RW=L,D0-D7=数据，E=高脉冲                                         
;***************************************************************        
LCD_WDATA:                                                              
          ACALL  CHECKBUSY                                              
          SETB  LCD_RS                                                  
          CLR   LCD_RW                                                  
          MOV   P0,A                                                    
          SETB  LCD_EN                                                  
          NOP                                                           
          NOP                                                           
          NOP                                                           
          NOP                                                           
          CLR   LCD_EN                                                  
          RET                                                           
;***************************************************************        
;检测LCD控制器忙状态                                                    
;读数据                                                                 
;RS=L,RW=H,E=H,输出：D0-D7=数据                                         
;P0.7=1，LCD忙，等待。P0.7=0，LCD闲，可以进行读写操作。                 
;***************************************************************        
CHECKBUSY:                                                              
          PUSH  ACC                                                     
          MOV  P0,#0FFH                                                 
          CLR   LCD_RS                                                  
          SETB  LCD_RW                                                  
          SETB  LCD_EN                                                  
BUSYLOOP:                                                               
          NOP                                                           
          JB P0.7,BUSYLOOP                                              
          CLR  LCD_EN                                                   
          POP  ACC                                                      
          RET                                                           
;***************************************************************        
; 发送字符串子程序                                                      
;***************************************************************        
WRITE:                                                                  
          PUSH ACC                                                      
WRITE1:                                                                 
          CLR  A                                                        
          MOVC  A,@A+DPTR                                               
          JZ  WRITE2                                                    
          INC  DPTR                                                     
          ACALL LCD_WDATA                                               
          JMP  WRITE1                                                   
WRITE2:                                                                 
          POP  ACC                                                      
          RET                                                           
;***************************************************************        
;闪烁子程序                                                             
;***************************************************************        
FLASH:                                                                  
          MOV A,#08H                ;关闭显示                           
          ACALL LCD_CMD                                                 
          ACALL DELAY4                                                  
          MOV A,#0CH                ;开显示，关闭光标                   
          ACALL LCD_CMD                                                 
          ACALL DELAY4                                                  
          DJNZ R4,FLASH                                                 
          RET                                                           
;***************************************************************        
;延时5MS子程序                                                          
;LCD初始化使用                                                          
;***************************************************************        
DELAY5MS:                                                               
          MOV  R6,#10                                                   
 DL1:                                                                   
          MOV  R7,#249                                                  
 DL2:                                                                   
          DJNZ R7,DL2                                                   
          DJNZ R6,DL1                                                   
          RET                                                           
;***************************************************************        
;延时125MS子程序                                                        
;字符串移动时使用                                                       
;***************************************************************        
DELAY125MS:                                                             
 DL3:                                                                   
          MOV  R6,#250                                                  
 DL4:                                                                   
          MOV  R7,#249                                                  
 DL5:                                                                   
          DJNZ R7,DL5                                                   
          DJNZ R6,DL4                                                   
          DJNZ R5,DL3                                                   
          RET                                                           
;***************************************************************        
;延时800MS子程序                                                        
;闪烁时使用                                                             
;***************************************************************        
DELAY4:                                                                 
          MOV R5,#40                                                    
 DL6:                                                                   
          MOV R6,#100                                                   
 DL7:                                                                   
          MOV R7,#100                                                   
 DL8:                                                                   
          DJNZ R7,DL8                                                   
          DJNZ R6,DL7                                                   
          DJNZ R5,DL6                                                   
          RET                                                           
;***************************************************************        
          END                                                