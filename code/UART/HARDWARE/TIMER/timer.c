#include "timer.h"
#include "pi.h"
#include "pwm.h"
#include "dma.h"


void senddata(void);
//////////////////////////////////////////////////////////////////////////////////   	 
//通用定时器6中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM4, ENABLE);  //使能TIMx					 
}

void TIM1_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM1, ENABLE);  //使能TIMx					 
}


#define MinSpeed 2000
extern volatile u32 roud;
volatile u32 oldroud;
volatile u32 newroud;
volatile u32 droud;
volatile u32 Loldtime,Lnewtime;
volatile u32 Ltime;
extern volatile u16 setspeed,speed;
extern tPIParm Parm;
u8 cnt;
u8 startflag;
u16 speedbuf[10],speedcnt;
void TIM1_UP_IRQHandler(void)   //TIM3中断
{
	u8 i;
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx更新中断标志 
		
		Loldtime = Lnewtime;
		Lnewtime = TIM4->CNT;
		
//		oldroud = newroud;
//		newroud = roud;
//		
//		if(oldroud < newroud)
//			droud = newroud - oldroud;
//		else
//			droud = 0XFFFFFFFF - oldroud + newroud +1;
//		
//		speed = 1000 / droud * 60;

			
		if(Loldtime < Lnewtime){
			Ltime = Lnewtime - Loldtime;
		}else{
			Ltime = (0xFFFF - Loldtime) + Lnewtime + 1;
		}
//		speedcnt++;
//		if(speedcnt==10){
//			speedcnt = 0;
//		}
//		for(i=0;i<10;i++){
//			if(!speedbuf[i]
//		}
		speedbuf[speedcnt] =
		Parm.qInRef = setspeed;
		Parm.qInMeas = speed;
		if(startflag)
			CalcPI(&Parm);
		if(setspeed ==0){
			Parm.qdSum = 0;
			Parm.qOut = 1100;
		}
		if(startflag)
		TIM_SetTIM3Compare4(Parm.qOut);
		cnt++;
		if((cnt %2)==0)
			senddata();
	}
}
//发送数据到上位机
void senddata(void){
	//if(Txd4_Flag) 
		//return;
	static float a=0;
	static int b=0;
	static u8 sendData[62]= {0xA5,0x5A};//			2byte
	u8 *pd = &sendData[2];
	a++;
	b++;
	u8 *p = (u8 *)&Parm.qInRef;//Parm.qInRef; //加速度原始数据//		2 + 4 * 3 = 14byte            3 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	p = (u8 *)&Parm.qInMeas;
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	p = (u8 *)&Parm.qdSum;
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	float temp = (float)0;
	p = (u8 *)&temp;//Parm.qInRef; //加速度原始数据//		2 + 4 * 3 = 14byte            3 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	
	//滤波后的弧度 需要  * ( 180 / PI) 在上位机转换
	temp = (float)speed;
	p = (u8 *)&temp;//Parm.qOut;// 				26 + 4 = 30byte				  3 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	//temp = (float)setspeed;
	p = (u8 *)&Parm.qInRef;//				30 + 4 = 34byte
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	//temp = (float)(b);
	p = (u8 *)&Parm.qOut;//					34 + 4 = 38byte
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	
	//float usePerf = GetIdlePerf();									
	p = (u8 *)&a;//Parm.qdSum;						//		38 + 4 = 42byte				  1 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	temp = (float)(0);
	p = (u8 *)&temp;						//		42 + 4 = 46byte				  1 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	temp = (float)(0);
	p = (u8 *)&temp;						//		46 + 4 = 50byte				  1 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	temp = (float)(0);
	p = (u8 *)&temp;				    	//		50 + 4 = 54byte				  1 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	temp = (float)(0);
	p = (u8 *)&temp;				    	//		54 + 4 = 58byte				  1 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	temp = (float)(0);
	p = (u8 *)&temp;				    	//		58 + 4 = 62byte				  1 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	MYDMA_Enable(DMA1_Channel4,(u32)sendData,62);
	//USART4_Send((u8 *)&sendData ,62);
}



void TIM2_Int_Init(u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 
	
}

u16 t500mscnt,t500msflag;
vu32 croud;
extern vu16 speed;
extern vu32 roud;
u8 startcnt;
extern u8 start;
void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		if(t500mscnt < 200){
			t500mscnt++;
		}else{
			if(croud == roud){
				speed = 0;
			}
			croud = roud;
			if(start){
				if(startcnt < 2){
					startcnt++;
					TIM_SetTIM3Compare4(1145);
				}else{
					startflag = 1;
				}
			}else{
				startcnt = 0;
				startflag = 0;
				TIM_SetTIM3Compare4(1100);
			}
			t500mscnt = 0;
		}
	}
}

void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
		
		
	}
}


void settime (u32 * buf){
	u32 t = 0;
	t |= TIM7->CNT; 
	*buf = t;
}









