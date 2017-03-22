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
void ADS1248_CS(unsigned char Ch)
{
			GPIOA->BSRRL=GPIO_Pin_4;//����CS1
			GPIOC->BSRRL=GPIO_Pin_15;//
			GPIOA->BSRRL=GPIO_Pin_0;//
			GPIOA->BSRRL=GPIO_Pin_1;//
			switch(Ch)
			{
				case 0x01:
					GPIOA->BSRRH=GPIO_Pin_4;
					break;
				case 0x02:
					GPIOC->BSRRH=GPIO_Pin_15;
					break;
				case 0x04:
					GPIOA->BSRRH=GPIO_Pin_0;
					break;
				case 0x08:
					GPIOA->BSRRH=GPIO_Pin_1;
					break;
				default:
					break;				
			}
}

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
	ADS1248_WriteByte(ADS_ReadREG+(Reg_addr & 0xf));
	ADS1248_WriteByte(0);
	Reg_addr_data=ADS1248_WriteByte(ADS_NOP);
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
	ADS1248_WriteByte(ADS_WriteREG+(Reg_addr & 0xf));
	ADS1248_WriteByte(0);
	ADS1248_WriteByte(data);
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
	//������ѡ��AIN0��������ѡ��AIN1�����ɸ���
	ADS1248_WriteReg(ADS_MUX0,0x01);  //00 001 000,Bit7-6:����������Դ��ⲻʹ�ã�Bit5-3:������ΪAIN0��Bit2-0:������ΪAIN1
	ADS1248_WriteReg(ADS_VBIAS,0x00); //00000000,Bit7-0:ƫ�õ�ѹѡ��رգ�Ĭ�ϣ�
	//�̶�ѡ���ⲿREF1��Ϊ�ο���ѹԴ�����ɸ���
	ADS1248_WriteReg(ADS_MUX1,0x00); //000 00 000,Bit7�������ڲ�����Bit6-5:�ڲ���׼һֱ�رգ�Bit4-3:�����׼Ϊ�ⲿREF1��Bit2-0:��ͨģʽ
//	//�̶������ɸ���
//	ADS1248_WriteReg(ADS_OFC0,0x00); //00000000
//	ADS1248_WriteReg(ADS_OFC1,0x00); //00000000
//	ADS1248_WriteReg(ADS_OFC2,0x00); //00000000
//	ADS1248_WriteReg(ADS_FSC0,0x00); //00000000
//	ADS1248_WriteReg(ADS_FSC1,0x00); //00000000	
//	ADS1248_WriteReg(ADS_FSC2,0x00); //00000000
	ADS1248_WriteReg(ADS_IDAC0,0x00); //0000 0000,Bit7-4:ֻ����Bit3:DRDY/DOUT���ܸ���Dout�� ��Bit2-0:�ر��ڲ�����Դ 	0x06,����Ϊ1mA
	//GPIO��Ӧ�˿ڳ�ʼ������������Ϊ��������˿ڣ����ɸ���
	ADS1248_WriteReg(ADS_GPIOCFG,0xec); //11101100,Bit7-0:���ö�Ӧ�˿�Ϊģ��ڻ���GPIO����1��ΪGPIO
	ADS1248_WriteReg(ADS_GPIODIR,0x13); //0001 0011,Bit7-0:���ö�ӦGPIO�˿�Ϊ���뻹�������0Ϊ�����1Ϊ����
	//����Դ���ã���ѹ������1mA��������
	ADS1248_WriteReg(ADS_IDAC1,0xff); //1111 1111,Bit7-4      ����Ϊ0x8f ����1mA������ADS1248��IEXC1���
	//��������Ӧ�����������ٶȿ�ѡ��5��10�� 20��40��80��160��320��640��1000��2000SPS
	ADS1248_WriteReg(ADS_SYS0,Sample_Speed | 0x70); //0000 0000��Bit7:��������Ϊ0��Bit6-4��ѡ�����棺1-128����Bit3-0��ѡ������ٶ� ��5-2000SPS
	//GPIO�˿ڲ��������õ������ͣ�����
	ADS1248_WriteReg(ADS_GPIODAT,Bridge_Type); //1000 0100,Bit7-0:GPIO��д�ڣ�������������1/4��
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
	
	memcpy(&Channel_CFG,data_p+7,*(data_p+4));
	Channel_CFG.K_Val=htons(Channel_CFG.K_Val);//�����ֽ�����ת����С�˸�ʽ
		
	if((Channel_CFG.Ch_Select&0x0f)!=ERROR)		//���ͨ��
	{
		if( (Channel_CFG.Bridge_Type==0x00)|(Channel_CFG.Bridge_Type==0x01)| \
			(Channel_CFG.Bridge_Type==0x02)|(Channel_CFG.Bridge_Type==0x04) ) {;}//����������
		else{Return_Ack(CHANNEL_SETCFG,PARAMETERS_ILLEGAL);return;}
	}
	else{Return_Ack(CHANNEL_SETCFG,PARAMETERS_ILLEGAL);return;}

	if(Channel_CFG.Ch_Select & 0x01)//ͨ��A��������
	{
		Ch_A_DATA.Bridge_Type=Channel_CFG.Bridge_Type;
		EEPROM_DATA.A_Filter_Val=Channel_CFG.Filter_Val;//����������˲������������������
		EEPROM_DATA.A_K_Val=Channel_CFG.K_Val;//���������Ӧ��ƬKֵ�����������
		
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+10,(unsigned char *)&EEPROM_DATA.A_Filter_Val,sizeof(EEPROM_DATA.A_Filter_Val));
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+8,(unsigned char *)&EEPROM_DATA.A_K_Val,sizeof(EEPROM_DATA.A_K_Val));
		switch(Ch_A_DATA.Bridge_Type)
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
				break;
		}
		EEPROM_DATA.A_Bridge_Type=Set_Bridge_Type;
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+44,(unsigned char *)&EEPROM_DATA.A_Bridge_Type,sizeof(EEPROM_DATA.A_Bridge_Type));


#ifdef	Debug_EN
		printf("Channel A Configuration Finished!\r\n");
#endif
	}
	if(Channel_CFG.Ch_Select & 0x02)//ͨ��B��������
	{
		Ch_B_DATA.Bridge_Type=Channel_CFG.Bridge_Type;
		EEPROM_DATA.B_Filter_Val=Channel_CFG.Filter_Val;
		EEPROM_DATA.B_K_Val=Channel_CFG.K_Val;
		
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+21,(unsigned char *)&EEPROM_DATA.B_Filter_Val,sizeof(EEPROM_DATA.B_Filter_Val));
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+19,(unsigned char *)&EEPROM_DATA.B_K_Val,sizeof(EEPROM_DATA.B_K_Val));
		switch(Ch_B_DATA.Bridge_Type)
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
				break;
		}	
		EEPROM_DATA.B_Bridge_Type=Set_Bridge_Type;
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+45,(unsigned char *)&EEPROM_DATA.B_Bridge_Type,sizeof(EEPROM_DATA.B_Bridge_Type));

	
		
#ifdef	Debug_EN
		printf("Channel B Configuration Finished!\r\n");
#endif
	}
	if(Channel_CFG.Ch_Select & 0x04)//ͨ��C��������
	{
		Ch_C_DATA.Bridge_Type=Channel_CFG.Bridge_Type;
		EEPROM_DATA.C_Filter_Val=Channel_CFG.Filter_Val;
		EEPROM_DATA.C_K_Val=Channel_CFG.K_Val;
		
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+32,(unsigned char *)&EEPROM_DATA.C_Filter_Val,sizeof(EEPROM_DATA.C_Filter_Val));
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+30,(unsigned char *)&EEPROM_DATA.C_K_Val,sizeof(EEPROM_DATA.C_K_Val));
		switch(Ch_C_DATA.Bridge_Type)
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
				break;
		}

		EEPROM_DATA.C_Bridge_Type=Set_Bridge_Type;
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+46,(unsigned char *)&EEPROM_DATA.C_Bridge_Type,sizeof(EEPROM_DATA.C_Bridge_Type));

	
#ifdef	Debug_EN
		printf("Channel C Configuration Finished!\r\n");
#endif
	}
	if(Channel_CFG.Ch_Select & 0x08)//ͨ��D��������
	{
		Ch_D_DATA.Bridge_Type=Channel_CFG.Bridge_Type;
		EEPROM_DATA.D_Filter_Val=Channel_CFG.Filter_Val;
		EEPROM_DATA.D_K_Val=Channel_CFG.K_Val;
		
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+43,(unsigned char *)&EEPROM_DATA.D_Filter_Val,sizeof(EEPROM_DATA.D_Filter_Val));
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+41,(unsigned char *)&EEPROM_DATA.D_K_Val,sizeof(EEPROM_DATA.D_K_Val));
		switch(Ch_D_DATA.Bridge_Type)
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
				break;
		}		
		
		EEPROM_DATA.D_Bridge_Type=Set_Bridge_Type;
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+47,(unsigned char *)&EEPROM_DATA.D_Bridge_Type,sizeof(EEPROM_DATA.D_Bridge_Type));

#ifdef	Debug_EN
		printf("Channel D Configuration Finished!\r\n");
#endif	
	}
	Return_Ack(CHANNEL_SETCFG,SUCCESSFUL_EXECUTION);
}














