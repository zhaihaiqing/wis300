/* Includes ------------------------------------------------------------------*/
#include "main.h"


uint16_t 	DelayTime = 0;//��ʱ����ֵ(ms)
uint8_t 	DelayTimeFlag = 0;//��ʱ��־

unsigned int  SysTick_Count;//����SysTick������
unsigned int SampleTime;//���尴ʱ������ʱ��
unsigned char SampleTime_Flag;//���尴ʱ��������ʼ��־λ

unsigned char Send_Flag_TPA=0;//������TPA�����жϱ�־λ
unsigned char Clear_Flag_TPA=0;
unsigned char Set_Sample_Speed , Set_Bridge_Type ;//��������ٶȺ͵�������
//����������ϳ��ȣ�������TPA����һ������
//����ͨ��1��2��3��4����
unsigned char *data_p=NULL;
int data_len;
//unsigned char xxxxx=0xff;
 
Ch_DATA_tagdef 						       Ch_A_DATA,Ch_B_DATA,Ch_C_DATA,Ch_D_DATA;
EEPROM_DATA_tagdef					     EEPROM_DATA;
Channel_SET_tagdef  				     Channel_CFG;   //�����޸�ͨ�������ṹ��
Sample_Instruction_tagdef        Sample_Control;//����������ƽṹ��
Inquire_CFGInfo_tagdef 			     Inquire_CFG;//��������������Ϣ�ṹ��
AUTO_CALIBRATION_tagdef          AUTO_CALIBRATION;//�����Զ������ṹ��
Inquire_CALIBRATION_CFG_tagdef   CALIBRATION_CFG;//
CALIBRATION_tagdef               CALIBRATION_SET;//�ֶ�����У׼����

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
	while(TimingDelay != 0)__WFI;//�ȴ������н���sleepģʽ,�·�ָ����ִ�й��������׽���
	//Ӳ���쳣ģʽ
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
	TIM6_Configuration();//TIM6  ��ʱ
	NVIC_TIM6_Configuration();//TIM6�ж�����
	GPIO_Configuration();//GPIO
	SPI1_Configuration();
	UART1_Configuration(57600);//UART1 ��TPXͨѶ
#ifdef  Debug_EN
	UART2_Configuration(256000);//UART2  ���Դ�ӡ
#endif
	NVIC_UART_Configuration();//UART�ж�����
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
		//Return_Ack(HADRWARE_NOTREADY);
		RCC_ClearFlag();}
		IWDG_INIT(5000);
#endif
	//�����������Ӳ����ʼ��
	//EEPROM_EraseFullChip();
	//I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+44,&xxxxx,1);
	//I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+45,&xxxxx,1);
	//I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+46,&xxxxx,1);
	//I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+47,&xxxxx,1);
	I2C1_ReadS_24C(EEPROM_DATA_BASEADDRESS,(unsigned char *)&EEPROM_DATA.A_Zero_offset,sizeof(EEPROM_DATA));	
		
#ifdef WatchDog_EN
	IWDG_ReloadCounter();//ι��
#endif
#ifdef	Debug_EN
	printf("Hardware ready!\r\n");
#endif
		
		
//	send_message_prepare(0xa1);
//	
//  send_message_add(0x05);//���ֽ�

//		
//	send_message_add(0x00);//���ֽ�
//	send_message_add(0x00);//���ֽ�
//	 
//	send_message(1);
	
	
	while(1)
	{
		data_p=get_active_message(&data_len);
#ifdef	Debug_EN
		printf("\r\n");//ͨ�����Դ��ڽ��յ��������	
#endif		
		if(*(data_p+6)==0xa1)//�жϵ�6λ�Ƿ�Ϊa1��������Ϊ�յ�ָ�+7����Ϣ���ͣ�Ȼ������������
		{	
			ADS1248_POWER_H;Delay(200);//����AD��Դ���ȴ���Դ�ȶ�
			switch(*(data_p+7))
			{
				case 	 REQUEST_CHANNEL_CFG:				//��ѯͨ��������Ϣ01
														Return_Channel_CFG();
														Return_Ack(REQUEST_CHANNEL_CFG,SUCCESSFUL_EXECUTION);
														break;
				case	 CHANNEL_SETCFG:						//�޸�ͨ������   03
														ADS1248_CFG_SET();
														break;
				case   REQUEST_CFG_AND_CALIBRATION:		//��ѯͨ�����ò�����У׼����  05
														Return_Channel_CFG();
														Return_Channel_Offset();
														Return_Ack(REQUEST_CFG_AND_CALIBRATION,SUCCESSFUL_EXECUTION);
														break;
				case 	 REQUEST_CALIBRATION_VAL:		//��ȡУ׼����   21
														Return_Channel_Offset();
														Return_Ack(REQUEST_CALIBRATION_VAL,SUCCESSFUL_EXECUTION);
														break;
				case	 CALIBRATION_VAL_SET:				//�ֶ�����У׼   23
														CALIBRATION_PARAMETERS_SET();
														break;
				case	 AUTO_CALIBRATION_VAL_SET:	//�Զ�У׼����   29
														Channel_Offset();
														break;
				case	 SAMPLE_INSTRUCTION:				//��������       41													
														Sample_Instruction_Control();
														break;
				default:														
														break;
			}
		}
	}
}


