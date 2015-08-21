/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"
#include "spi.h"
#include "time.h"

uint16_t 	DelayTime = 0;//��ʱ����ֵ(ms)
uint8_t 	DelayTimeFlag = 0;//��ʱ��־

unsigned int  SysTick_Count;
unsigned int SampleTime;

//unsigned char USART2_RXDATA[9];//һ���ֽڿ�ʼλ�������ֽ�����
unsigned char Sample_Flag;//������־
//unsigned char Overrange_Flag=1;
//����ָ�����־��ָ�����󣬷���1��ָ������ִ��
unsigned char Instruction_Check_Flag; 
//����ͨ�����ñ�־��ֻ�ж�Ӧͨ��������ã������ſɿ�ʼ�����򷵻�Ӳ��û׼����
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

float A_HalfSubZero,B_HalfSubZero,C_HalfSubZero,D_HalfSubZero;//�������Ե���ϵ����������У׼��0��У׼�Ĳ�ֵ��ȷ��Ϊ����

unsigned int count;//�����������
unsigned int TIM_ACC;//���������ʱ��ʱ��

unsigned char Channel_Select;
Channel_SET_tagdef  				A_Channel_CFG,B_Channel_CFG,C_Channel_CFG,D_Channel_CFG;//�����޸�ͨ�������ṹ��
Sample_Instruction_tagdef   Sample_Control;//����������ƽṹ��
Inquire_CFG_tagdef 				  Inquire_CFG;//��������������Ϣ�ṹ��
AUTO_CALIBRATION_tagdef     AUTO_CALIBRATION;//�����Զ������ṹ��
EEPROM_DATA_tagdef					EEPROM_DATA;

	__IO uint32_t TimingDelay = 0;;
/**********************************************************************************
* Function Name  : Delay
* ��ʱ����,ͨ����time6�趨
* nTime:ʱ��ֵ(ms)
**********************************************************************************/
void Delay(__IO uint32_t nTime)
{ 
	TIM6_Reconfiguration(nTime);
	TimingDelay = 1;
	while(TimingDelay != 0)__WFI;//�ȴ������н���sleepģʽ
	//PWR_EnterSleepMode(PWR_Regulator_LowPower,PWR_SLEEPEntry_WFI);
	TIM6_Disable();
}

/*******************************************************************************
* Function Name  : GPIO_PinReverse
* Description    : GPIOȡ������
* Input          : GPIOx��GPIO_PIN_x
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
* Input          : FeedTime ι��ʱ�� ms
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
* Description    : ��⴮�ڷ�������Ϸ��ԣ����Ϸ���ִ��
* Input          : GPIOx��GPIO_PIN_x
* Output         : None
* Return         : None
*******************************************************************************/
static unsigned char Instruction_Check(void)
{
	if((*(data_p+7))==REQUEST_CHANNEL_CFG)//��ѯͨ��������Ϣ    0x01
	{
		//Return_Ack(SUCCESSFUL_EXECUTION);//����ACK
		return SUCCESS;
	}
	
	else if((*(data_p+7))==CHANNEL_SETCFG)//�����Ϣ����   
	{				//�������					 0x03
			if(((*(data_p+8))&0x0f)!=ERROR)		//���ͨ��
			{
				if( ((*(data_p+9))==0x00)|((*(data_p+9))==0x01)|((*(data_p+9))==0x02)|((*(data_p+9))==0x04) )//����������
				{
					//Return_Ack(SUCCESSFUL_EXECUTION);//����ACK
					return SUCCESS;
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//����ACK
					return ERROR;
				}
			}
			else
			{
			Return_Ack(PARAMETERS_ILLEGAL);//����ACK
			return ERROR;
			}
	 }
				
	else if((*(data_p+7))==REQUEST_CALIBRATION_VAL)
	{
				//����ȡУ׼����    0x21
				if( ((*(data_p+8))==0x01)|((*(data_p+8))==0x02)|((*(data_p+8))==0x04)|((*(data_p+8))==0x08) )
				{
					//Return_Ack(SUCCESSFUL_EXECUTION);//����ACK
					return SUCCESS;
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//����ACK
					return ERROR;
				}
	}				
#ifdef  CALIBRATION_BYHAND
	else if((*(data_p+7))==CALIBRATION_VAL_SET)//�ֶ�����У׼        0x23
	{
				if( ((*(data_p+8))==0x01)|((*(data_p+8))==0x02)|((*(data_p+8))==0x04)|((*(data_p+8))==0x08) )
				{
					//Return_Ack(SUCCESSFUL_EXECUTION);//����ACK
					return SUCCESS;
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//����ACK
					return ERROR;
				}
	}
#endif
		
	else if((*(data_p+7))==AUTO_CALIBRATION_VAL_SET)											//���У׼����   0x29
	{
			if(((*(data_p+8))&0x0f)!=ERROR)										//���ͨ��
			{
				if( ((*(data_p+9))==0x00)|((*(data_p+9))==0x01))//���У׼����
				{
					//Return_Ack(SUCCESSFUL_EXECUTION);//����ACK
					return SUCCESS;
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//����ACK
					return ERROR;
				}
			}
			else
			{
				Return_Ack(PARAMETERS_ILLEGAL);//����ACK
				return ERROR;
			}
	}
		
	else if((*(data_p+7))==SAMPLE_INSTRUCTION)								//����������             0x41
	{
			if(((*(data_p+8))&0x0f)!=ERROR)												//���ͨ��
			{
				if((*(data_p+9)  )<0x07)														//������Ƶ��
				{
					if( ((*(data_p+10))==0x00)|((*(data_p+10))==0x01))//����������
					{
						
						
						if(    ((*(data_p+13)  )>0x00) &&  ((*(data_p+13)  )<0x07)  )
						{
									//Return_Ack(SUCCESSFUL_EXECUTION);//����ACK
									return SUCCESS;
						}
						else
						{
							Return_Ack(PARAMETERS_ILLEGAL);//����ACK
							return ERROR;
						}
					}
					else
					{
						Return_Ack(PARAMETERS_ILLEGAL);//����ACK
						return ERROR;
					}
				}
				else
				{
					Return_Ack(PARAMETERS_ILLEGAL);//����ACK
					return ERROR;
				}				
			}
			else
			{
				Return_Ack(PARAMETERS_ILLEGAL);//����ACK
				return ERROR;
			}		
	}
	else
	{
		Return_Ack(PARAMETERS_ILLEGAL);//����ACK
		return ERROR;
	}	
}

/*******************************************************************************
* Function Name  : ������
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	/******************��������***********************/
 	/* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж����ȼ�����
	if (SysTick_Config(SystemCoreClock / 1000))while(1);	
	GPIO_Configuration();//GPIO
	UART1_Configuration(256000);//UART1 ��TPXͨѶ
	
#ifdef  Debug_EN
	UART2_Configuration(256000);//UART2  ���Դ�ӡ
#endif
	
	NVIC_UART_Configuration();//UART�ж�����
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
	TIM6_Configuration();//TIM6  ��ʱ
	NVIC_TIM6_Configuration();//TIM6�ж�����
	Delay(5);								//��ʱ5ms
	SPI1_Configuration();
	TIM2_Configuration();
	//////////////////////////////////
	//FBI 
	//���棺�˴������ҵ�
	//EEPROM_Check();
	//EEPROM_EraseFullChip();
	//////////////////////////////
//		
//if(1)
//{
//	send_message_prepare(0xa1);
//	send_message_add(0x41);
//	
//	send_message_add(0x01);//���ֽ����  ͨ��
//	
//	send_message_add(0x02);//���ֽ����  
//	
//	send_message_add(0x01);//���ֽ����  ��������
//	
//	send_message_add(0x00);//���ֽ����  ����ʱ��
//	send_message_add(0x00);//���ֽ����
//	
//	send_message_add(0x06);//���ֽ����

//	send_message_add(0x00);//���ֽ����
//	send_message_add(0x00);//���ֽ����
//	send_message(1);//���Ͱ������
//}	


//if(1)
//{
//	send_message_prepare(0xa1);
//	send_message_add(0x03);
//	
//	send_message_add(0x08);//���ֽ����  ͨ��
//	
//	send_message_add(0x04);//���ֽ����  
//	
//	send_message_add(0x00);//���ֽ����  ��������
//	
//	send_message_add(0x07);//���ֽ����  ����ʱ��
//	send_message_add(0xd0);//���ֽ����
//	


//	send_message_add(0x00);//���ֽ����
//	send_message_add(0x00);//���ֽ����
//	send_message(1);//���Ͱ������
//}	


//if(1)
//{
//	send_message_prepare(0xa1);
//	
//	send_message_add(0x23);
//	send_message_add(0x01);//���ֽ����  ͨ��
//	
//	send_message_add(0x00);//���ֽ����  �ٶ�
//	send_message_add(0x0b);//���ֽ����  ��������
//	send_message_add(0xce);//���ֽ����  ����ʱ��
//	send_message_add(0xeb);//���ֽ����
//	
//	send_message_add(0xff);//���ֽ����
//	send_message_add(0xbf);//���ֽ����
//	send_message_add(0x8e);//���ֽ����
//	send_message_add(0x09);//���ֽ����

//	send_message_add(0x00);//���ֽ����
//	send_message_add(0x00);//���ֽ����
//	send_message(1);//���Ͱ������
//}	


		
	/*****************************************************************************
	//EEPROM���ݴ洢�ṹ���������ֽڶ���
	//
//									0��У׼(4B)			������У׼(4B)				Kֵ(2B)			�˲�������(1B)
//	
//			Aͨ��			 base(0x0040)           +4								+8						+10
//	
//			Bͨ��						+11								+15								+19						+21
//	
//			Cͨ��						+22								+26								+30						+32
//	
//			Dͨ��						+33								+37								+41						+43
//	
	*****************************************************************************/	
	//��EEPROM�д洢���˲�������
	I2C1_ReadS_24C(EEPROM_DATA_BASEADDRESS,(unsigned char *)&EEPROM_DATA.A_Zero_offset,sizeof(EEPROM_DATA));

#ifdef WatchDog_EN
	IWDG_ReloadCounter();//ι��
#endif

#ifdef	Debug_EN
	printf("Hardware ready!\r\n");
	printf("Waiting Instruction!\r\n");
#endif
	Return_Ack(SUCCESSFUL_EXECUTION);//�ϵ��ӡִ�гɹ����ȴ���������
	while(1)
	{
		data_p=get_active_message(&data_len);
		printf("\r\n");
		//ͨ�����Դ��ڽ��յ��������
		//�жϵ�6λ�Ƿ�Ϊa1��������Ϊ�յ�ָ�+7����Ϣ���ͣ�Ȼ������������
		if(*(data_p+6)==0xa1)
		{
			Instruction_Check_Flag=Instruction_Check();
			ADS1248_POWER_H;
			Delay(200);//�ȴ���Դ�ȶ�
			if(Instruction_Check_Flag)
			{				
				switch(*(data_p+7))
				{
					case 	 REQUEST_CHANNEL_CFG:				//��ѯͨ��������Ϣ0x01
#ifdef	       						Debug_EN
													printf("Return Channel CFG!\r\n");
#endif
													Return_Channel_CFG();													
													break;
					case	 CHANNEL_SETCFG:						//�޸�ͨ������   0x03
													ADS1248_CFG_SET();
													break;
					case 	 REQUEST_CALIBRATION_VAL:		//��ȡУ׼����   0x21
													Return_Channel_Offset();
													break;
					case	 CALIBRATION_VAL_SET:				//�ֶ�����У׼   0x23
													CALIBRATION_PARAMETERS_SET();
													break;
					case	 AUTO_CALIBRATION_VAL_SET:	//�Զ�У׼����   0x29
													Channel_Offset();
													break;
					case	 SAMPLE_INSTRUCTION:				//��������       0x41													
													Sample_Instruction_Control();
													break;
					default:
													break;
				}
			}
		}
	}
}


