
#include "delay.h"
#include "sys.h"
#include "24tft.h"
#include "usart.h"	 
#include "adc.h"
#include "pwm.h"
#include "exti.h"
#include "timer.h"
#include "dma.h"
#include "pi.h"
#include "stdio.h"
#include "key.h"
#include "MM.h"
#define MaxSpeed 4000
#define G (MaxSpeed/3.3)
#define MinSpeed 1000
#define len (1900-1100)
#define F (MaxSpeed/len)
#define SEND_BUF_SIZE 400

u16 setspeed,setspeedbuf[100];
u8 lens = 100;
u16 pwmbuf;
u8 SendBuff[400];
extern tPIParm Parm;
// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[10];



#define N 10   
  
//char value_buf[N];  
char i=0;  
 u8 startup;
uint16_t filter(__IO uint16_t* value_buf)  
{  
   char count;  
   int  sum=0;  
   //value_buf[i++] = get_ad();  
   if ( i == N )   i = 0;  
   for(count=0;count<N;count++)  
      sum += value_buf[count];  
   return (uint16_t)(sum/N);  
}  
u16 adcx;
float temp;
u16 sp;
u8 adccnt;
u8 start;
u8 key;
void getSetSpeed(void)
{
	key = KEY_Scan(0);
	switch (key){     
		case KEY1_PRES:
			if(!startup){
				setspeed = 0;
			}else{
				if(setspeed > MinSpeed)
					setspeed -= 100;
				else
					setspeed = 0;
			}
			break;
		case KEY0_PRES:
				if(setspeed == 0)
					setspeed = MinSpeed;
				else if(startup){
					if(setspeed < MaxSpeed)
						setspeed += 100;
					else
						setspeed = MaxSpeed;
				}
			break;
		default :break;
	}
	if(setspeed ==0){
		start = 0;
		startup = 0;
	}else{
		start = 1;
	}
}

void ShowSpeed(void){
	u8 buf[5]="";
	buf[0] = (u16)Parm.qInRef / 1000 % 10 + 0x30;
	buf[1] = (u16)Parm.qInRef / 100 % 10 + 0x30;
	buf[2] = (u16)Parm.qInRef / 10 % 10 + 0x30;
	buf[3] = (u16)Parm.qInRef  % 10 + 0x30;
	LCD_PutString(0,40,buf);
	
	buf[0] = (u16)Parm.qInMeas / 1000 % 10 + 0x30;
	buf[1] = (u16)Parm.qInMeas / 100 % 10 + 0x30;
	buf[2] = (u16)Parm.qInMeas / 10 % 10 + 0x30;
	buf[3] = (u16)Parm.qInMeas  % 10 + 0x30;
	LCD_PutString(0,80,buf);
}
u8 t;
int main(void)
{	 

	delay_init();	    	 //��ʱ������ʼ��	  
	InitPI(&Parm);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(921600);	 	//���ڳ�ʼ��Ϊ115200
	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���1��DMA����      
	TIM3_PWM_Init(2500,72-1);	
//  TIM_SetTIM3Compare4(1900);	
//	delay_ms(4000);
	TIM_SetTIM3Compare4(1100);
	
	//ADC1_Init();		  		//ADC��ʼ��
	TIM2_Int_Init(2000,72-1);
	TIM4_Int_Init(0xFFFF,144-1);
	TIM1_Int_Init(2000,72-1);
	KEY_EXTI_Init();    
	KEY_Init();
	sprintf((char *)SendBuff,"TEST DMA IS READ!\r\n");
	MYDMA_Enable(DMA1_Channel4,(u32)SendBuff,20);
	
	
	GPIO_Config();
	
	ILI9325_CMO24_Initial();	

	SPILCD_Clear(0x00);

	LCD_PutString(0,0,"  Liu Yang");
	LCD_PutString(0,20,"SetSpeed:RPM"); 
	LCD_PutString(0,40,":");
	LCD_PutString(0,60,"MeaSpeed:RPM");
	LCD_PutString(0,80,":");
	delay_ms(10);
	while(1)
	{
		t++;
		getSetSpeed();
		
		if((t%40) == 0 ){ //200ms 
		
			ShowSpeed();
			MYDMA_Enable(DMA1_Channel4,(u32)sendData,62);
		}
		delay_ms(10); 
		
		//������
		if(!startup){
			if((t%30)==0){
				if(start){
					if(setspeed < 3000)
						setspeed += 100;
					else{
						startup = 1;
					}
				}
				
			}
		}
//		adcx=Get_Adc_Average(ADC_Channel_1,10);
//		pwmbuf = (setspeed - 2000) / F + 1100;
//		if(pwmbuf<1100) pwmbuf= 1100;
//		TIM_SetTIM3Compare4(pwmbuf);
//		delay_ms(250);	
	}
}

