


#include "main.h"

/**********************************************
* Function Name  : EXTI15_10_Config
* Description    : ADS1248����׼�����ж����ã�ʹ��PA11��Ϊ�ⲿ�ж�Դ���½��ش����ж�
* Input          : None
* Output         : None
* Return         : None
*STM32�У�ÿһ��GPIO�����Դ���һ���ⲿ�жϣ����ǣ�GPIO���ж��ǿ�������Ϊһ����λ��
*ͬ�����ⲿ�ж�ͬһʱ��ֻ��ʹ��һ�������磬PA0��PB0��PC0��PD0��PE0��PF0��PG0��Щ
*Ϊһ�飬�������ʹ����PA0��Ϊ�ⲿ�жϣ���ô��ľͲ�����ʹ���ˣ�ֻ��ʹ������PA1��PB2
*����ĩ����Ų�ͬ���ⲿ�ж�Դ��ÿһ��ʹ��һ���жϱ�־EXTIx��EXTI0-EXTI4������ⲿ�ж�
*�����Լ��������ж���Ӧ������EXTI5-EXTI9����һ���ж���Ӧ������EXTI10-EXTI15����һ����

����ͬ����ռʽ���ȼ����жϲ���ִ���ж�Ƕ�ף�����ռʽ���ȼ��Ŀ�����ռ�����ȼ��ĳ���

**********************************************/
void EXTI0_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Configure PB0 pin as input floating */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;																//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;														//
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;												//
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;												//
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//STM32ֻʹ���˸���λ��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//STM32ֻʹ���˸���λ��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	D_Ch_STOP;				//����Aͨ���ж�
}

void EXTI1_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Configure PB0 pin as input floating */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;																//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;															//
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;													//
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;													//
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	C_Ch_STOP;
}



void EXTI2_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Configure PB0 pin as input floating */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;																//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;															//
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;													//
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;													//
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	B_Ch_STOP;
}


void EXTI15_10_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Configure PB10 pin as input floating */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;																//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;															//
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;													//
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;													//
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI10 Line to PB10 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);

  /* Configure EXTI10 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line10;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI10 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	A_Ch_STOP;
}









