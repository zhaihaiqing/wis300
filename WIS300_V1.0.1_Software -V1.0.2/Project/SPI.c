/*******************************************************************************
* File Name          : SPI.c
* Description        : ADS1248与STM32L151通信驱动程序
* Author             : zhaihaiqing
* Mail               : zhaihaiqing@foxmail.com
* Created Time       : 2015-06-17 15:16
* Last Update				 :
*******************************************************************************/

#include "main.h"
//#include "spi.h"

/*******************************************************************************
* Function Name  : SPI1_Configuration
* Description    : STM32 SPI1配置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_Configuration(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);													//打开外设时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC , ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);                     //设置外设功能
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_7;					//设置端口模式，PA5=SCK1,PA6=MISO1,PA7=MOSI1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                //外设复用模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;															//推挽模式
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;															//默认下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;														//GPIO翻转速度为40MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);																			//将配置写入对应的GPIO寄存器中

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4;							//设置SPI片选，PA1=CS1，PA0=CS2 ，PA4=CS4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																//端口输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;															//推挽模式
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;																//默认上拉，禁止AD片选
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;														//GPIO翻转速度为2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);																			//将配置写入对应的GPIO寄存器中 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;																	//设置SPI片选，PC15=CS3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																//端口输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;															//推挽模式
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;																//默认上拉，禁止AD片选
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;														//GPIO翻转速度为2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);																			//将配置写入对应的GPIO寄存器中
	
	/* SPI1 设置 */
	SPI_Cmd(SPI1, DISABLE);
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   	//SPI1设置为两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;									       	//设置SPI1为主模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;											//SPI发送接收8位帧结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;														//串行时钟空闲 低电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;													//第二个时钟沿开始采样数据
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//NSS信号由软件（使用SSI位）管理
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为2
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;										//数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;															//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);
	
  SPI_Cmd(SPI1, ENABLE); 
}
/*******************************************************************************
* Function Name  : SPI1_CSx_LOW
* Description    : SPI的片选信号，
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADS1248_CS(unsigned char Ch)
{
			GPIOA->BSRRL=GPIO_Pin_4;//拉低CS1
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
* Description    : STM32 从SPI总线上读一个字节数据
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
* Description    : STM32 向SPI1总线写一个字节数据
* Input          : 数据，无符号char型
* Output         : None
* Return         : 16位无符号整型
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
* Description    : STM32 通过SPI1读ADS1248寄存器值
* Input          : 地址
* Output         : None
* Return         : 寄存器值
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
* Description    : STM32 通过SPI1总线向ADS1248对应寄存器中写数据
* Input          : 寄存器地址及数据
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
* Description    : ADS1248初始化函数，配置测量过程中不可更改的寄存器
ADS1248初始化函数，配置桥类型、激励源、采样速度
									 ADS1248在开启增益时，数据的噪声度有所降低，但数据的有效位下降
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADS1248_Init(unsigned char Sample_Speed,unsigned char Bridge_Type)
{                       
	//正输入选择AIN0，负输入选择AIN1，不可更改
	ADS1248_WriteReg(ADS_MUX0,0x01);  //00 001 000,Bit7-6:传感器电流源检测不使用，Bit5-3:正输入为AIN0，Bit2-0:负输入为AIN1
	ADS1248_WriteReg(ADS_VBIAS,0x00); //00000000,Bit7-0:偏置电压选择关闭（默认）
	//固定选择外部REF1作为参考电压源，不可更改
	ADS1248_WriteReg(ADS_MUX1,0x00); //000 00 000,Bit7：启用内部晶振，Bit6-5:内部基准一直关闭，Bit4-3:输入基准为外部REF1，Bit2-0:普通模式
//	//固定，不可更改
//	ADS1248_WriteReg(ADS_OFC0,0x00); //00000000
//	ADS1248_WriteReg(ADS_OFC1,0x00); //00000000
//	ADS1248_WriteReg(ADS_OFC2,0x00); //00000000
//	ADS1248_WriteReg(ADS_FSC0,0x00); //00000000
//	ADS1248_WriteReg(ADS_FSC1,0x00); //00000000	
//	ADS1248_WriteReg(ADS_FSC2,0x00); //00000000
	ADS1248_WriteReg(ADS_IDAC0,0x00); //0000 0000,Bit7-4:只读，Bit3:DRDY/DOUT功能复用Dout线 ，Bit2-0:关闭内部恒流源 	0x06,设置为1mA
	//GPIO对应端口初始化，，，设置为数字输出端口，不可更改
	ADS1248_WriteReg(ADS_GPIOCFG,0xec); //11101100,Bit7-0:设置对应端口为模拟口还是GPIO，置1设为GPIO
	ADS1248_WriteReg(ADS_GPIODIR,0x13); //0001 0011,Bit7-0:设置对应GPIO端口为输入还是输出，0为输出，1为输入
	//激励源设置，电压激励或1mA电流激励
	ADS1248_WriteReg(ADS_IDAC1,0xff); //1111 1111,Bit7-4      设置为0x8f 设置1mA电流从ADS1248的IEXC1输出
	//增益自适应，，，测量速度可选择5、10、 20、40、80、160、320、640、1000、2000SPS
	ADS1248_WriteReg(ADS_SYS0,Sample_Speed | 0x70); //0000 0000，Bit7:必须设置为0，Bit6-4：选择增益：1-128倍，Bit3-0：选择测量速度 ：5-2000SPS
	//GPIO端口操作，设置电桥类型，四种
	ADS1248_WriteReg(ADS_GPIODAT,Bridge_Type); //1000 0100,Bit7-0:GPIO读写口，，，，两线制1/4桥
}

/*******************************************************************************
* Function Name  : ADS1248_CFG_SET
* Description    : ADS1248四通道设置函数，配置桥类型、激励源、采样速度
									 ADS1248在开启增益时，数据的噪声度有所降低，但数据的有效位下降
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADS1248_CFG_SET(void)
{
/*******************************************************
测量速度可选择5、10、 20、40、80、160、320、640、1000、2000SPS
激励源设置，电压激励或1mA电流激励
GPIO端口操作，设置电桥类型，四种,,0x84:两线制1/4桥,0x04:三线制1/4桥,0x04:1/2桥,0x00:全桥	
默认采样速率5SPS，电压激励，两线制1/4桥
主函数中，等待串口1数据，数据分为两类，设置+命令
实测，当ADS1248采样速度设置在40sps以上时，测量噪声明显增大，在高精度测量时会影响精度
通道1设置,配置通道使能，，，配置电桥类型，，，配置滤波器参数，，，配置K值
ADS1248在初始化完成后第一次读数时，由于没有接出转换完成引脚
因而在采样函数中，发送完转换命令后立即读数，读到的是上一次转换
的数据，所以第一次读数时，读到的是0x7fffffff，屏蔽即可；
初始化定时器3，采样由定时器3定时完成
************************************************/
	
	memcpy(&Channel_CFG,data_p+7,*(data_p+4));
	Channel_CFG.K_Val=htons(Channel_CFG.K_Val);//将多字节数据转换成小端格式
		
	if((Channel_CFG.Ch_Select&0x0f)!=ERROR)		//检查通道
	{
		if( (Channel_CFG.Bridge_Type==0x00)|(Channel_CFG.Bridge_Type==0x01)| \
			(Channel_CFG.Bridge_Type==0x02)|(Channel_CFG.Bridge_Type==0x04) ) {;}//检查电桥类型
		else{Return_Ack(CHANNEL_SETCFG,PARAMETERS_ILLEGAL);return;}
	}
	else{Return_Ack(CHANNEL_SETCFG,PARAMETERS_ILLEGAL);return;}

	if(Channel_CFG.Ch_Select & 0x01)//通道A参数设置
	{
		Ch_A_DATA.Bridge_Type=Channel_CFG.Bridge_Type;
		EEPROM_DATA.A_Filter_Val=Channel_CFG.Filter_Val;//如果更改了滤波器参数，将其存起来
		EEPROM_DATA.A_K_Val=Channel_CFG.K_Val;//如果更改了应变片K值，将其存起来
		
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
	if(Channel_CFG.Ch_Select & 0x02)//通道B参数设置
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
	if(Channel_CFG.Ch_Select & 0x04)//通道C参数设置
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
	if(Channel_CFG.Ch_Select & 0x08)//通道D参数设置
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














