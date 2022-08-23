#include<reg51.h>
typedef unsigned char u8;
typedef unsigned int u16;
 u8 m;
 u8 n;
 u8 x;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u8 smgduan[]={      0x3f  , 0x06 , 0x5b , 0x4f , 0x66 , 0x6d , 	//0,1,2,3,4,5,
                    0x7d , 0x07 , 0x7f  , 0x6f ,0x00,			   //6,7,8,9,
				    0x77 , 0x7c ,0x39 , 0x5e , 0x79 , 0x71  };

u8 smgbuan[]={      0x00,  0x06 , 0x5b , 0x4f , 0x66 , 0x6d , 	//0,1,2,3,4,5,
                    0x7d , 0x07 , 0x7f  , 0x6f ,0x00,			   //6,7,8,9,
				    0x77 , 0x7c ,0x39 , 0x5e , 0x79 , 0x71  };

u8 output[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
void delay(u16 i)
{
   while(i--);
}

void Timer0Init()
{
  TMOD=TMOD|0x01;   //寄存器配置详见ppt
  TH0=0xfc;			//高四位初值
  TL0=0x18;  		//低四位初值
  ET0=1;			//打开计数器0中断
  EA=1;				//打开总中断
  TR0=1;			//打开计数器功能
}



void main()
{
	 u16 i;
	 Timer0Init();
	 m=0;
	 n=0;
	 x=0;
	 while(1)
	 {
	 for(i=0;i<8;i++)
	  {
	  	switch(i)
		{
		   case 0:
		          LSA=0;LSB=0;LSC=0;break;
		   case 1:
		          LSA=1;LSB=0;LSC=0;break;
		   case 2:
		          LSA=0;LSB=1;LSC=0;break;
		   case 3:
		          LSA=1;LSB=1;LSC=0;break;
		   case 4:
		          LSA=0;LSB=0;LSC=1;break;
		   case 5:
		          LSA=1;LSB=0;LSC=1;break;
		   case 6:
		          LSA=0;LSB=1;LSC=1;break;
		   case 7:
		          LSA=1;LSB=1;LSC=1;break;
		}
		output[0]=smgduan[m];
		output[1]=smgbuan[n];
		P0=output[i];
		delay(100);
		P0=0x00;
		
	  }
	}
}

void Time0() interrupt 1
{
  static u16 i; //静态变量
  TH0=0xfc;
  TL0=0x18;
  i++;
  if(m==10)
  {
    m=0;
	n++;
  }
  if(i==1000)	 //一秒
  {
    i=0;
	m++;
	
  }

}
	     

	   

