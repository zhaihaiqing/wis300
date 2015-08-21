/*******************************************************************************
* File Name          : SPI.h
* Author             : zhaihaiqing
* Mail               : zhaihaiqing@foxmail.com
* Created Time       : 2015-06-17 15:16
* Last Update				 :
* Description        : ADS1248及STM32L151 SPI相关信息
*******************************************************************************/
#ifndef __SPI_H
#define __SPI_H

#define Apply_Filter


#define SPI_FLAG_TimeOut  0x5000   //超时常量
#define SPI_LONG_TimeOut  (10 * SPI_FLAG_TimeOut)
#define Dummy_Byte 0x00  //不考虑的数据

#define ADS1248_CS1_LOW											GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define ADS1248_CS1_H												GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define ADS1248_CS2_LOW											GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define ADS1248_CS2_H												GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define ADS1248_CS3_LOW											GPIO_ResetBits(GPIOC,GPIO_Pin_15)
#define ADS1248_CS3_H												GPIO_SetBits(GPIOC,GPIO_Pin_15)

#define ADS1248_CS4_LOW											GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define ADS1248_CS4_H												GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define ADS1248_POWER_LOW									GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define ADS1248_POWER_H											GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define ADS1248_SPI      										SPI1
#define ADS1248_READY_PIN()                	GPIOA->IDR & GPIO_Pin_11


/********************************************/
/* ADS1248 register map   寄存器地址        */
/*******************************************/
#define ADS_MUX0						0x00
#define ADS_VBIAS     			0x01
#define ADS_MUX1      			0x02
#define ADS_SYS0      			0x03
#define ADS_OFC0						0x04
#define ADS_OFC1						0x05
#define ADS_OFC2						0x06
#define ADS_FSC0						0x07
#define ADS_FSC1						0x08
#define ADS_FSC2						0x09
#define ADS_IDAC0						0x0a
#define ADS_IDAC1						0x0b
#define ADS_GPIOCFG					0x0c
#define ADS_GPIODIR					0x0d
#define ADS_GPIODAT					0x0e

/******************************************/
/* ADS1248 SPI Commands     SPI命令        */
/*******************************************/
#define ADS_WAKEUP					0x00
#define ADS_SLEEP						0x02
#define ADS_SYNC						0x04
#define ADS_RESET						0x06
#define ADS_NOP							0xff
#define ADS_RDATA						0x12
#define ADS_RDATAC					0x14
#define ADS_SDATAC					0x16
#define ADS_ReadREG					0x20
#define ADS_WriteREG				0x40
#define ADS_SYSOCAL					0x60
#define ADS_SYSGCAL					0x61
#define ADS_SELFOCAL				0x62

/************************************************************************/
/* ads1248 macroinstruction                                             */
/************************************************************************/
// about MUX0: Multiplexer Control Register 0
#define  P_AIN0                    0x00
#define  P_AIN1                    0x08
#define  P_AIN2                    0x10
#define  P_AIN3                    0x18
#define  P_AIN4                    0x20
#define  P_AIN5                    0x28
#define  P_AIN6                    0x30
#define  P_AIN7                    0x38
#define  N_AIN0                    0x00
#define  N_AIN1                    0x01
#define  N_AIN2                    0x02
#define  N_AIN3                    0x03
#define  N_AIN4                    0x04
#define  N_AIN5                    0x05
#define  N_AIN6                    0x06
#define  N_AIN7                    0x07

// about MUX1: Multiplexer Control Register 1
#define  CLK_Inter                  0x00
#define  CLK_Exter                  0x80
#define  REF_Inter_AlwaysOn         0x20  // selecte internal reference and always open  
#define  REF_Inter_AlwaysOff        0x00  // selecte internal reference and always off 
#define  SELT_REF0                  0x00                                
#define  SELT_REF1                  0x08
#define  SELT_Inter                 0x10
#define  SELT_Inter_REF0            0x18

// about SYS0 : System Control Register 0
#define         PGAGAIN_1           0x00
#define         PGAGAIN_2           0x10
#define         PGAGAIN_4           0x20
#define         PGAGAIN_8           0x30
#define         PGAGAIN_16          0x40
#define         PGAGAIN_32          0x50
#define         PGAGAIN_64          0x60
#define         PGAGAIN_128         0x70
#define         DATARATE_5          0x00
#define        	DATARATE_10         0x01
#define         DATARATE_20         0x02
#define         DATARATE_40         0x03
#define         DATARATE_80         0x04
#define         DATARATE_160        0x05
#define         DATARATE_320        0x06
#define         DATARATE_640        0x07
#define         DATARATE_1000       0x08
#define         DATARATE_2000       0x09

// about IDAC0: IDAC Control Register 0
#define  DRDY_MODE_EN               0x08
#define  DRDY_MODE_DIS              0x00

// the magnitude of the excitation current.
// The IDACs require the internal reference to be on.
#define  		IMAG_Off                 0x00
#define  		IMAG_50                  0x01
#define 		IMAG_100                 0x02
#define 		IMAG_250                 0x03
#define  		IMAG_500                 0x04
#define  		IMAG_750                 0x05
#define  		IMAG_1000                0x06
#define  		IMAG_1500                0x07

// about IDAC1: IDAC Control Register 1
// select the output pin for the first current source DAC.
#define         IDAC1_AIN0           0x00
#define         IDAC1_AIN1           0x10
#define         IDAC1_AIN2           0x20
#define         IDAC1_AIN3           0x30
#define         IDAC1_AIN4           0x40
#define         IDAC1_AIN5           0x50
#define         IDAC1_AIN6           0x60
#define         IDAC1_AIN7           0x70
#define         IDAC1_IEXT1          0x80
#define         IDAC1_IEXT2          0x90
#define         IDAC1_OFF            0xC0

//select the output pin for the second current source DAC.
#define         IDAC2_AIN0           0x00
#define         IDAC2_AIN1           0x01
#define         IDAC2_AIN2           0x02
#define         IDAC2_AIN3           0x03
#define         IDAC2_AIN4           0x04
#define         IDAC2_AIN5           0x05
#define         IDAC2_AIN6           0x06
#define         IDAC2_AIN7           0x07
#define         IDAC2_IEXT1          0x08
#define         IDAC2_IEXT2          0x09
#define         IDAC2_OFF            0x0C


void ADS1248_delay(unsigned int);																				//声明ADS1248延时函数
void SPI1_Configuration(void);																					//声明SPI配置函数
unsigned char ADS1248_ReadByte(void);																		//声明ADS1248读字节函数
unsigned char ADS1248_WriteByte(unsigned char);													//声明ADS1248写字节函数
unsigned char ADS1248_ReadREG(unsigned char);														//声明ADS1248读寄存器函数
void ADS1248_WriteReg(unsigned char,unsigned char);											//声明ADS1248写寄存器函数
void ADS1248_Init(unsigned char Sample_Speed,unsigned char Bridge_Type);//声明ADS1248初始化函数

void ADS1248_CFG_SET(void);																							//声明ADS1248配置函数

//void EXTI15_10_Config(void);


void SPI1_CS1_LOW(void);												//声明SPI片选1
void SPI1_CS2_LOW(void);												//声明SPI片选2
void SPI1_CS3_LOW(void);												//声明SPI片选3
void SPI1_CS4_LOW(void);												//声明SPI片选4
void SPI1_ALLCS_H(void);												//声明SPI片选全关断






#endif


