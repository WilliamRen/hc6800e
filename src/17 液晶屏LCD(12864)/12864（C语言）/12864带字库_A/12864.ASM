#include"reg51.h"

void delay(void)  
{
    unsigned char a,b,c;
    for(c=1;c>0;c--);
	

        for(b=142;b>0;b--) ;

            for(a=2;a>0;a--);

}


void main()
{
P2=0xff;
delay();
P2=0x00;
delay();
}
