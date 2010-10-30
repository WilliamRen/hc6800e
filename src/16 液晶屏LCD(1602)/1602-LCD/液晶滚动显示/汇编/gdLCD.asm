;/********************************************************************************

;*  描述:                LCD1602 滚动显示                                       *
;* 显示方式：                                                                   *
;* 1、从左到右逐字显示，闪动二次，清屏。                                        *
;* 2、再从右到左逐字显示，闪动二次，清屏。                                      *
;* 3、周期性地重复上述显示方式。                                                *                            
;********************************************************************************/
                                                                         
          LCD_RS   EQU  P2.6    //控制端接口                                         
          LCD_RW   EQU  P2.5                                              
          LCD_EN   EQU  P2.7                                             
                                                                         
;*******************************************************************     
                                                                         
          ORG  0000H                                                     
          AJMP MAIN                                                      
          ORG  0030H                                                     
                                                                         
;*******************************************************************     
MAIN:                                                                    
          MOV  SP,#60H                                                   
          MOV  R4,#02H               ;设置闪烁次数                       
          ACALL  LCD_INIT                                                
MAIN1:                                                                   
          ACALL LCDSET1                                                  
          MOV  DPTR,#CHAR1                                               
          ACALL WRITE1              ;显示“WELCOME TO”                  
          MOV  A,#0C0H              ;显示第二行左边第一位位置            
          ACALL  LCD_CMD                                                 
          MOV  DPTR,#CHAR2      ;显示             
          ACALL  WRITE1                                                  
          ACALL DELAY4              ;延时                                
          ACALL DELAY4                                                   
          ACALL SHAN                  ;闪烁两次                          
                                                                         
          ACALL LCDSET2                                                  
          MOV  DPTR,#CHAR3      ;显示“WELCOME TO”                      
          ACALL WRITE1                                                   
          MOV  A,#0CFH              ;显示第二行右边第一位位置            
          ACALL  LCD_CMD                                                 
          MOV  DPTR,#CHAR4      ;显示WWW.PRECHIN.COM 
          ACALL  WRITE1                                                  
          ACALL DELAY4              ;延时                                
          ACALL DELAY4                                                   
          ACALL SHAN                 ;闪烁两次                           
          ACALL  MAIN1                                                   
                                                                         
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
                                                                         
          MOV  A,#08H                  ;显示关                           
          ACALL LCD_CMD             ;进行LCD忙检测                       
                                                                         
          MOV A,#01H                   ;清除屏幕                         
          ACALL LCD_CMD            ;进行LCD忙检测                        
                                                                         
          MOV A,#06H                  ;移动光标                          
          ACALL LCD_CMD           ;进行LCD忙检测                         
                                                                         
          MOV A,#0CH                 ;显示开，关光标                     
          ACALL LCD_CMD           ;进行LCD忙检测                         
          RET                                                            
;***************************************************************         
;显示位置与移动光标设定                                                  
;***************************************************************         
LCDSET1:                                                                 
          MOV A,#01H                 ;清除屏幕                           
          ACALL LCD_CMD                                                  
          ACALL DELAY5MS                                                 
          MOV A,#06H                ;移动光标（光标加1）                 
          ACALL LCD_CMD                                                  
          ACALL DELAY5MS                                                 
          MOV A,#80H                ;显示第一行左边第一位位置            
          ACALL LCD_CMD                                                  
          ACALL DELAY5MS                                                 
          RET                                                            
LCDSET2:                                                                 
          MOV A,#01H                ;清除屏幕                            
          ACALL LCD_CMD                                                  
          ACALL DELAY5MS                                                 
          MOV A,#04H               ;移动光标（光标减1）                  
          ACALL LCD_CMD                                                  
          ACALL DELAY5MS                                                 
          MOV A,#8FH               ;显示第一行右边第一位位置             
          ACALL LCD_CMD                                                  
          ACALL DELAY5MS                                                 
          RET                                                            
;***************************************************************         
;写指令数据到LCD                                                         
;RS=L,RW=L,D0-D7=指令码，E=高脉冲                                        
;***************************************************************         
LCD_CMD:                                                                 
          ACALL  CHECKBUSY                                               
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
; 发送字符串子程序                                                       
;***************************************************************         
WRITE1:                                                                  
          MOV  R0,#16                                                    
WRITE0:                                                                  
          CLR  A                                                         
          MOVC  A,@A+DPTR                                                
          INC  DPTR                                                      
          ACALL LCD_WDATA                                                
          CALL  DELAY                    ;加延时形成滚动效果             
          DJNZ  R0,WRITE0                                                
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
;闪烁子程序                                                              
;***************************************************************         
SHAN:                                                                    
          MOV A,#08H                ;关闭显示                            
          ACALL LCD_CMD                                                  
          ACALL DELAY4                                                   
          MOV A,#0CH               ;开显示，关闭光标                     
          ACALL LCD_CMD                                                  
          ACALL DELAY4                                                   
          DJNZ R4,SHAN                                                   
          MOV  R4,#02H            ;设置闪烁次数                          
          RET                                                            
;***************************************************************         
;延时120MS子程序                                                         
;发送字符串时使用                                                        
;***************************************************************         
DELAY:                                                                   
             MOV R7,#240                                                 
 DL1:     MOV R6,#250                                                    
 DL2:     DJNZ R6,DL2                                                    
             DJNZ R7,DL1                                                 
             RET                                                         
;***************************************************************         
;延时800MS子程序                                                         
;闪烁时使用                                                              
;***************************************************************         
DELAY4:                                                                  
             MOV R0,#40                                                  
 DL3:     MOV R1,#100                                                    
 DL4:     MOV R2,#100                                                    
 DL5:     DJNZ R2,DL5                                                    
             DJNZ R1,DL4                                                 
             DJNZ R0,DL3                                                 
             RET                                                         
;***************************************************************         
;延时5MS子程序                                                           
;LCD初始化使用                                                           
;***************************************************************         
DELAY5MS:                                                                
             MOV  R1,#10                                                 
 DL6:     MOV  R2,#249                                                   
 DL7:     DJNZ R2,DL7                                                    
             DJNZ R1,DL6                                                 
             RET                                                         
;***************************************************************         
;延时1MS子程序                                                           
;***************************************************************         
DELAY1MS:                                                                
             MOV  R6,#14H        ;20                                     
 DL8:     MOV  R7,#19H        ;25                                        
 DL9:     DJNZ  R7,DL9                                                   
             DJNZ  R6,DL8                                                
             RET                                                         
;***************************************************************         
CHAR1:                                                                   
DB  "   WELCOME TO   "                                                   
                                                                         
CHAR2:                                                                   
DB  " WWW.PRECHIN.COM "                                                   
                                                                         
CHAR3:                                                                   
DB  "   OT EMOCLEW   "                                                   
                                                                         
CHAR4:                                                                   
DB  " MOC.NIHCERP.WWW "                                                   
                                                                         
;***************************************************************         
       END                                                               
                                                                         
                                                                         
                                                                         
                                                         
