#include<reg51.h>
typedef unsigned char u8;
typedef unsigned int u16;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit beep=P1^5;

#define GPIO_KEY P1
#define GPIO_DIG P0

u8 KeyValue;
u8 num;
u8 m;
u8 n;


u8 smgduan[4][4]={     0x06,0x5b,0x4f,0x00,
                       0x66,0x6d,0x7d,0x40,
				       0x07,0x7f,0x6f,0x00,
				       0x3f,0x00,0x00,0x12};

void delay(u16 i)
{
  while (i--);
}

void F2(u16 m)
   {
    u16 i; 
    for(i=0;i<=m;i++)
	{
	beep=!beep;
	delay(36);
	}
   }

void B1(u16 m)
   {
    u16 i; 
    for(i=0;i<=m;i++)
	{
	beep=!beep;
	delay(51);
	}
   }

void KeyDown()
{
  u8 x;
  u8 y;
  u8 a=0;
  GPIO_KEY=0x0f;
  if(GPIO_KEY!=0x0f)
  {
    delay(1000);
	if(GPIO_KEY!=0x0f)
	{
	  GPIO_KEY=0x0f;
	  switch(GPIO_KEY)
	  {
	    case 0x07 : n=0; break;
		case 0x0b : n=1; break;
		case 0x0d : n=2; break;
		case 0x0e : n=3; break;
	  }
	  GPIO_KEY=0xf0;
	  switch(GPIO_KEY)
	  {
	    case 0x70 : m=0; break;
		case 0xb0 : m=1; break;
		case 0xd0 : m=2; break;
		case 0xe0 : m=3; break;
	  }
	  while((a<50)&&(GPIO_KEY!=0xf0))
	  {
		delay(1000);
		a++;
	  }	
	  for(x=0;x<4;x++)
	  {
	    for(y=0;y<4;y++)
		{
		  switch(smgduan[m][n])
		  {
		    case 0x06 : num=1;break;
			case 0x5b : num=2;break;
			case 0x4f : num=3;break;
			case 0x66 : num=4;break;
			case 0x6d : num=5;break;
			case 0x7d : num=6;break;
			case 0x07 : num=7;break;
			case 0x7f : num=8;break;
			case 0x6f : num=9;break;
			case 0x3f : num=0;break;
			case 0x40 : num='-';break;
			case 0x12 : num='=';break;

		  }
		}
	  }
	}
  }
  GPIO_KEY=0x0f;
}


void main()
{
      u8 fu;
	  u8 a;
	  u8 b;
	  u8 sum;
	  u16 c;
      while(1)
	  {	
	    LSA=0;
		LSB=0;
		LSC=0;
		GPIO_DIG=0x00;
	    GPIO_KEY=0x0f;
		delay(500);
	    while(GPIO_KEY==0x0f);
	    KeyDown();
		a=num;
		GPIO_DIG=smgduan[m][n];
		F2(500);
		
		GPIO_KEY=0x0f;
		delay(500);
	    while(GPIO_KEY==0x0f);
		KeyDown();
		fu=num;
		GPIO_DIG=smgduan[m][n];
		F2(500);

		GPIO_KEY=0x0f;
		delay(500);
	    while(GPIO_KEY==0x0f);
		KeyDown();
		b=num;
		GPIO_DIG=smgduan[m][n];
		F2(500);
		
		GPIO_KEY=0x0f;
		delay(500);
	    //while(GPIO_KEY==0x0f);
		

		if(fu=='-')
		{
		  sum=a-b;
		  
		  switch (sum)
		  {
		    case 1 : c=0x06;break;
			case 2 : c=0x5b;break;
			case 3 : c=0x4f;break;
			case 4 : c=0x66;break;
			case 5 : c=0x6d;break;
			case 6 : c=0x7d;break;
			case 7 : c=0x07;break;
			case 8 : c=0x7f;break;
			case 9 : c=0x6f;break;
			case 0 : c=0x3f;break;
		  }
		  delay(50000);
		  while(GPIO_KEY==0x0f);
		  KeyDown();
		  while(num!='=');
		  
		  GPIO_DIG=c;
	      F2(500);
		    
		  delay(50000);
		  delay(50000);
		  delay(50000);
		  
		  KeyDown();
		  while(GPIO_KEY==0x0f);
		  GPIO_KEY=0x00;
		  B1(500);
		  //delay (50000);
		  
		}
		
	  }
}