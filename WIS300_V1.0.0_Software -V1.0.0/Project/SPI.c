/*******************************************************************************
* File Name          : SPI.c
* Description        : ADS1248��STM32L151ͨ����������
* Author             : zhaihaiqing
* Mail               : zhaihaiqing@foxmail.com
* Created Time       : 2015-06-17 15:16
* Last Update				 :
*******************************************************************************/

#include "main.h"
//#include "spi.h"

/*******************************************************************************
* Function Name  : ADS1248_delay
* Description    : ��ʱ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADS1248_delay(unsigned int val)
{
  unsigned int i;
  i = 1000*val;
  while(i--);
}

/*******************************************************************************
* Function Name  : SPI1_Configuration
* Description    : STM32 SPI1����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_Configuration(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);													//������ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC , ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);                     //�������蹦��
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_7;					//���ö˿�ģʽ��PA5=SCK1,PA6=MISO1,PA7=MOSI1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                //���踴��ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;															//����ģʽ
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;															//Ĭ������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;														//GPIO��ת�ٶ�Ϊ40MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);																			//������д���Ӧ��GPIO�Ĵ�����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4;							//����SPIƬѡ��PA1=CS1��PA0=CS2 ��PA4=CS4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																//�˿����ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;															//����ģʽ
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;																//Ĭ����������ֹADƬѡ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;														//GPIO��ת�ٶ�Ϊ2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);																			//������д���Ӧ��GPIO�Ĵ����� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;																	//����SPIƬѡ��PC15=CS3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																//�˿����ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;															//����ģʽ
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;																//Ĭ����������ֹADƬѡ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;														//GPIO��ת�ٶ�Ϊ2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);																			//������д���Ӧ��GPIO�Ĵ�����
	
	/* SPI1 ���� */
	SPI_Cmd(SPI1, DISABLE);
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   	//SPI1����Ϊ����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;									       	//����SPI1Ϊ��ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;											//SPI���ͽ���8λ֡�ṹ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;														//����ʱ�ӿ��� �͵�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;													//�ڶ���ʱ���ؿ�ʼ��������
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//NSS�ź��������ʹ��SSIλ������
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ2
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;										//���ݴ����MSBλ��ʼ
  SPI_InitStructure.SPI_CRCPolynomial = 7;															//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);
	
  SPI_Cmd(SPI1, ENABLE); 
}
/*******************************************************************************
* Function Name  : SPI1_CSx_LOW
* Description    : SPI��Ƭѡ�źţ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_CS1_LOW(void)
{
			GPIOA->BSRRL=GPIO_Pin_0;//CS2��1
			GPIOA->BSRRL=GPIO_Pin_4;//CS4
			GPIOC->BSRRL=GPIO_Pin_15;//CS3
			GPIOA->BSRRH=GPIO_Pin_1;//����CS1
}

void SPI1_CS2_LOW(void)
{
			GPIOA->BSRRL=GPIO_Pin_1;//
			GPIOA->BSRRL=GPIO_Pin_4;
			GPIOC->BSRRL=GPIO_Pin_15;
			GPIOA->BSRRH=GPIO_Pin_0;//
}

void SPI1_CS3_LOW(void)
{
			GPIOA->BSRRL=GPIO_Pin_0;//
			GPIOA->BSRRL=GPIO_Pin_4;
			GPIOA->BSRRL=GPIO_Pin_1;
			GPIOC->BSRRH=GPIO_Pin_15;//
}

void SPI1_CS4_LOW(void)
{
			GPIOA->BSRRL=GPIO_Pin_0;//
			GPIOA->BSRRL=GPIO_Pin_1;
			GPIOC->BSRRL=GPIO_Pin_15;
			GPIOA->BSRRH=GPIO_Pin_4;//
}

void SPI1_ALLCS_H(void)
{
			GPIOA->BSRRL=GPIO_Pin_0;//
			GPIOA->BSRRL=GPIO_Pin_4;
			GPIOC->BSRRL=GPIO_Pin_15;
			GPIOA->BSRRL=GPIO_Pin_1;//
}

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
**********************************************/
//void EXTI15_10_Config(void)
//{
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	GPIO_InitTypeDef   GPIO_InitStructure;
//	NVIC_InitTypeDef   NVIC_InitStructure;
//  /* Enable GPIOA clock */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//  /* Configure PA11 pin as input floating */
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;																//
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;															//
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;													//
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;													//
//	GPIO_Init(GPIOA, &GPIO_InitStructure); 

//  /* Enable SYSCFG clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//  /* Connect EXTI11 Line to PA11 pin */
//  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource11);

//  /* Configure EXTI0 line */
//  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);

//  /* Enable and set EXTI0 Interrupt to the lowest priority */
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}

/*******************************************************************************
* Function Name  : ADS1248_ReadByte
* Description    : STM32 ��SPI�����϶�һ���ֽ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char ADS1248_ReadByte(void)
{
	return (ADS1248_WriteByte(ADS_NOP));
}

/*******************************************************************************
* Function Name  : ADS1248_WriteByte
* Description    : STM32 ��SPI1����дһ���ֽ�����
* Input          : ���ݣ��޷���char��
* Output         : None
* Return         : 16λ�޷�������
*******************************************************************************/
unsigned char ADS1248_WriteByte(unsigned char data)
{
	
	while(SPI_I2S_GetFlagStatus(ADS1248_SPI, SPI_I2S_FLAG_TXE) == RESET);	// Loop while DR register in not emplty 
	
	SPI_I2S_SendData(ADS1248_SPI, data);																	// Send u8 through the SPI bus peripheral 
	
	while(SPI_I2S_GetFlagStatus(ADS1248_SPI, SPI_I2S_FLAG_RXNE) == RESET);// Wait to receive a u8 
	
	return SPI_I2S_ReceiveData(ADS1248_SPI);															// Return the u8 read from the SPI bus 
}

/*******************************************************************************
* Function Name  : ADS1248_ReadREG
* Description    : STM32 ͨ��SPI1��ADS1248�Ĵ���ֵ
* Input          : ��ַ
* Output         : None
* Return         : �Ĵ���ֵ
*******************************************************************************/
unsigned char ADS1248_ReadREG(unsigned char Reg_addr)
{
	unsigned char Reg_addr_data;
	ADS1248_delay(1);
	ADS1248_WriteByte(ADS_ReadREG+(Reg_addr & 0xf));
	ADS1248_WriteByte(0);
	Reg_addr_data=ADS1248_WriteByte(ADS_NOP);
	ADS1248_delay(1);
	return Reg_addr_data;
}

/*******************************************************************************
* Function Name  : ADS1248_WriteReg
* Description    : STM32 ͨ��SPI1������ADS1248��Ӧ�Ĵ�����д����
* Input          : �Ĵ�����ַ������
* Output         : None
* Return         : None
*******************************************************************************/
void ADS1248_WriteReg(unsigned char Reg_addr,unsigned char data)
{
	ADS1248_delay(1);
	ADS1248_WriteByte(ADS_WriteREG+(Reg_addr & 0xf));
	ADS1248_WriteByte(0);
	ADS1248_WriteByte(data);
	ADS1248_delay(1);
}

/*******************************************************************************
* Function Name  : ADS1248_Init1
* Description    : ADS1248��ʼ�����������ò��������в��ɸ��ĵļĴ���
ADS1248��ʼ�����������������͡�����Դ�������ٶ�
									 ADS1248�ڿ�������ʱ�����ݵ��������������ͣ������ݵ���Чλ�½�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADS1248_Init(unsigned char Sample_Speed,unsigned char Bridge_Type)
{
	                       
	ADS1248_delay(1);	
	//�̶�AIN0Ϊ�����룬AIN1Ϊ�����룬���ɸ���
	ADS1248_WriteReg(ADS_MUX0,0x01);  //00 001 000,Bit7-6:����������Դ��ⲻʹ�ã�Bit5-3:������ΪAIN0��Bit2-0:������ΪAIN1
	ADS1248_delay(1);
	
  ADS1248_WriteReg(ADS_VBIAS,0x00); //00000000,Bit7-0:ƫ�õ�ѹѡ��رգ�Ĭ�ϣ�
	ADS1248_delay(1);
	//�̶�ѡ���ⲿREF1��Ϊ�ο���ѹԴ�����ɸ���
	ADS1248_WriteReg(ADS_MUX1,0x00); //000 00 000,Bit7�������ڲ�����Bit6-5:�ڲ���׼һֱ�رգ�Bit4-3:�����׼Ϊ�ⲿREF1��Bit2-0:��ͨģʽ
	ADS1248_delay(1);

//	//�̶������ɸ���
//	ADS1248_WriteReg(ADS_OFC0,0x00); //00000000
//	ADS1248_delay(1);
//	ADS1248_WriteReg(ADS_OFC1,0x00); //00000000
//	ADS1248_delay(1);
//	ADS1248_WriteReg(ADS_OFC2,0x00); //00000000
//	ADS1248_delay(1);
//	ADS1248_WriteReg(ADS_FSC0,0x00); //00000000
//	ADS1248_delay(1);	
//	ADS1248_WriteReg(ADS_FSC1,0x00); //00000000
//	ADS1248_delay(1);	
//	ADS1248_WriteReg(ADS_FSC2,0x00); //00000000
//	ADS1248_delay(1);

	ADS1248_WriteReg(ADS_IDAC0,0x00); //0000 0000,Bit7-4:ֻ����Bit3:DRDY/DOUT���ܸ���Dout�� ��Bit2-0:�ر��ڲ�����Դ 	0x06,����Ϊ1mA
	ADS1248_delay(1);
	
	//GPIO��Ӧ�˿ڳ�ʼ������������Ϊ��������˿ڣ����ɸ���
	ADS1248_WriteReg(ADS_GPIOCFG,0xec); //11101100,Bit7-0:���ö�Ӧ�˿�Ϊģ��ڻ���GPIO����1��ΪGPIO
	ADS1248_delay(1);
	ADS1248_WriteReg(ADS_GPIODIR,0x13); //0001 0011,Bit7-0:���ö�ӦGPIO�˿�Ϊ���뻹�������0Ϊ�����1Ϊ����
	ADS1248_delay(1);
	
	//����Դ���ã���ѹ������1mA��������
	ADS1248_WriteReg(ADS_IDAC1,0xff); //1111 1111,Bit7-4      ����Ϊ0x8f ����1mA������ADS1248��IEXC1���
	ADS1248_delay(1);
	
	//��������Ӧ�����������ٶȿ�ѡ��5��10�� 20��40��80��160��320��640��1000��2000SPS
	ADS1248_WriteReg(ADS_SYS0,Sample_Speed | 0x70); //0000 0000��Bit7:��������Ϊ0��Bit6-4��ѡ�����棺1-128����Bit3-0��ѡ������ٶ� ��5-2000SPS
	ADS1248_delay(1);
	
	//GPIO�˿ڲ��������õ������ͣ�����
	ADS1248_WriteReg(ADS_GPIODAT,Bridge_Type); //1000 0100,Bit7-0:GPIO��д�ڣ�������������1/4��
	ADS1248_delay(1);
}

/*******************************************************************************
* Function Name  : ADS1248_CFG_SET
* Description    : ADS1248��ͨ�����ú��������������͡�����Դ�������ٶ�
									 ADS1248�ڿ�������ʱ�����ݵ��������������ͣ������ݵ���Чλ�½�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADS1248_CFG_SET(void)
{

/*******************************************************
�����ٶȿ�ѡ��5��10�� 20��40��80��160��320��640��1000��2000SPS
����Դ���ã���ѹ������1mA��������
GPIO�˿ڲ��������õ������ͣ�����,,0x84:������1/4��,0x04:������1/4��,0x04:1/2��,0x00:ȫ��	
Ĭ�ϲ�������5SPS����ѹ������������1/4��
�������У��ȴ�����1���ݣ����ݷ�Ϊ���࣬����+����
ʵ�⣬��ADS1248�����ٶ�������40sps����ʱ�������������������ڸ߾��Ȳ���ʱ��Ӱ�쾫��
ͨ��1����,����ͨ��ʹ�ܣ��������õ������ͣ����������˲�����������������Kֵ
ADS1248�ڳ�ʼ����ɺ��һ�ζ���ʱ������û�нӳ�ת���������
����ڲ��������У�������ת���������������������������һ��ת��
�����ݣ����Ե�һ�ζ���ʱ����������0x7fffffff�����μ��ɣ�
��ʼ����ʱ��3�������ɶ�ʱ��3��ʱ���
************************************************/
//	switch(USART2_RXDATA[3])////�ɼ��ٶ�����
//	{
//		case 0x01:
//			Set_Sample_Speed = 0x06;//����ADS1248��������320SPS
//			TIM_ACC=20;							//40sps  Ƶ��=800/TIM_ACC
//			break;
//		case 0x02:
//			Set_Sample_Speed = 0x04;//����ADS1248��������160SPS
//			TIM_ACC=80;							//20sps
//			break;
//		case 0x03:
//			Set_Sample_Speed = 0x02;//����ADS1248��������10SPS
//			TIM_ACC=160;						//5sps
//			break;
//		case 0x04:
//			Set_Sample_Speed = 0x00;//����ADS1248��������5SPS
//			TIM_ACC=800;						//1sps
//			break;
//		default:
//			break;	
//	}
	


	Channel_Select=*(data_p+8);
	if(Channel_Select & 0x01)//ͨ��A��������
	{
		A_Channel_CFG.Bridge_Type=*(data_p+9);
		EEPROM_DATA.A_Filter_Val=*(data_p+10);//����������˲������������������
		EEPROM_DATA.A_K_Val=(short int)((*(data_p+11)<<8)|(*(data_p+12)));//���������Ӧ��ƬKֵ�����������
		
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+10,(unsigned char *)&EEPROM_DATA.A_Filter_Val,sizeof(EEPROM_DATA.A_Filter_Val));
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+8,(unsigned char *)&EEPROM_DATA.A_K_Val,sizeof(EEPROM_DATA.A_K_Val));
		switch(A_Channel_CFG.Bridge_Type)
		{
			case 0x00:
				Set_Bridge_Type=TWO_WIRE_SINGLE_BRIDGE;
				break;
			case 0x01:
				Set_Bridge_Type=THREE_WIRE_SINGLE_BRIDGE;
				break;
			case 0x02:
				Set_Bridge_Type=HALF_BRIDGE;
				break;
			case 0x04:
				Set_Bridge_Type=FULL_BRIDGE;
				break;
			default:
				//Return_Ack(PARAMETERS_ILLEGAL);
				break;
		}
		SPI1_CS1_LOW();
		ADS1248_Init(Set_Sample_Speed,Set_Bridge_Type);
		//ADS1248_WriteReg(ADS_SYS0,Set_Sample_Speed);//ͨ��A��δ��Ӧ��Ƭʱ�����������ѹ�ϴ󣬴˴�����������Ŵ�
		ADS1248_WriteByte(ADS_RDATA);
		Delay(210);
		SPI1_ALLCS_H();
		//I2C1_ReadS_24C(EEPROM_DATA_BASEADDRESS,(unsigned char *)&EEPROM_DATA.A_Zero_offset,sizeof(EEPROM_DATA.A_Zero_offset));//��EEPROM�д洢�Ĺ���ֵ
		A_Channel_CFG_Flag=SUCCESS;//��λͨ��A����
#ifdef	Debug_EN
		printf("Channel A Configuration Finished!\r\n");
#endif
	}
	if(Channel_Select & 0x02)//ͨ��B��������
	{
		B_Channel_CFG.Bridge_Type=*(data_p+9);
		EEPROM_DATA.B_Filter_Val=*(data_p+10);
		EEPROM_DATA.B_K_Val=(short int)((*(data_p+11)<<8)|(*(data_p+12)));
		
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+21,(unsigned char *)&EEPROM_DATA.B_Filter_Val,sizeof(EEPROM_DATA.B_Filter_Val));
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+19,(unsigned char *)&EEPROM_DATA.B_K_Val,sizeof(EEPROM_DATA.B_K_Val));
		switch(B_Channel_CFG.Bridge_Type)
		{
			case 0x00:
				Set_Bridge_Type=TWO_WIRE_SINGLE_BRIDGE;
				break;
			case 0x01:
				Set_Bridge_Type=THREE_WIRE_SINGLE_BRIDGE;
				break;
			case 0x02:
				Set_Bridge_Type=HALF_BRIDGE;
				break;
			case 0x04:
				Set_Bridge_Type=FULL_BRIDGE;
				break;
			default:
				//Return_Ack(PARAMETERS_ILLEGAL);
				break;
		}			
		SPI1_CS2_LOW();
		ADS1248_Init(Set_Sample_Speed,Set_Bridge_Type);
		//ADS1248_WriteReg(ADS_SYS0,Set_Sample_Speed);//ͨ��A��δ��Ӧ��Ƭʱ�����������ѹ�ϴ󣬴˴�����������Ŵ�
		ADS1248_WriteByte(ADS_RDATA);
		Delay(210);
		SPI1_ALLCS_H();
		//I2C1_ReadS_24C(EEPROM_DATA_BASEADDRESS+11,(unsigned char *)&EEPROM_DATA.B_Zero_offset,sizeof(EEPROM_DATA.B_Zero_offset));//��EEPROM�д洢�Ĺ���ֵ
		B_Channel_CFG_Flag=SUCCESS;
#ifdef	Debug_EN
		printf("Channel B Configuration Finished!\r\n");
#endif
	}
	if(Channel_Select & 0x04)//ͨ��C��������
	{
		C_Channel_CFG.Bridge_Type=*(data_p+9);
		EEPROM_DATA.C_Filter_Val=*(data_p+10);
		EEPROM_DATA.C_K_Val=(short int)((*(data_p+11)<<8)|(*(data_p+12)));
		
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+32,(unsigned char *)&EEPROM_DATA.C_Filter_Val,sizeof(EEPROM_DATA.C_Filter_Val));
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+30,(unsigned char *)&EEPROM_DATA.C_K_Val,sizeof(EEPROM_DATA.C_K_Val));
		switch(C_Channel_CFG.Bridge_Type)
		{
			case 0x00:
				Set_Bridge_Type=TWO_WIRE_SINGLE_BRIDGE;
				break;
			case 0x01:
				Set_Bridge_Type=THREE_WIRE_SINGLE_BRIDGE;
				break;
			case 0x02:
				Set_Bridge_Type=HALF_BRIDGE;
				break;
			case 0x04:
				Set_Bridge_Type=FULL_BRIDGE;
				break;
			default:
				//Return_Ack(PARAMETERS_ILLEGAL);
				break;
		}			
		SPI1_CS3_LOW();
		ADS1248_Init(Set_Sample_Speed,Set_Bridge_Type);
		//ADS1248_WriteReg(ADS_SYS0,Set_Sample_Speed);//ͨ��A��δ��Ӧ��Ƭʱ�����������ѹ�ϴ󣬴˴�����������Ŵ�
		ADS1248_WriteByte(ADS_RDATA);
		Delay(210);
		SPI1_ALLCS_H();
		//I2C1_ReadS_24C(EEPROM_DATA_BASEADDRESS+22,(unsigned char *)&EEPROM_DATA.C_Zero_offset,sizeof(EEPROM_DATA.C_Zero_offset));//��EEPROM�д洢�Ĺ���ֵ
		C_Channel_CFG_Flag=SUCCESS;
#ifdef	Debug_EN
		printf("Channel C Configuration Finished!\r\n");
#endif
	}
	if(Channel_Select & 0x08)//ͨ��D��������
	{
		D_Channel_CFG.Bridge_Type=*(data_p+9);
		EEPROM_DATA.D_Filter_Val=*(data_p+10);
		EEPROM_DATA.D_K_Val=(short int)((*(data_p+11)<<8)|(*(data_p+12)));
		
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+43,(unsigned char *)&EEPROM_DATA.D_Filter_Val,sizeof(EEPROM_DATA.D_Filter_Val));
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+41,(unsigned char *)&EEPROM_DATA.D_K_Val,sizeof(EEPROM_DATA.D_K_Val));
		switch(D_Channel_CFG.Bridge_Type)
		{
			case 0x00:
				Set_Bridge_Type=TWO_WIRE_SINGLE_BRIDGE;
				break;
			case 0x01:
				Set_Bridge_Type=THREE_WIRE_SINGLE_BRIDGE;
				break;
			case 0x02:
				Set_Bridge_Type=HALF_BRIDGE;
				break;
			case 0x04:
				Set_Bridge_Type=FULL_BRIDGE;
				break;
			default:
				//Return_Ack(PARAMETERS_ILLEGAL);
				break;
		}			
		SPI1_CS4_LOW();
		ADS1248_Init(Set_Sample_Speed,Set_Bridge_Type);
		//ADS1248_WriteReg(ADS_SYS0,Set_Sample_Speed);//ͨ��A��δ��Ӧ��Ƭʱ�����������ѹ�ϴ󣬴˴�����������Ŵ�
		ADS1248_WriteByte(ADS_RDATA);
		Delay(210);
		SPI1_ALLCS_H();
		//I2C1_ReadS_24C(EEPROM_DATA_BASEADDRESS+33,(unsigned char *)&EEPROM_DATA.D_Zero_offset,sizeof(EEPROM_DATA.D_Zero_offset));//��EEPROM�д洢�Ĺ���ֵ
		D_Channel_CFG_Flag=SUCCESS;
#ifdef	Debug_EN
		printf("Channel D Configuration Finished!\r\n");
#endif	
	}
	Return_Ack(SUCCESSFUL_EXECUTION);
}














