#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dma.h"
#include "key.h"
u8 t;						//帧头 长度 序号 系统 单元 包号 内容 编码
u8 SendBuff[400]={0xFE,120,0,0,0,0};
u8 slen = 120;
uint16_t schecksum;
int main(void)
{ 
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为9600
	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,128);//DMA1通道4,外设为串口1,存储器为SendBuff,长度SEND_BUF_SIZE.
 
	while(1)
	{
		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //????1?DMA??        
		
		schecksum = crc_calculate(&SendBuff[1],SendBuff[1] + 5);//RX[1]存储的是命令长度 
		crc_accumulate(MAVLINK_MESSAGE_CRCS[SendBuff[5]],(uint16_t *)&schecksum);   //根据MAVLINK_MESSAGE_CRCS数组得到值后再计算一次CRC值
			
		*(uint16_t *)(&SendBuff[slen+8 -2]) = schecksum;
		if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)//等待通道4传输完成
		{
				DMA_ClearFlag(DMA1_FLAG_TC4);//清除通道4传输完成标志
		}
		MYDMA_Enable(DMA1_Channel4,(u32)SendBuff,128);
		
		delay_ms(100);
	}
}
