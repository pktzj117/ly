#include "exti.h"
#include "delay.h"
#include "usart.h"
#include "dma.h"

void KEY_EXTI_Init(void){
	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;	//
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

}

/*
	�ɼ������������źţ������ת�٣�RPM��

*/

u32 key0count;
vu32 roud;
vu32 oldtime,newtime;
vu32 time;
vu16 speed;
vu32 errspeed;
extern u8 SendBuff[400];
void EXTI9_5_IRQHandler(void)
{
	
	if((TIM4->CNT - newtime) < 11333/2){
		errspeed++;
		EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE4�ϵ��жϱ�־λ  
		return;
	}
	if((newtime - TIM4->CNT) < 11333/2){
		errspeed++;
		EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE4�ϵ��жϱ�־λ  
		return;
	}
	roud++;
	oldtime = newtime;
	newtime = TIM4->CNT;
	
	if(oldtime < newtime){
		time = newtime - oldtime;
	}else{
		time = (0xFFFF - oldtime) + newtime + 1;
	}
	speed = 1000000 * 60 / 2 /time;
	
	//sprintf((char *)SendBuff,"speed: %d  time: %d  oldtime: %d  newtime: %d\r\n",speed,time,oldtime,newtime);
	//MYDMA_Enable(DMA1_Channel4,(u32)SendBuff,100);
	EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE4�ϵ��жϱ�־λ  
}

