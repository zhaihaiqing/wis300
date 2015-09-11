


#include "main.h"

/**********************************************
* Function Name  : EXTI15_10_Config
* Description    : ADS1248数据准备好中断配置，使用PA11作为外部中断源，下降沿触发中断
* Input          : None
* Output         : None
* Return         : None
*STM32中，每一个GPIO都可以触发一个外部中断，但是，GPIO的中断是可以以组为一个单位的
*同组间的外部中断同一时间只能使用一个。比如，PA0、PB0、PC0、PD0、PE0、PF0、PG0这些
*为一组，如果我们使用了PA0作为外部中断，那么别的就不能再使用了，只能使用类似PA1、PB2
*这种末端序号不同的外部中断源。每一组使用一个中断标志EXTIx，EXTI0-EXTI4这五个外部中断
*有着自己单独的中断响应函数，EXTI5-EXTI9共用一个中断响应函数，EXTI10-EXTI15共用一个。

具有同级抢占式优先级的中断不能执行中断嵌套，高抢占式优先级的可以抢占低优先级的程序

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
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//STM32只使用了高四位，
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//STM32只使用了高四位，
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	D_Ch_STOP;				//屏蔽A通道中断
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









