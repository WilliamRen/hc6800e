
/**********双四拍运行********************/

#include <system.h>

 
///////////////////////////////////////
//步进电机驱动
UCHAR MotorStep=0;
UINT MotorTimer = 0;
unsigned int MotorDelay;

void InitMotor()
{
  F1 = 1;
  F2 = 1;
  F3 = 1;
  F4 = 1;
}


void SetMotor()
{
    if(D[12] == 0) return;

    MotorDelay=D[12];
    switch(MotorStep)
    {
       case 0:
          if(TickOut(&MotorTimer,MotorDelay))
          {
            F1 = 0;
            F2 = 1;
			F3 = 1;
			F4 = 1;
            MotorStep = 1;
          }
       break;

       case 1:
          if(TickOut(&MotorTimer,MotorDelay))
          {
            F1 = 1;
            F2 = 0;
            F3 = 1;
            F4 = 1;
            MotorStep = 2;
          }
       break;

       case 2:
          if(TickOut(&MotorTimer,MotorDelay))
          {
            F1 = 1;
            F2 = 1;
            F3 = 0;
            F4 = 1;
            MotorStep = 3;
          }
       break;


       case 3:
          if(TickOut(&MotorTimer,MotorDelay))
          {
            F1 = 1;
            F2 = 1;
            F3 = 1;
            F4 = 0;
            MotorStep = 0;
          }
       break;
    }

}
