
//������3���׿�����С���ڶ�������	  ��18  ��08 ��5A  ��1C  ����47  ģʽ46
#include <reg52.h>//51ͷ�ļ�
#include <QX_A11.h>//QX_A11����С�������ļ�
#include <intrins.h>
typedef  int u16;	  //���������ͽ�����������
typedef unsigned char u8;
unsigned char	pwm_val_left,pwm_val_right;	//�м�������û������޸ġ�
unsigned char 	pwm_left,pwm_right;			//����PWM����ߵ�ƽ��ʱ��ı������û�����PWM�ı�����
#define		PWM_DUTY		200			//����PWM�����ڣ���ֵΪ��ʱ��0������ڣ����綨ʱ�����ʱ��Ϊ100us����PWM����Ϊ20ms��
#define		PWM_HIGH_MIN	70			//����PWM�������Сռ�ձȡ��û������޸ġ�
#define		PWM_HIGH_MAX	PWM_DUTY	//����PWM��������ռ�ձȡ��û������޸ġ�

void Timer0_Init(void); //��ʱ��0��ʼ��
void LoadPWM(void);//װ��PWM���ֵ 
void forward(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11����С��ǰ�� 
void left_run(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11����С����ת  
void right_run(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11����С����ת
void stop(void);//QX_A11����С��ͣ��
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

void IrInit()    //����ͨ���ж�����
{
	IT0=1;//�½��ش���
	EX0=1;//���ж�0����
	EA=1;	//�����ж�

	IRIN=1;//��ʼ���˿�
}
void Int1Init()	   //���ⲿ�ж�1Ϊ�� ,���P3^2��
{
  IT1=1;      //�����ⲿ�жϴ�����ʽ��ѡ���½��ش�������Ϊ0��Ϊ�͵�ƽ����
  EX1=1;      //���ⲿ�ж�1
  EA=1;		  //��CPU���ж�

}


/*void Tracking()
{
	//Ϊ0 û��ʶ�𵽺��� Ϊ1ʶ�𵽺���
	char data1,data2 = left_led1,data3 = right_led1;
	data1 = data2*10+data3;
	if(data1 == 11)//�ں����ϣ�ǰ��
	{
		forward(120,120);//ǰ��
	}
	else
	{
	 	if(data1 == 10)//С��ƫ�ң���ת
		{
			left_run(80,160);//��ת
		}
		if(data1 == 1)//С��ƫ����ת
		{
			right_run(160,80);//��ת
		}
		if(data1 == 0)//���ƫ���ƫ�ң���������
		{
			stop();	
		}
	}
} */

/*������*/     
void main(void)
{
    u16 m;
	u16 i;
	u8 rem;
	u8 symbol=0;
	u8 lowspeed=90;
	u8 highspeed=180;
	Timer0_Init();//��ʱ0��ʼ��
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
		   wait(5000);//ǰ��
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

void Int1() interrupt 2		//����⵽P3^3���½��ش���
{
  u16 num=300,i;
  delay(1000);			   //����
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
QX_A11����С��ǰ��
��ڲ�����LeftSpeed��RightSpeed
���ڲ���: ��
˵����LeftSpeed��RightSpeed�ֱ��������ҳ���ת��
**********************************************/
void forward(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//�����ٶ�
	left_motor_go; //����ǰ��
	right_motor_go; //�ҵ��ǰ��
}
/*С����ת*/
/*********************************************
QX_A11����С����ת
��ڲ�����LeftSpeed��RightSpeed
���ڲ���: ��
˵����LeftSpeed��RightSpeed�ֱ��������ҳ���ת��
**********************************************/
void left_run(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//�����ٶ�
	left_motor_back; //��������
	right_motor_go; //�ҵ��ǰ��	
}

/*********************************************
QX_A11����С����ת
��ڲ�����LeftSpeed��RightSpeed
���ڲ���: ��
˵����LeftSpeed��RightSpeed�ֱ��������ҳ���ת��
**********************************************/
void right_run(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//�����ٶ�
	right_motor_back;//�ҵ������
	left_motor_go;    //����ǰ��
}
/*********************************************
QX_A11����С��ͣ��
��ڲ�������
���ڲ���: ��
˵����QX_A11����С��ͣ��
**********************************************/
void stop(void)
{
	left_motor_stops;
	right_motor_stops;
}
void back(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//�����ٶ�
	right_motor_back;//�ҵ������
	left_motor_back;    //����ǰ��
}
/*********************************************
QX_A11����С��PWM���
��ڲ�������
���ڲ���: ��
˵����װ��PWM���,�������ȫ�ֱ���pwm_left,pwm_right�ֱ�������������ߵ�ƽʱ��
**********************************************/
void LoadPWM(void)
{
	if(pwm_left > PWM_HIGH_MAX)		pwm_left = PWM_HIGH_MAX;	//��������д��������ռ�ձ����ݣ���ǿ��Ϊ���ռ�ձȡ�
	if(pwm_left < PWM_HIGH_MIN)		pwm_left = PWM_HIGH_MIN;	//��������д��С����Сռ�ձ����ݣ���ǿ��Ϊ��Сռ�ձȡ�
	if(pwm_right > PWM_HIGH_MAX)	pwm_right = PWM_HIGH_MAX;	//��������д��������ռ�ձ����ݣ���ǿ��Ϊ���ռ�ձȡ�
	if(pwm_right < PWM_HIGH_MIN)	pwm_right = PWM_HIGH_MIN;	//��������д��С����Сռ�ձ����ݣ���ǿ��Ϊ��Сռ�ձȡ�
	if(pwm_val_left<=pwm_left)		Left_moto_pwm = 1;  //װ����PWM����ߵ�ƽʱ��
	else Left_moto_pwm = 0; 						    //װ����PWM����͵�ƽʱ��
	if(pwm_val_left>=PWM_DUTY)		pwm_val_left = 0;	//�����Ա�ֵ���ڵ������ռ�ձ����ݣ���Ϊ��

	if(pwm_val_right<=pwm_right)	Right_moto_pwm = 1; //װ����PWM����ߵ�ƽʱ��
	else Right_moto_pwm = 0; 							//װ����PWM����͵�ƽʱ��
	if(pwm_val_right>=PWM_DUTY)		pwm_val_right = 0;	//����ҶԱ�ֵ���ڵ������ռ�ձ����ݣ���Ϊ��
}
/********************* Timer0��ʼ��************************/
void Timer0_Init(void)
{
	TMOD |= 0x02;//��ʱ��0��8λ�Զ���װģ��
	TH0 = 164;
	TL0 = 164;//11.0592M����12T���ʱ��Լ����100΢��
	TR0 = 1;//������ʱ��0
	ET0 = 1;//����ʱ��0�ж�
	EA	= 1;//���ж�����	
}
 
/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt 1
{
	 pwm_val_left++;
	 pwm_val_right++;
	 LoadPWM();//װ��PWM���
}

  


void ReadIr() interrupt 0	//�����жϳ���
{
	u8 j,k;
	u16 err;
	Time=0;					 
	wait(700);	//7ms
	if(IRIN==0)		//ȷ���Ƿ���Ľ��յ���ȷ���ź�
	{	 
		
		err=1000;				//1000*10us=10ms,����˵�����յ�������ź�
		/*������������Ϊ����ѭ���������һ������Ϊ�ٵ�ʱ������ѭ������ó�������ʱ
		�������������*/	
		while((IRIN==0)&&(err>0))	//�ȴ�ǰ��9ms�ĵ͵�ƽ��ȥ  		
		{			
			wait(1);
			err--;
		} 
		if(IRIN==1)			//�����ȷ�ȵ�9ms�͵�ƽ
		{
			err=500;
			while((IRIN==1)&&(err>0))		 //�ȴ�4.5ms����ʼ�ߵ�ƽ��ȥ
			{
				wait(1);
				err--;
			}
			for(k=0;k<4;k++)		//����4������
			{				
				for(j=0;j<8;j++)	//����һ������
				{

					err=60;		
					while((IRIN==0)&&(err>0))//�ȴ��ź�ǰ���560us�͵�ƽ��ȥ
					{
						wait(1);
						err--;
					}
					err=500;
					while((IRIN==1)&&(err>0))	 //����ߵ�ƽ��ʱ�䳤�ȡ�
					{
						wait(10);	 //0.1ms
						Time++;
						err--;
						if(Time>30)
						{
							return;
						}
					}
					IrValue[k]>>=1;	 //k��ʾ�ڼ�������
					if(Time>=8)			//����ߵ�ƽ���ִ���565us����ô��1
					{
						IrValue[k]|=0x80;
					}
					Time=0;		//����ʱ��Ҫ���¸�ֵ							
				}
			}
		}
		if(IrValue[2]!=~IrValue[3])
		{
			return;
		}
	}			
}    	
