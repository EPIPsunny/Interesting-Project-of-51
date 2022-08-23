#include<reg51.h>
#include<intrins.h>
typedef unsigned char u8;
typedef unsigned int u16;

sbit SRCLK=P3^6;
sbit RCLK=P3^5;
sbit SER=P3^4;

u8 ledduan[]={0x46,0x8a,0x72,0x00,0xfe,0x92,0x92,0x6c};  //D口
u8 ledwei[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe}; //P口

void delay(u16 i)
{
  while(i--);
}
	

void Hc595SendByte(u8 dat)
{
   u8 a;
   SRCLK=0;
   RCLK=0;
   for(a=0;a<8;a++)
   {
     SER=dat>>7;		//右移7位剩最高位	 1     0011001
	 dat<<=1;           //再左移一位1001 1001 变成   0011 0010
	 SRCLK=1;
	 _nop_();			//#include<intrins.h>头文件中的延时周期
	 _nop_();
	 SRCLK=0;  
   }
   RCLK=1;
   _nop_();
   _nop_();
   RCLK=0;
}


void main()
{
	  u8 i;
	  while(1)
	   {
	     P0=0x7f;
	   	 for(i=0;i<8;i++)
		 {
		   P0=ledwei[i];
		   Hc595SendByte(ledduan[i]);
		   delay(100);
		   Hc595SendByte(0x00);
		 }
	   }
	   

}