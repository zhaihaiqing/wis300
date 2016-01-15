
#include "main.h"
//#include "EEPROM_I2C.h"

//������AT24Cϵ�� ,��������������С���޸�I2C_PAGESIZE����
#define EEPROM_ADDR						0xA0			//EEPROM������ַ����ʽ��1 0 1 0  A2 A1 A0 R/W
#define I2C1_SLAVE_ADDRESS7   0x30     //�����ַ
#define ClockSpeed            200000   //����200K
#define I2C_PAGESIZE					64    //AT24C128Cÿҳ64byte

#define I2C_FLAG_TimeOut  		0xffff     //��ʱ���� 0x5000
#define I2C_LONG_TimeOut  		(10 * I2C_FLAG_TimeOut)

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

										A��������+44   		B��������+45   C��������+46   D��������+47

//	
	*****************************************************************************/


/*******************************************************************************
* Function Name  : I2C1_Configuration
* 
*******************************************************************************/
void I2C1_Configuration(void)
{
	//����GPIO�ṹ�壬����I2C�ṹ��
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
  
  /* Enable I2C1 ��GPIO clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* Reset I2C1 peripheral */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,  ENABLE);
	/* Release reset signal of I2C1 IP */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	
	//���������ӵ���Ӧ�˿���
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
 
  /* Configure I2C1 pins: SCL and SDA --GPIO6 and GPIO7 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���踴��ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  /* I2C1 configuration ------------------------------------------------------*/
  I2C_DeInit(I2C1);//I2C��ʼ��
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//����ΪI2Cģʽ
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//������ֻ����I2C�����ڿ���ģʽ�£�ʱ�ӹ���Ƶ�ʸ���100KHz���²�������
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;//���õ�һ���豸�������ַ
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//ʹ��Ӧ��
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//AT24C64��ַΪ7λ����������7λ
  I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;//����ʱ���ٶȣ�
	I2C_AcknowledgeConfig(I2C1, ENABLE);  //����1�ֽ�1Ӧ��ģʽ
 
  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);	
}
/****************************************************
**������:I2C_Standby_24C
**����:24C�Ƿ�׼������д����ж�
**ע������:�������������Ϊ:��æ
****************************************************/
void I2C_Standby_24C(void)      
{
  unsigned short int SR1_Tmp=0;
  unsigned int timeout=0;
  timeout = I2C_LONG_TimeOut;//���ó�ʱ����
  do
  {
    /*��ʼλ*/
    I2C_GenerateSTART(I2C1, ENABLE);
    /*��SR1*/
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
    /*������ַ(д)*/
    I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
		if(timeout == (I2C_LONG_TimeOut - 1000))I2C1_Configuration();
  }while((!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002)) && (timeout--));

  /**/
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
  /*����ֹͣλ*/    
  I2C_GenerateSTOP(I2C1, ENABLE);
}

/***************************************************
**������:I2C1_ReadS_24C
**����:��ȡEEPROM����ֽڣ����8192
**
***************************************************/
void I2C1_ReadS_24C(unsigned short addr ,unsigned char * pBuffer,unsigned short Length)
{
  unsigned int timeout;
  if(Length==0)return;  //����Ϊ0ֱ�ӷ���
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
	I2C_Standby_24C();
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && (timeout--));

  /*����1�ֽ�1Ӧ��ģʽ*/
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /* ������ʼλ */
  I2C_GenerateSTART(I2C1, ENABLE);
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));/*EV5,��ģʽ*/

  /*����������ַ(д)*/
  I2C_Send7bitAddress(I2C1,  EEPROM_ADDR, I2C_Direction_Transmitter);
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
  while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));

  /*���͵�ַ,,��ַ�������֣��ߵ�ַ���ֽڵ�ַ���͵�ַ��ҳ��ַ*/
  I2C_SendData(I2C1, (addr & 0xff00) >> 8); //���͸�8λ��ַ
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
  while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/
  I2C_SendData(I2C1, addr & 0x00ff); //���͵�8λ��ַ
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
  while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/

  /*��ʼλ*/
  I2C_GenerateSTART(I2C1, ENABLE);
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));

  /*������*/
  I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Receiver);
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && (timeout--));
  while (Length)
  {
    if(Length==1)
		{
			I2C_AcknowledgeConfig(I2C1, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
			I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ
		}

		timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
		while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) && (timeout--)); /* EV7 */
		*pBuffer = I2C_ReceiveData(I2C1);
		pBuffer++;
		/* Decrement the read bytes counter */
		Length--;
  }
  //�ٴ�����Ӧ��ģʽ
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}
/*************************************************
**������:I2C_PageWrite_24C
**����:д��һҳ(����)  ���32byte
*************************************************/
unsigned char I2C_PageWrite_24C(unsigned short addr,unsigned char* pBuffer, unsigned char Length)
{
  unsigned int timeout;
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && (timeout--));
	if(!timeout)return ERROR;

  /*��ʼλ*/
  I2C_GenerateSTART(I2C1, ENABLE);
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));
	if(!timeout)return ERROR;	

  /*������ַ(д)*/
  I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));
	if(!timeout)return ERROR;

  /*д��ֵַ*/
  I2C_SendData(I2C1, (addr & 0xff00) >> 8); //���͸�8λ��ַ
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
  while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/
	if(!timeout)return ERROR;
  I2C_SendData(I2C1, addr & 0x00ff); //���͵�8λ��ַ
  timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
  while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/
	if(!timeout)return ERROR;

  while(Length--)  
  {
    I2C_SendData(I2C1, *pBuffer); 
    pBuffer++; 
    timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
    while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));
		if(!timeout)return ERROR;
  }
  /*ֹͣλ*/
  I2C_GenerateSTOP(I2C1, ENABLE);
	return SUCCESS;
}
/*************************************************
**������:I2C_WriteS_24C
**����:ҳд��24C
*************************************************/
unsigned char I2C_WriteS_24C(unsigned short addr,unsigned char* pBuffer,  unsigned short Length)
{
	unsigned char temp;
	//1.�Ȱ�ҳ������Ĳ���д��
	temp=addr % I2C_PAGESIZE;
	I2C_Standby_24C(); //�ж��Ƿ�æ
	if(temp)
	{
    temp=I2C_PAGESIZE-temp;
    if(Length >= temp)
    {
      I2C_PageWrite_24C(addr,pBuffer,  temp);
      Length-=temp;
      addr+=temp;
      pBuffer+=temp;
    }
    else
    {
      if(I2C_PageWrite_24C(addr,pBuffer,  Length) == ERROR )return ERROR;
      Length = 0;
    }
		I2C_Standby_24C(); //�ж��Ƿ�æ
	}
	//2.��ҳ����ĵط���ʼд
	while(Length)
	{
		if(Length>=I2C_PAGESIZE)
		{
			if(I2C_PageWrite_24C(addr,pBuffer,  I2C_PAGESIZE) == ERROR )return ERROR; //
			Length-=I2C_PAGESIZE;
			addr+=I2C_PAGESIZE;
			pBuffer+=I2C_PAGESIZE;
			I2C_Standby_24C();  //�ж��Ƿ�æ
		}
		else
		{
			if(I2C_PageWrite_24C(addr,pBuffer,  Length) == ERROR )return ERROR;
			Length=0;
			I2C_Standby_24C(); //�ж��Ƿ�æ
		}
	}
}
/*******************************************************************************
* Function Name  : EEPROM_Erase
* Description    : EEPROMȫƬ��������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EEPROM_EraseFullChip(void)
{
	unsigned char  DATA_temp=0x00;
	I2C_WriteS_24C(0x0000,&DATA_temp,16384);
	printf("Erase EEPROM OK!\r\n");
}

/*******************************************************************************
* Function Name  : EEPROM_Check
* Description    : EEPROMУ�������EEPROM��д��8�����ݣ�����������Ƿ�һ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EEPROM_Check(void)
{
	unsigned char counti=0,countj=0;
	unsigned char I2C_WriteData[8]={0x00,0x01,0x02,0x03,0x04,0x05,0x07,0x08};
	unsigned char I2C_ReadData[8];
	printf("Write EEPROM!\r\n");
	for(counti=0;counti<8;counti++)
	{
		printf("0x%x\r\n",I2C_WriteData[counti]);
	}
	I2C_WriteS_24C(0x0000,I2C_WriteData,8);
	Delay(200);
	printf("Read EEPROM!\r\n");
	I2C1_ReadS_24C(0x0000,I2C_ReadData,8);
	for(counti=0;counti<8;counti++)
	{
		printf("0x%x\r\n",I2C_ReadData[counti]);
	}
	for(counti=0;counti<8;counti++)
	{
		if(I2C_ReadData[counti]==I2C_WriteData[counti])
		{
			countj++;
		}
	}
	if(countj==8)
		printf("EEPROM is OK !\r\n");
	else
		printf("EEPROM Data wrong !\r\n");
}




