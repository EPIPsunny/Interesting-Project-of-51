
//黑线贴3厘米宽，否则小车摆动很厉害	  上18  左08 右5A  后1C  音乐47  模式46
#include <reg52.h>//51头文件
#include <QX_A11.h>//QX_A11智能小车配置文件
#include <intrins.h>
typedef  int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
unsigned char	pwm_val_left,pwm_val_right;	//中间变量，用户请勿修改。
unsigned char 	pwm_left,pwm_right;			//定义PWM输出高电平的时间的变量。用户操作PWM的变量。
#define		PWM_DUTY		200			//定义PWM的周期，数值为定时器0溢出周期，假如定时器溢出时间为100us，则PWM周期为20ms。
#define		PWM_HIGH_MIN	70			//限制PWM输出的最小占空比。用户请勿修改。
#define		PWM_HIGH_MAX	PWM_DUTY	//限制PWM输出的最大占空比。用户请勿修改。

void Timer0_Init(void); //定时器0初始化
void LoadPWM(void);//装入PWM输出值 
void forward(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11智能小车前进 
void left_run(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11智能小车左转  
void right_run(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11智能小车右转
void stop(void);//QX_A11智能小车停车
void back(unsigned char LeftSpeed,unsigned char RightSpeed);

u8 IrValue[6];
u8 Time;

u8 DisplayData[8];


void wait(u16 i)
{
	while(i--);	
}
void delay(u16 i)
{
	while(i--);	
}

void IrInit()    //红外通信中断允许
{
	IT0=1;//下降沿触发
	EX0=1;//打开中断0允许
	EA=1;	//打开总中断

	IRIN=1;//初始化端口
}
void Int1Init()	   //以外部中断1为例 ,检测P3^2口
{
  IT1=1;      //设置外部中断触发方式：选择下降沿触发，若为0则为低电平触发
  EX1=1;      //打开外部中断1
  EA=1;		  //打开CPU总中断

}


/*void Tracking()
{
	//为0 没有识别到黑线 为1识别到黑线
	char data1,data2 = left_led1,data3 = right_led1;
	data1 = data2*10+data3;
	if(data1 == 11)//在黑线上，前进
	{
		forward(120,120);//前进
	}
	else
	{
	 	if(data1 == 10)//小幅偏右，左转
		{
			left_run(80,160);//左转
		}
		if(data1 == 1)//小幅偏左，右转
		{
			right_run(160,80);//右转
		}
		if(data1 == 0)//大幅偏左或偏右，已脱离轨道
		{
			stop();	
		}
	}
} */

/*主函数*/     
void main(void)
{
    u16 m;
	u16 i;
	u8 rem;
	u8 symbol=0;
	u8 lowspeed=90;
	u8 highspeed=180;
	Timer0_Init();//定时0初始化
	IrInit();
	Int1Init();
	IrValue[2]=0x00;
	
	while(1)
	{
		
		 if(IrValue[2]==0x46)
		  {
		    symbol=!symbol;
		    rem=IrValue[2];
		    LED1=!LED1;
			IrValue[2]=0x00;
			delay(50000);
		  }
		
		if(IrValue[2]==0x18)
		{
		   if(symbol==0)
		   {
		     forward(lowspeed,lowspeed);
		   }
		   if(symbol==1)
		   {
		     forward(highspeed,highspeed);
		   }
		   rem=IrValue[2];
		   wait(5000);//前进
		}
		else if(IrValue[2]==0x08)
		{
		   if(symbol==0)
		   {
		     left_run(lowspeed,lowspeed);
		   }
		   if(symbol==1)
		   {
		     left_run(highspeed,highspeed);
		   }
		   rem=IrValue[2];
		}
		else if(IrValue[2]==0x5a)
		{
		   if(symbol==0)
		   {
		     right_run(lowspeed,lowspeed);
		   }
		   if(symbol==1)
		   {
		     right_run(highspeed,highspeed);
		   }
		   rem=IrValue[2];
		   wait(5000);
		}
		else if(IrValue[2]==0x1c)
		{
		   if(symbol==0)
		   {
		     back(lowspeed,lowspeed);
		   }
		   if(symbol==1)
		   {
		     back(highspeed,highspeed);
		   }
		   rem=IrValue[2];
		   wait(5000);
		}
		else if(IrValue[2]==0x45) 
		{
		   stop();
		   wait(5000);
		   rem=IrValue[2];
		}

		else if(IrValue[2]==0x47) 
		{
		   KEY=0;
		   
		   wait(5000);
		   KEY=1;
		   LED2=!LED2;
		   IrValue[2]=0x00;
		   
		}

		
	}	
}

void Int1() interrupt 2		//当检测到P3^3口下降沿触发
{
  u16 num=300,i;
  delay(1000);			   //消抖
  if(KEY==0)
  {

    for(i=0;i<num;i++)
	{
	  beep=!beep;
	  delay(50);
	  beep=!beep;
	  delay(50);
	}
      
  }

}


/*********************************************
QX_A11智能小车前进
入口参数：LeftSpeed，RightSpeed
出口参数: 无
说明：LeftSpeed，RightSpeed分别设置左右车轮转速
**********************************************/
void forward(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//设置速度
	left_motor_go; //左电机前进
	right_motor_go; //右电机前进
}
/*小车左转*/
/*********************************************
QX_A11智能小车左转
入口参数：LeftSpeed，RightSpeed
出口参数: 无
说明：LeftSpeed，RightSpeed分别设置左右车轮转速
**********************************************/
void left_run(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//设置速度
	left_motor_back; //左电机后退
	right_motor_go; //右电机前进	
}

/*********************************************
QX_A11智能小车右转
入口参数：LeftSpeed，RightSpeed
出口参数: 无
说明：LeftSpeed，RightSpeed分别设置左右车轮转速
**********************************************/
void right_run(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//设置速度
	right_motor_back;//右电机后退
	left_motor_go;    //左电机前进
}
/*********************************************
QX_A11智能小车停车
入口参数：无
出口参数: 无
说明：QX_A11智能小车停车
**********************************************/
void stop(void)
{
	left_motor_stops;
	right_motor_stops;
}
void back(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//设置速度
	right_motor_back;//右电机后退
	left_motor_back;    //左电机前进
}
/*********************************************
QX_A11智能小车PWM输出
入口参数：无
出口参数: 无
说明：装载PWM输出,如果设置全局变量pwm_left,pwm_right分别配置左右输出高电平时间
**********************************************/
void LoadPWM(void)
{
	if(pwm_left > PWM_HIGH_MAX)		pwm_left = PWM_HIGH_MAX;	//如果左输出写入大于最大占空比数据，则强制为最大占空比。
	if(pwm_left < PWM_HIGH_MIN)		pwm_left = PWM_HIGH_MIN;	//如果左输出写入小于最小占空比数据，则强制为最小占空比。
	if(pwm_right > PWM_HIGH_MAX)	pwm_right = PWM_HIGH_MAX;	//如果右输出写入大于最大占空比数据，则强制为最大占空比。
	if(pwm_right < PWM_HIGH_MIN)	pwm_right = PWM_HIGH_MIN;	//如果右输出写入小于最小占空比数据，则强制为最小占空比。
	if(pwm_val_left<=pwm_left)		Left_moto_pwm = 1;  //装载左PWM输出高电平时间
	else Left_moto_pwm = 0; 						    //装载左PWM输出低电平时间
	if(pwm_val_left>=PWM_DUTY)		pwm_val_left = 0;	//如果左对比值大于等于最大占空比数据，则为零

	if(pwm_val_right<=pwm_right)	Right_moto_pwm = 1; //装载右PWM输出高电平时间
	else Right_moto_pwm = 0; 							//装载右PWM输出低电平时间
	if(pwm_val_right>=PWM_DUTY)		pwm_val_right = 0;	//如果右对比值大于等于最大占空比数据，则为零
}
/********************* Timer0初始化************************/
void Timer0_Init(void)
{
	TMOD |= 0x02;//定时器0，8位自动重装模块
	TH0 = 164;
	TL0 = 164;//11.0592M晶振，12T溢出时间约等于100微秒
	TR0 = 1;//启动定时器0
	ET0 = 1;//允许定时器0中断
	EA	= 1;//总中断允许	
}
 
/********************* Timer0中断函数************************/
void timer0_int (void) interrupt 1
{
	 pwm_val_left++;
	 pwm_val_right++;
	 LoadPWM();//装载PWM输出
}

  


void ReadIr() interrupt 0	//红外中断程序
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
