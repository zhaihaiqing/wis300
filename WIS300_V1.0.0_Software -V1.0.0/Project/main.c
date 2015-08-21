/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"
#include "spi.h"
#include "time.h"

uint16_t 	DelayTime = 0;//延时计数值(ms)
uint8_t 	DelayTimeFlag = 0;//计时标志

unsigned int  SysTick_Count;
unsigned int SampleTime;

//unsigned char USART2_RXDATA[9];//一个字节开始位，三个字节数据
unsigned char Sample_Flag;//采样标志
//unsigned char Overrange_Flag=1;
//定义指令检查标志，指令无误，返回1，指令有误不执行
unsigned char Instruction_Check_Flag; 
//定义通道设置标志，只有对应通道完成设置，采样才可开始，否则返回硬件没准备好
unsigned char A_Channel_CFG_Flag=0,B_Channel_CFG_Flag=0,C_Channel_CFG_Flag=0,D_Channel_CFG_Flag=0;
unsigned char Send_Flag_TPA=1;
unsigned char Set_Sample_Speed , Set_Bridge_Type ;
int A_Channel_DATAbuf[6],B_Channel_DATAbuf[6],C_Channel_DATAbuf[6],D_Channel_DATAbuf[6];
unsigned char Channel_DATASize;

unsigned char ADS1_PGA=0x80;
unsigned char ADS2_PGA=0x80;
unsigned char ADS3_PGA=0x80;
unsigned char ADS4_PGA=0x80;

unsigned char *data_p=NULL;
int data_len;

float A_HalfSubZero,B_HalfSubZero,C_HalfSubZero,D_HalfSubZero;//定义线性调整系数，半量程校准和0点校准的差值，确保为正数

unsigned int count;//定义采样次数
unsigned int TIM_ACC;//定义采样定时器时间

unsigned char Channel_Select;
Channel_SET_tagdef  				A_Channel_CFG,B_Channel_CFG,C_Channel_CFG,D_Channel_CFG;//定义修改通道参数结构体
Sample_Instruction_tagdef   Sample_Control;//定义采样控制结构体
Inquire_CFG_tagdef 				  Inquire_CFG;//定义请求配置信息结构体
AUTO_CALIBRATION_tagdef     AUTO_CALIBRATION;//定义自动采样结构体
EEPROM_DATA_tagdef					EEPROM_DATA;

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
	while(TimingDelay != 0)__WFI;//等待过程中进入sleep模式
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
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}
  
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
* Function Name  : Instruction_Check
* Description    : 检测串口发送命令合法性，不合法不执行
* Input          : GPIOx，GPIO_PIN_x
* Output         : None
* Return         : None
*******************************************************************************/
static unsigned char Instruction_Check(void)
{
	if((*(data_p+7))==REQUEST_CHANNEL_CFG)//查询通道配置信息    0x01
	{
		//Return_Ack(SUCCESSFUL_EXECUTION);//返回ACK
		return SUCCESS;
	}
	
	else if((*(data_p+7))==CHANNEL_SETCFG)//检查消息类型   
	{				//检查配置					 0x03
			if(((*(data_p+8))&0x0f)!=ERROR)		//检查通道
			{
				if( ((*(data_p+9))==0x00)|((*(data_p+9))==0x01)|((*(data_p+9))==0x02)|((*(data_p+9))==0x04) )//检查电桥类型
				{
					//Return_Ack(SUCCESSFUL_EXECUTION);//返回ACK
					return SUCCESS;
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
					return ERROR;
				}
			}
			else
			{
			Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
			return ERROR;
			}
	 }
				
	else if((*(data_p+7))==REQUEST_CALIBRATION_VAL)
	{
				//检查读取校准参数    0x21
				if( ((*(data_p+8))==0x01)|((*(data_p+8))==0x02)|((*(data_p+8))==0x04)|((*(data_p+8))==0x08) )
				{
					//Return_Ack(SUCCESSFUL_EXECUTION);//返回ACK
					return SUCCESS;
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
					return ERROR;
				}
	}				
#ifdef  CALIBRATION_BYHAND
	else if((*(data_p+7))==CALIBRATION_VAL_SET)//手动设置校准        0x23
	{
				if( ((*(data_p+8))==0x01)|((*(data_p+8))==0x02)|((*(data_p+8))==0x04)|((*(data_p+8))==0x08) )
				{
					//Return_Ack(SUCCESSFUL_EXECUTION);//返回ACK
					return SUCCESS;
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
					return ERROR;
				}
	}
#endif
		
	else if((*(data_p+7))==AUTO_CALIBRATION_VAL_SET)											//检查校准命令   0x29
	{
			if(((*(data_p+8))&0x0f)!=ERROR)										//检查通道
			{
				if( ((*(data_p+9))==0x00)|((*(data_p+9))==0x01))//检查校准类型
				{
					//Return_Ack(SUCCESSFUL_EXECUTION);//返回ACK
					return SUCCESS;
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
					return ERROR;
				}
			}
			else
			{
				Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
				return ERROR;
			}
	}
		
	else if((*(data_p+7))==SAMPLE_INSTRUCTION)								//检查采样命令             0x41
	{
			if(((*(data_p+8))&0x0f)!=ERROR)												//检查通道
			{
				if((*(data_p+9)  )<0x07)														//检查采样频率
				{
					if( ((*(data_p+10))==0x00)|((*(data_p+10))==0x01))//检查采样类型
					{
						
						
						if(    ((*(data_p+13)  )>0x00) &&  ((*(data_p+13)  )<0x07)  )
						{
									//Return_Ack(SUCCESSFUL_EXECUTION);//返回ACK
									return SUCCESS;
						}
						else
						{
							Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
							return ERROR;
						}
					}
					else
					{
						Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
						return ERROR;
					}
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
					return ERROR;
				}				
			}
			else
			{
				Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
				return ERROR;
			}		
	}
	else
	{
		Return_Ack(PARAMETERS_ILLEGAL);//返回ACK
		return ERROR;
	}	
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
	/******************外设配置***********************/
 	/* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断优先级分组
	if (SysTick_Config(SystemCoreClock / 1000))while(1);	
	GPIO_Configuration();//GPIO
	UART1_Configuration(256000);//UART1 与TPX通讯
	
#ifdef  Debug_EN
	UART2_Configuration(256000);//UART2  调试打印
#endif
	
	NVIC_UART_Configuration();//UART中断配置
#ifdef WatchDog_EN
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET)
	{
		printf("Independent watchdog reset flag...\r\n");
		Return_Ack(HADRWARE_NOTREADY);
		RCC_ClearFlag();
	}
	IWDG_INIT(5000);
#endif

	I2C1_Configuration();
	TIM6_Configuration();//TIM6  延时
	NVIC_TIM6_Configuration();//TIM6中断配置
	Delay(5);								//延时5ms
	SPI1_Configuration();
	TIM2_Configuration();
	//////////////////////////////////
	//FBI 
	//警告：此处不可乱点
	//EEPROM_Check();
	//EEPROM_EraseFullChip();
	//////////////////////////////
//		
//if(1)
//{
//	send_message_prepare(0xa1);
//	send_message_add(0x41);
//	
//	send_message_add(0x01);//单字节组包  通道
//	
//	send_message_add(0x02);//单字节组包  
//	
//	send_message_add(0x01);//单字节组包  采样类型
//	
//	send_message_add(0x00);//单字节组包  采样时长
//	send_message_add(0x00);//单字节组包
//	
//	send_message_add(0x06);//单字节组包

//	send_message_add(0x00);//单字节组包
//	send_message_add(0x00);//单字节组包
//	send_message(1);//发送包，完成
//}	


//if(1)
//{
//	send_message_prepare(0xa1);
//	send_message_add(0x03);
//	
//	send_message_add(0x08);//单字节组包  通道
//	
//	send_message_add(0x04);//单字节组包  
//	
//	send_message_add(0x00);//单字节组包  采样类型
//	
//	send_message_add(0x07);//单字节组包  采样时长
//	send_message_add(0xd0);//单字节组包
//	


//	send_message_add(0x00);//单字节组包
//	send_message_add(0x00);//单字节组包
//	send_message(1);//发送包，完成
//}	


//if(1)
//{
//	send_message_prepare(0xa1);
//	
//	send_message_add(0x23);
//	send_message_add(0x01);//单字节组包  通道
//	
//	send_message_add(0x00);//单字节组包  速度
//	send_message_add(0x0b);//单字节组包  采样类型
//	send_message_add(0xce);//单字节组包  采样时长
//	send_message_add(0xeb);//单字节组包
//	
//	send_message_add(0xff);//单字节组包
//	send_message_add(0xbf);//单字节组包
//	send_message_add(0x8e);//单字节组包
//	send_message_add(0x09);//单字节组包

//	send_message_add(0x00);//单字节组包
//	send_message_add(0x00);//单字节组包
//	send_message(1);//发送包，完成
//}	


		
	/*****************************************************************************
	//EEPROM数据存储结构，，，单字节对齐
	//
//									0点校准(4B)			半量程校准(4B)				K值(2B)			滤波器参数(1B)
//	
//			A通道			 base(0x0040)           +4								+8						+10
//	
//			B通道						+11								+15								+19						+21
//	
//			C通道						+22								+26								+30						+32
//	
//			D通道						+33								+37								+41						+43
//	
	*****************************************************************************/	
	//读EEPROM中存储的滤波器参数
	I2C1_ReadS_24C(EEPROM_DATA_BASEADDRESS,(unsigned char *)&EEPROM_DATA.A_Zero_offset,sizeof(EEPROM_DATA));

#ifdef WatchDog_EN
	IWDG_ReloadCounter();//喂狗
#endif

#ifdef	Debug_EN
	printf("Hardware ready!\r\n");
	printf("Waiting Instruction!\r\n");
#endif
	Return_Ack(SUCCESSFUL_EXECUTION);//上电打印执行成功，等待串口数据
	while(1)
	{
		data_p=get_active_message(&data_len);
		printf("\r\n");
		//通过调试串口将收到的命令返回
		//判断第6位是否为a1，是则认为收到指令，+7是消息类型，然后后面就是数据
		if(*(data_p+6)==0xa1)
		{
			Instruction_Check_Flag=Instruction_Check();
			ADS1248_POWER_H;
			Delay(200);//等待电源稳定
			if(Instruction_Check_Flag)
			{				
				switch(*(data_p+7))
				{
					case 	 REQUEST_CHANNEL_CFG:				//查询通道配置信息0x01
#ifdef	       						Debug_EN
													printf("Return Channel CFG!\r\n");
#endif
													Return_Channel_CFG();													
													break;
					case	 CHANNEL_SETCFG:						//修改通道设置   0x03
													ADS1248_CFG_SET();
													break;
					case 	 REQUEST_CALIBRATION_VAL:		//读取校准参数   0x21
													Return_Channel_Offset();
													break;
					case	 CALIBRATION_VAL_SET:				//手动设置校准   0x23
													CALIBRATION_PARAMETERS_SET();
													break;
					case	 AUTO_CALIBRATION_VAL_SET:	//自动校准命令   0x29
													Channel_Offset();
													break;
					case	 SAMPLE_INSTRUCTION:				//采样命令       0x41													
													Sample_Instruction_Control();
													break;
					default:
													break;
				}
			}
		}
	}
}


