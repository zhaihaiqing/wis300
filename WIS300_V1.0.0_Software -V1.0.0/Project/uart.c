/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"

#define UART_FLAG_TimeOut  0x50000   //超时常量

unsigned char Uart1Flag;
UART1_RBUF_ST	uart1_rbuf	=	{ 0, 0, };

//unsigned char Uart2Err;
//UART2_RBUF_ST	uart2_rbuf	=	{ 0, 0, };

unsigned char Uart2Flag;
UART2_RBUF_ST	uart2_rbuf	=	{ 0, 0, };


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**********************************************************************************
* Function Name  : UART1_Configuration
* 串口1初始化
* 入口参数
* baudrate:波特率
**********************************************************************************/
void UART1_Configuration(unsigned int baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能串口1时钟
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//IO连接外设
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//初始化TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//初始化RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//初始化串口
	USART_Init(USART1, &USART_InitStructure);
	//使能
	USART_Cmd(USART1, ENABLE);
	//中断使能
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
/**********************************************************************************
* Function Name  : UART3_Configuration
* 串口2初始化
* 入口参数
* baudrate:波特率
**********************************************************************************/
void UART2_Configuration(unsigned int baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_Init(USART2, &USART_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

/**********************************************************************************
* Function Name  : NVIC_UART_Configuration
* 串口中断配置
**********************************************************************************/
void NVIC_UART_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
	/* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**********************************************************************************
* Function Name  : USART_PutChar
* 串口发送一个字符
* 入口参数
* USARTx:串口号
* ch:数据
**********************************************************************************/
void USART_PutChar(USART_TypeDef* USARTx,unsigned char ch)
{
	unsigned short int timeout = 0;
  USART_SendData(USARTx, (uint8_t) ch);
  while((USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) && (timeout++ < UART_FLAG_TimeOut));
}
/**********************************************************************************
* Function Name  : USART_PutData
* 串口发送若干个字符
* 入口参数
* USARTx:串口号
* ch:数据
* len:数据长度
**********************************************************************************/
void USART_PutData(USART_TypeDef* USARTx,unsigned char *dat,unsigned short int len)
{
	unsigned short int i;
	for(i = 0;i < len;i++)USART_PutChar(USARTx,(uint8_t)* (dat++));
}
/**********************************************************************************
* Function Name  : USART_PutS
* 串口发送字符串
* 入口参数
* USARTx:串口号
* *s:数据指针
**********************************************************************************/
void USART_PutS(USART_TypeDef* USARTx,unsigned char *s)
{
	while(*s != '\0')USART_PutChar(USARTx,*(s++));
}
/**********************************************************************************
* 串口1接收字符函数，阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART1_GetCharBlock(uint16_t timeout)
{
	UART1_RBUF_ST *p = &uart1_rbuf;
	uint16_t to = timeout;	
	while(((p->out - p->in)& (UART1_RBUF_SIZE - 1)) == 0)if(!(--to))return TIMEOUT;
	return (p->buf [(p->out++) & (UART1_RBUF_SIZE - 1)]);
}

/**********************************************************************************
* 串口1接收字符函数，非阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART1_GetChar(void)
{
	UART1_RBUF_ST *p = &uart1_rbuf;	
#ifdef WatchDog_EN
	IWDG_ReloadCounter();//喂狗
#endif		
	if(((p->out - p->in) & (UART1_RBUF_SIZE - 1)) == 0) //缓冲区空条件
		return EMPTY;
	return USART1_GetCharBlock(1000);
}
/**********************************************************************************
* 串口2接收字符函数，阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART2_GetCharBlock(uint16_t timeout)
{
	UART2_RBUF_ST *p = &uart2_rbuf;
	uint16_t to = timeout;
	while(((p->out - p->in)& (UART2_RBUF_SIZE - 1)) == 0)if(!(--to))return TIMEOUT;
	return (p->buf [(p->out++) & (UART2_RBUF_SIZE - 1)]);
}

/**********************************************************************************
* 串口2接收字符函数，非阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART2_GetChar(void)
{
	UART2_RBUF_ST *p = &uart2_rbuf;		
#ifdef WatchDog_EN
	IWDG_ReloadCounter();//喂狗
#endif
	if(((p->out - p->in) & (UART2_RBUF_SIZE - 1)) == 0) //缓冲区空条件
		return EMPTY;
	return USART2_GetCharBlock(1000);
}
/**********************************************************************************
printf功能定义，(括号中表示从哪个串口输出数据及数据类型)
**********************************************************************************/
PUTCHAR_PROTOTYPE
{
	USART_PutChar(USART2,(uint8_t) ch);
	return ch;
}


