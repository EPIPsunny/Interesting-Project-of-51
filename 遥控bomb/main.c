#include <reg51.h>			 //此文件中定义了单片机的一些特殊功能寄存器
#include<BOMB.h>
#include <intrins.h>	

typedef  int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;



sbit IRIN=P3^2;

u8 IrValue[6];
u8 Time;

u8 DisplayData[8];




void wait(u16 i)
{
	while(i--);	
} 



void IrInit()
{
	IT0=1;//下降沿触发
	EX0=1;//打开中断0允许
	EA=1;	//打开总中断

	IRIN=1;//初始化端口
}



void main()
{	
	IrInit();
	IrValue[2]=0x00;
	while(1)
	{	
		if(IrValue[2]==0x45)
		{
		   BOMB();
		}
		
	}		
}



void ReadIr() interrupt 0
{
	u8 j,k;
	u16 err;
	Time=0;					 
	wait(700);	//7ms
	if(IRIN==0)		//确认是否真的接收到正确的信号
	{	 
		
		err=1000;				//1000*10us=10ms,超过说明接收到错误的信号
		/*当两个条件都为真是循环，如果有一个条件为假的时候跳出循环，免得程序出错的时
		侯，程序死在这里*/	
		while((IRIN==0)&&(err>0))	//等待前面9ms的低电平过去  		
		{			
			wait(1);
			err--;
		} 
		if(IRIN==1)			//如果正确等到9ms低电平
		{
			err=500;
			while((IRIN==1)&&(err>0))		 //等待4.5ms的起始高电平过去
			{
				wait(1);
				err--;
			}
			for(k=0;k<4;k++)		//共有4组数据
			{				
				for(j=0;j<8;j++)	//接收一组数据
				{

					err=60;		
					while((IRIN==0)&&(err>0))//等待信号前面的560us低电平过去
					{
						wait(1);
						err--;
					}
					err=500;
					while((IRIN==1)&&(err>0))	 //计算高电平的时间长度。
					{
						wait(10);	 //0.1ms
						Time++;
						err--;
						if(Time>30)
						{
							return;
						}
					}
					IrValue[k]>>=1;	 //k表示第几组数据
					if(Time>=8)			//如果高电平出现大于565us，那么是1
					{
						IrValue[k]|=0x80;
					}
					Time=0;		//用完时间要重新赋值							
				}
			}
		}
		if(IrValue[2]!=~IrValue[3])
		{
			return;
		}
	}			
}