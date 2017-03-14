#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dma.h"
#include "key.h"
u8 t;						//֡ͷ ���� ��� ϵͳ ��Ԫ ���� ���� ����
u8 SendBuff[400]={0xFE,120,0,0,0,0};
u8 slen = 120;
uint16_t schecksum;
int main(void)
{ 
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,128);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����SEND_BUF_SIZE.
 
	while(1)
	{
		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //????1?DMA??        
		
		schecksum = crc_calculate(&SendBuff[1],SendBuff[1] + 5);//RX[1]�洢��������� 
		crc_accumulate(MAVLINK_MESSAGE_CRCS[SendBuff[5]],(uint16_t *)&schecksum);   //����MAVLINK_MESSAGE_CRCS����õ�ֵ���ټ���һ��CRCֵ
			
		*(uint16_t *)(&SendBuff[slen+8 -2]) = schecksum;
		if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)//�ȴ�ͨ��4�������
		{
				DMA_ClearFlag(DMA1_FLAG_TC4);//���ͨ��4������ɱ�־
		}
		MYDMA_Enable(DMA1_Channel4,(u32)SendBuff,128);
		
		delay_ms(100);
	}
}
