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
  if(CoreDebug->DHCSR & 1)
	{
		__breakpoint(0);
	}
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

//外部中断0，，对应通道4
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		Ch_D_DATA.Sample_Flag=1;
		if(  (Ch_D_DATA.Count==0) && (Send_Flag_TPA==0)    ) 
		{
			Send_Flag_TPA = 0x08;
			//GPIO_SetBits(GPIOB,GPIO_Pin_3);
		}
		GPIO_PinReverse(GPIOB,GPIO_Pin_15);
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

//外部中断1，，对应通道3
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
		Ch_C_DATA.Sample_Flag=1;
		if(  (Ch_C_DATA.Count==0) && (Send_Flag_TPA==0)  ) 
		{
			Send_Flag_TPA = 0x04;
			//GPIO_SetBits(GPIOB,GPIO_Pin_3);
		}
		GPIO_PinReverse(GPIOB,GPIO_Pin_15);
    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

//外部中断2，，对应通道2
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
		Ch_B_DATA.Sample_Flag=1;
		if(  (Ch_B_DATA.Count==0) && (Send_Flag_TPA==0)  ) 
		{
			Send_Flag_TPA = 0x02;
			//GPIO_SetBits(GPIOB,GPIO_Pin_3);
		}
		GPIO_PinReverse(GPIOB,GPIO_Pin_15);
    /* Clear the EXTI line 2 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}

//外部中断10，，对应通道1
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line10) != RESET)
  {
		Ch_A_DATA.Sample_Flag=1;					//如果是A通道产的的中断    ，置位标志位
		if(  (Ch_A_DATA.Count==0) && (Send_Flag_TPA==0)  ) 
		{
			Send_Flag_TPA = 0x01;
			//GPIO_SetBits(GPIOB,GPIO_Pin_3);
		}
		///如果是一包数据中的第一个中断，且没有别的通道向TPA发送中断，则产生中断
		GPIO_PinReverse(GPIOB,GPIO_Pin_15);
    /* Clear the EXTI line 10 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line10);
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	if(DelayTimeFlag)DelayTime++;
	if(SampleTime_Flag)
	{
		SysTick_Count++;
		if(SysTick_Count>=999)
		{
			SysTick_Count=0;
			SampleTime--;
		}
	}	
	if(TpaCommandIntervalTime)TpaCommandIntervalTime--;
	else//如果间隔时间已超时,并且缓存中有数据,并且数据标志位为0则说明新包接收完毕
	{
		if(TpaCommandLen && !TpaCommand_Sampling_Flag)TpaCommand_Sampling_Flag = 1;
	}		
}

void USART1_IRQHandler(void)
{
	unsigned char 	Uart_Get_Data;	//串口1接收的数据
	UART1_RBUF_ST *p = &uart1_rbuf;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Uart_Get_Data = USART_ReceiveData(USART1);
		if(!TpaCommand_Sampling_Flag)
		{
			if((((p->out - p->in) & (UART1_RBUF_SIZE - 1)) == 0) || TpaCommandIntervalTime)//如果缓存为空,表示第一个数据,或者计时未结束
			{
				TpaCommandIntervalTime = TPA_COMMAND_INTERVAL_TIME;
				if((p->in - p->out)<UART1_RBUF_SIZE)
				{
					p->buf [p->in & (UART1_RBUF_SIZE-1)] = Uart_Get_Data;	
					p->in++;
					Uart1Flag = 1;
				}
				TpaCommandLen  = (p->in - p->out) & (UART2_RBUF_SIZE - 1);//获取数据长度       
			}
		}
#ifdef Debug_EN
		else printf("Tpa data lost!\r\n");
#endif
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
		  GPIOB->ODR ^= GPIO_Pin_14;
   }
}



void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //?????
		if(TimingDelay)TimingDelay--;
	}
}


void DMA1_Channel4_IRQHandler(void)
{
	/* Test on DMA1 Channel2 Transfer Complete interrupt */
  if(DMA_GetITStatus(DMA1_IT_TC4))
  {
		/* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
		DMA_ClearITPendingBit(DMA1_IT_TC4);//清楚标志位
		DMA_Cmd(DMA1_Channel4, DISABLE);//关闭DMA
	  GPIO_ResetBits(GPIOB,GPIO_Pin_3);//串口数据发送完成后拉低中断引脚
	  DMASendDataCompleted=0;
    //USART_Cmd(USART1, DISABLE);
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
