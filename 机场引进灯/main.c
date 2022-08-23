#include<reg51.h>
#include<intrins.h>
typedef unsigned char u8;
typedef unsigned int u16;

#define led P2

void delay(u16 i)
{
while(i--);
}

void main()
{
      u8 i;
	
      while(1)
	  {
	      led=0x1f;
	      delay(5000);
	  	  for(i=0;i<7;i++)
		  {
		  led=_cror_(led,1);
		  delay(5000);	
		  }
	  }
}