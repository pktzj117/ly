#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
	x = x; 
	return 0;
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#define X25_INIT_CRC 0xffff
#define X25_VALIDATE_CRC 0xf0b8
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

  //Usart1 NVIC 配置

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 

}
uint8_t MAVLINK_MESSAGE_CRCS[] =  
{50,124,137,0,237,217,104,119,0,0,0,89,0,0,0,0,0,0,0,0,214,
	159,220,168,24,23,170,144,67,115,39,246,185,104,237,244,222,
	212,9,254,230,28,28,132,221,232,11,153,41,39,214,223,141,33,
	15,3,100,24,239,238,30,240,183,130,130,118,148,21,0,243,124,
	0,0,0,20,0,152,143,0,0,127,106,147,211,198,0,0,0,0,231,183,
	63,54,0,0,0,0,0,0,0,175,102,158,208,56,93,211,108,32,185,235,
	93,124,124,119,4,76,128,56,116,134,237,203,250,87,203,220,25,
	226,0,29,223,85,6,229,203,1,0,0,0,0,0,0,0,0,0,0,177,241,15,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,204,49,170,44,83,46,0};		

	/**
 * @brief Accumulate the X.25 CRC by adding one char at a time.
 *
 * The checksum function adds the hash of one char at a time to the
 * 16 bit checksum (uint16_t).
 *
 * @param data new char to hash
 * @param crcAccum the already accumulated checksum
 **/
void crc_accumulate(uint8_t data, uint16_t *crcAccum)
{
        /*Accumulate one byte of data into the CRC*/
        uint8_t tmp;

        tmp = data ^ (uint8_t)(*crcAccum &0xff);
        tmp ^= (tmp<<4);
        *crcAccum = (*crcAccum>>8) ^ (tmp<<8) ^ (tmp <<3) ^ (tmp>>4);
}

/**
 * @brief Initiliaze the buffer for the X.25 CRC
 *
 * @param crcAccum the 16 bit X.25 CRC
 */
void crc_init(uint16_t* crcAccum)
{
        *crcAccum = X25_INIT_CRC;
}


/**
 * @brief Calculates the X.25 checksum on a byte buffer
 *
 * @param  pBuffer buffer containing the byte array to hash
 * @param  length  length of the byte array
 * @return the checksum over the buffer bytes
 **/
uint16_t crc_calculate(const uint8_t* pBuffer, uint16_t length)
{
        uint16_t crcTmp;
        crc_init(&crcTmp);
	while (length--) {
                crc_accumulate(*pBuffer++, &crcTmp);
        }
        return crcTmp;
}

///**
// * @brief Accumulate the X.25 CRC by adding an array of bytes
// *
// * The checksum function adds the hash of one char at a time to the
// * 16 bit checksum (uint16_t).
// *
// * @param data new bytes to hash
// * @param crcAccum the already accumulated checksum
// **/
//static inline void crc_accumulate_buffer(uint16_t *crcAccum, const char *pBuffer, uint8_t length)
//{
//	const uint8_t *p = (const uint8_t *)pBuffer;
//	while (length--) {
//                crc_accumulate(*p++, crcAccum);
//        }
//}

uint8_t recntmax=128;
uint8_t RX[200],RXbuff[200];
uint8_t rx_conut,flag_RX,RX_LEN;
uint16_t checksum;
uint16_t rechecksum;
uint8_t ID;
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		RX[rx_conut] = USART1->DR;   
		if((rx_conut==0)&&(RX[0]!=0xFE))  
		{
			rx_conut=0; 
			return;
		}
		if(rx_conut==1)
			recntmax=RX[rx_conut]+8; 

		rx_conut++;		
	
		if( rx_conut==recntmax)
		{
			
			checksum = crc_calculate(&RX[1],RX[1] + 5);//RX[1]存储的是命令长度 RX[1] + 5 内容的开始
			crc_accumulate(MAVLINK_MESSAGE_CRCS[RX[5]],(uint16_t *)&checksum);   //根据MAVLINK_MESSAGE_CRCS数组得到值后再计算一次CRC值
			
			rechecksum = *(uint16_t *)(&RX[recntmax -2]);
//			if(RX[5] ==0XE1){
//					ID = RX[5];
//				for(uint8_t i=0;i<=recntmax;i++)
//				{
//					RXbuff[i]=RX[i];
//					//printf("%X",RXbuff[i]);
//				}
//				flag_RX = 1;
//			}
//			if(RX[5] ==0X46){
//					ID = RX[5];
//			}
//			if(RX[5] ==0X00){
//					ID = RX[5];
//			}
//			if(RX[5] ==0X42){
//					ID = RX[5];
//			}
			if(rechecksum == checksum){
				for(uint8_t i=0;i<=recntmax;i++)
				{
					RXbuff[i]=RX[i];
					//printf("%X",RXbuff[i]);
				}
				
				flag_RX = 1;
			}
			RX_LEN = recntmax;
			rx_conut = 0;
	
			
			//OSSemPost(msg_uart);
				
		}
	}
}


