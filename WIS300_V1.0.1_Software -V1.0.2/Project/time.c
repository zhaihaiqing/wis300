/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "time.h"

/*******************************************************************************
* Function Name  : TIM2_Configuration
* Description    : ��ʱ��2��ʼ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_DeInit(TIM2);					//��λTIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//�ж����ȼ�NVIC����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�	
	
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 400;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler = 36863;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ,32M/(32000-1)=1KhZ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx	
}

//void TIM3_Configuration(void)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_DeInit(TIM3);					//��λTIM3
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	
//	//�ж����ȼ�NVIC����
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
//	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
//  TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE ); //ʹ��ָ����TIM3�ж�,��������ж�	
//	
//	//   29.4912M/(2950-1)=10K
//	TIM_InternalClockConfig(TIM3);
//	TIM_TimeBaseStructure.TIM_Period = 100;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//  TIM_TimeBaseStructure.TIM_Prescaler = 2950;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ,��ʱ�Ӳ���Ƶ����Ƶ��=800hZ
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷָ�:TDTS = Tck_tim
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx	
//}

/**********************************************************************************
* Function Name  : TIM6_Configuration
* TIM6��ʼ��
**********************************************************************************/
void TIM6_Configuration(void)
{
	/* TIM6 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
}
/**********************************************************************************
* Function Name  : TIM6_Reconfiguration
* TIM6��������
* nTime:��ʱֵ
**********************************************************************************/
void TIM6_Reconfiguration(__IO uint32_t nTime)
{ 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = nTime;
  TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 1000;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	TIM6->CNT = 0;
		/* TIM IT enable */
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

  /* TIM6 enable counter */
  TIM_Cmd(TIM6, ENABLE);
}
/**********************************************************************************
* Function Name  : TIM6_Disable
* TIM6�ر�
**********************************************************************************/
void TIM6_Disable(void)
{
  /* TIM IT disable */
	TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
  /* TIM6 disable counter */
  TIM_Cmd(TIM6, DISABLE);
}

/*******************************************************************************
* Function Name  : NVIC_TIM6_Configuration
* �����ж� ��ʼ��
*******************************************************************************/
void NVIC_TIM6_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM6 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; //�����ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռʽ
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}






