/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
//#include "stm32l1xx_it.h"
#include "main.h"
//#include "uart.h"
//#include "spi.h"


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

//void EXTI15_10_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line11) != RESET)
//  {
//    /* Toggle LED1 */
//		LED2_ON;
//    
//    /* Clear the EXTI line 0 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line11);
//  }
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	if(DelayTimeFlag)DelayTime++;
	SysTick_Count++;
	if(SysTick_Count>=1000)
	{
		SysTick_Count=0;
		SampleTime--;
	}
}

void USART1_IRQHandler(void)
{
	unsigned char 	Uart_Get_Data;	//串口1接收的数据
	UART1_RBUF_ST *p = &uart1_rbuf;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Uart_Get_Data = USART_ReceiveData(USART1);
		if((p->in - p->out)<UART1_RBUF_SIZE)
		{
			p->buf [p->in & (UART1_RBUF_SIZE-1)] = Uart_Get_Data;	
			p->in++;
			Uart1Flag = 1;
		}
	}
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : 串口2中断服务函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
	unsigned char 	Uart_Get_Data;	//串口2接收一个字节的数据
	UART2_RBUF_ST *p = &uart2_rbuf;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Uart_Get_Data = USART_ReceiveData(USART2);
		if((p->in - p->out)<UART2_RBUF_SIZE)
		{
			p->buf [p->in & (UART2_RBUF_SIZE-1)] = Uart_Get_Data;	
			p->in++;
			Uart2Flag = 1;
		}
	}
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : 定时器2中断服务函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	 if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	 {
			TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
			GPIO_PinReverse(GPIOB,GPIO_Pin_0);
   }
}

void TIM3_IRQHandler(void)
{
	TIM3->SR&=~(1<<0);//清楚TIM3中断标志位
	Sample_Flag=1;
}


void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //?????
		if(TimingDelay)TimingDelay--;
	}
}



/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
