#include<reg51.h>
#include<intrins.h>
typedef unsigned char u8;
typedef long int u16;

#define led P2
sbit beep=P1^5 ;

void delay(u16 i)
{
   while(i--);
}

void video(u16 m)
{
    unsigned int t;
	for(t=0;t<m;t++)
	{
	   beep=!beep;
	   delay(10) ;
	}
}

void main()
{
      u8 i;
	  u16 a=3000 ;
	  u16 b=100	;
	  u16 c=300;
      while(1)
	  {
	      a=a-b;
	      led=0xfe;
	      delay(a);
	  	  for(i=0;i<7;i++)
		  {
		    led=_crol_(led,1);
		    delay(a);	
		  }
		 led=0xff;
		 
		 video(c);

		 if(a<=1000)
		 { 
		   b=40;
		 }
		 if(a<=300)
		 {
		   while(1)
		   {
		   led=0x0;
		   beep=!beep	 ;
		   delay(10);
		   beep=!beep;
		   delay(5);
		   }	 
		 }
	  }
}