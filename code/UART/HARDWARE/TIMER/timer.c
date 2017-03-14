#include "timer.h"
#include "pi.h"
#include "pwm.h"
#include "dma.h"


void senddata(void);
//////////////////////////////////////////////////////////////////////////////////   	 
//ͨ�ö�ʱ��6�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx					 
}

void TIM1_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx					 
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
void TIM1_UP_IRQHandler(void)   //TIM3�ж�
{
	u8 i;
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
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
//�������ݵ���λ��
void senddata(void){
	//if(Txd4_Flag) 
		//return;
	static float a=0;
	static int b=0;
	static u8 sendData[62]= {0xA5,0x5A};//			2byte
	u8 *pd = &sendData[2];
	a++;
	b++;
	u8 *p = (u8 *)&Parm.qInRef;//Parm.qInRef; //���ٶ�ԭʼ����//		2 + 4 * 3 = 14byte            3 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	p = (u8 *)&Parm.qInMeas;
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	p = (u8 *)&Parm.qdSum;
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	float temp = (float)0;
	p = (u8 *)&temp;//Parm.qInRef; //���ٶ�ԭʼ����//		2 + 4 * 3 = 14byte            3 float
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	for(u8 n = 0;n <4;n++)
		*pd++ = *(p + n);
	
	//�˲���Ļ��� ��Ҫ  * ( 180 / PI) ����λ��ת��
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
	
}

u16 t500mscnt,t500msflag;
vu32 croud;
extern vu16 speed;
extern vu32 roud;
u8 startcnt;
extern u8 start;
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
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

void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
		
	}
}


void settime (u32 * buf){
	u32 t = 0;
	t |= TIM7->CNT; 
	*buf = t;
}









