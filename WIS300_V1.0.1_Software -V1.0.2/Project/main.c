/* Includes ------------------------------------------------------------------*/
#include "main.h"


uint16_t 	DelayTime = 0;//延时计数值(ms)
uint8_t 	DelayTimeFlag = 0;//计时标志

unsigned int  SysTick_Count;//定义SysTick计数器
unsigned int SampleTime;//定义按时长采样时长
unsigned char SampleTime_Flag;//定义按时长采样开始标志位

unsigned char Send_Flag_TPA=0;//定义向TPA发送中断标志位
unsigned char Clear_Flag_TPA=0;
unsigned char Set_Sample_Speed , Set_Bridge_Type ;//定义采样速度和电桥类型
//定义数据组合长度，用于向TPA发送一包数据
//定义通道1、2、3、4增益
unsigned char *data_p=NULL;
int data_len;

__IO uint8_t TpaCommandIntervalTime = 0;//定义tpa指令发送间隔时间,超时认为一条指令已发送完毕
__IO uint8_t TpaCommand_Sampling_Flag = 0;//指令标志位,1=已收到一条指令
__IO uint8_t TpaCommandLen = 0;//指令长度

 
Ch_DATA_tagdef 						       Ch_A_DATA,Ch_B_DATA,Ch_C_DATA,Ch_D_DATA;
EEPROM_DATA_tagdef					     EEPROM_DATA;
Channel_SET_tagdef  				     Channel_CFG;   //定义修改通道参数结构体
Sample_Instruction_tagdef        Sample_Control;//定义采样控制结构体
Inquire_CFGInfo_tagdef 			     Inquire_CFG;//定义请求配置信息结构体
AUTO_CALIBRATION_tagdef          AUTO_CALIBRATION;//定义自动采样结构体
Inquire_CALIBRATION_CFG_tagdef   CALIBRATION_CFG;//
CALIBRATION_tagdef               CALIBRATION_SET;//手动设置校准参数

__IO uint32_t TimingDelay = 0;;
/**********************************************************************************
* Function Name  : Delay
* 延时函数,通过对time6设定
* nTime:时间值(ms)
**********************************************************************************/
void Delay(__IO uint32_t nTime)
{ 
	TIM6_Reconfiguration(nTime);
	TimingDelay = 1;
	while(TimingDelay != 0)__WFI;//等待过程中进入sleep模式,下方指令在执行过程中容易进入
	//硬件异常模式
	//PWR_EnterSleepMode(PWR_Regulator_LowPower,PWR_SLEEPEntry_WFI);
	TIM6_Disable();
}

/*******************************************************************************
* Function Name  : GPIO_PinReverse
* Description    : GPIO取反操作
* Input          : GPIOx，GPIO_PIN_x
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    GPIOx->ODR ^= GPIO_Pin;
}

/*******************************************************************************
* Function Name  : IWDG_INIT
* Description    : 
* Input          : FeedTime 喂狗时间 ms
* Output         : None
* Return         : None
*******************************************************************************/
void IWDG_INIT(uint32_t FeedTime)
{
  /* Enable the LSI oscillator ************************************************/
  RCC_LSICmd(ENABLE);
  
  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: LSI/256 */
  //37k/256=144.53125Hz=6.9ms
  IWDG_SetPrescaler(IWDG_Prescaler_256);

  IWDG_SetReload(FeedTime/7);

  /* Reload IWDG counter */
  IWDG_ReloadCounter();

  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
}

/*******************************************************************************
* Function Name  : 主函数
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	
	unsigned char Test_i;
	/******************外设配置***********************/
 	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断优先级分组
	if (SysTick_Config(SystemCoreClock / 1000))while(1);	
	TIM6_Configuration();//TIM6  延时
	NVIC_TIM6_Configuration();//TIM6中断配置
	GPIO_Configuration();//GPIO
	SPI1_Configuration();
	UART1_Configuration(100000);//UART1 与TPX通讯
	UART1TX_DMA_Configuration();
#ifdef  Debug_EN
	UART2_Configuration(256000);//UART2  调试打印
#endif
	NVIC_UART_Configuration();//UART中断配置
	I2C1_Configuration();
	TIM2_Configuration();
	EXTI0_Config();
	EXTI1_Config();
	EXTI2_Config();
	EXTI15_10_Config();
#ifdef WatchDog_EN
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET)
	{
#ifdef Debug_EN
		printf("Watchdog Reset!\r\n");
#endif
		RCC_ClearFlag();}
		IWDG_INIT(10000);
#endif
	I2C1_ReadS_24C(EEPROM_DATA_BASEADDRESS,(unsigned char *)&EEPROM_DATA.A_Zero_offset,sizeof(EEPROM_DATA));

	printf("EEPROM_DATA.A_Zero_offset:%.9f\r\n",EEPROM_DATA.A_Zero_offset);
	printf("EEPROM_DATA.A_Half_offset:%.9f\r\n",EEPROM_DATA.A_Half_offset);
		
#ifdef WatchDog_EN
	IWDG_ReloadCounter();//喂狗
#endif
#ifdef	Debug_EN
	printf("Hardware ready!\r\n");
#endif		
	
		
//		
//		send_message_prepare(0xa1);
//		
//		
//		send_message_add(29);
//		send_message_add(1);
//		send_message_add(1);
//		
//		send_message_add(0);
//		send_message_add(0);
//		
//		send_message(1);
//		
	
			
		
		
		
		
		
	
	while(1)
	{
		data_p=get_active_message(&data_len);
		TpaCommandLen = 0;//先清理数据长度
		TpaCommand_Sampling_Flag = 0;//清零标志位
		USART1_ClearBuf();//清空串口缓存
#ifdef	Debug_EN
		printf("\r\n");//通过调试串口将收到的命令返回	
#endif		
		if(*(data_p+6)==0xa1)//判断第6位是否为a1，是则认为收到指令，+7是消息类型，然后后面就是数据
		{	
			ADS1248_POWER_H;Delay(200);//开启AD电源，等待电源稳定
			switch(*(data_p+7))
			{
				case 	 REQUEST_CHANNEL_CFG:				//查询通道配置信息01
														Return_Channel_CFG();
														Return_Ack(REQUEST_CHANNEL_CFG,SUCCESSFUL_EXECUTION);
														break;
				case	 CHANNEL_SETCFG:						//修改通道设置   03
														ADS1248_CFG_SET();
														break;
				case   REQUEST_CFG_AND_CALIBRATION:		//查询通道配置参数及校准参数  05
														Return_Channel_CFG();
														Return_Channel_Offset();
														Return_Ack(REQUEST_CFG_AND_CALIBRATION,SUCCESSFUL_EXECUTION);
														break;
				case 	 REQUEST_CALIBRATION_VAL:		//读取校准参数   21
														Return_Channel_Offset();
														Return_Ack(REQUEST_CALIBRATION_VAL,SUCCESSFUL_EXECUTION);
														break;
				case	 CALIBRATION_VAL_SET:				//手动设置校准   23
														CALIBRATION_PARAMETERS_SET();
														break;
				case	 AUTO_CALIBRATION_VAL_SET:	//自动校准命令   29
														Channel_Offset();
														break;
				case	 SAMPLE_INSTRUCTION:				//采样命令       41													
														Sample_Instruction_Control();
														break;
				default:														
														break;
			}
		}
	}
}


