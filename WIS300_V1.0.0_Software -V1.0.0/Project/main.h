
#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stm32l1xx.h"
#include "time.h"
#include "uart.h"
#include "spi.h"
#include "EEPROM_i2c.h"
#include "serial_packet.h"
#include "DATA_Process.h"
#include "Return_DATA.h"
#include "Offset.h"

#pragma   pack(1)										    //结构体单字节对齐方式      #pragma pack(n)

#define   WatchDog_EN
#define		Debug_EN


#define	ADS_PGA							 	        128
#define SystemResetSoft								*((uint32_t *)0xe000ed0c)=0x05fa0004; //实现系统软件复位


#define TWO_WIRE_SINGLE_BRIDGE				0x84					//两线制单桥
#define THREE_WIRE_SINGLE_BRIDGE			0x0c					//三线制单桥
#define HALF_BRIDGE										0x04					//半桥
#define	FULL_BRIDGE										0x20					//全桥


//定义配置信息
#define REQUEST_CHANNEL_CFG  			0x01		//查询通道配置参数
#define CHANNEL_SETCFG 			 			0x03		//通道设置
#define REQUEST_CALIBRATION_VAL   0x21		//查询校准值
#define CALIBRATION_VAL_SET       0x23		//手动设置校准参数
#define AUTO_CALIBRATION_VAL_SET  0x29		//自动校准
#define SAMPLE_INSTRUCTION        0x41		//采样命令

//定义返回错误消息类型
#define SUCCESSFUL_EXECUTION      0x00
//#define SAMPLE_COMPLETE						0x01
#define DATA_ERROR      					0x02
#define HADRWARE_NOTREADY					0x03
#define SAMPLING									0x05
#define PARAMETERS_ILLEGAL				0x06
//#define HARDWEAR_READY						0x07


#define Ch_A_ID										0x01
#define Ch_B_ID										0x02
#define Ch_C_ID										0x04
#define Ch_D_ID										0x08



//#define Voltage_EXC										1					//外部2V电压激励，当选用电压激励时，默认关闭电流激励
//#define Current_EXC1_OFF							1
////1mA电流激励
//#define Current_EXC1_50uA							1					//50uA电流激励
//#define Current_EXC1_100uA						1					//100uA电流激励
//#define Current_EXC1_250uA						1					//250uA电流激励
//#define Current_EXC1_500uA						1					//500uA电流激励
//#define Current_EXC1_750uA						1					//750uA电流激励
//#define Current_EXC1_1000uA						1					//1000uA电流激励
//#define Current_EXC1_1500uA						1					//1500uA电流激励


#define 	EEPROM_DATA_BASEADDRESS         0x0040

#define _NOP()    				__ASM("NOP")

#define LED0_ON					  GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define LED0_OFF					GPIO_SetBits(GPIOA,GPIO_Pin_15)

#define TXBOE_ON					GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define TXBOE_OFF					GPIO_ResetBits(GPIOA,GPIO_Pin_0)

#define ANALOGPOW_ON			GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define ANALOGPOW_OFF			GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define FCTR1_H						GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define FCTR1_L						GPIO_ResetBits(GPIOB,GPIO_Pin_14)

#define FCTR2_H						GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define FCTR2_L						GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define FCTR3_H						GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define FCTR3_L						GPIO_ResetBits(GPIOB,GPIO_Pin_12)
//返回查询配置信息结构体
typedef struct{
	unsigned char Message_type;				//消息类型
	unsigned char A_bridge_type;			//通道A电桥类型
	unsigned char B_bridge_type;			//通道B电桥类型
	unsigned char C_bridge_type;			//通道C电桥类型
	unsigned char D_bridge_type;			//通道D电桥类型
	unsigned char A_filter_val; 			//通道A滤波器参数
	unsigned char B_filter_val; 			//通道B滤波器参数
	unsigned char C_filter_val; 			//通道C滤波器参数
	unsigned char D_filter_val; 			//通道D滤波器参数
	unsigned char A_k_val_h;   			  //通道A K值高字节
	unsigned char A_k_val_l;   			  //通道A K值低字节
	unsigned char B_k_val_h;   			  //通道B K值高字节
	unsigned char B_k_val_l;   			  //通道B K值低字节
	unsigned char C_k_val_h;   			  //通道C K值高字节
	unsigned char C_k_val_l;   			  //通道C K值低字节
	unsigned char D_k_val_h;   			  //通道D K值高字节
	unsigned char D_k_val_l;   			  //通道D K值低字节
	unsigned char Reserve1;   			  //保留字节1
	unsigned char Reserve2;   			  //保留字节2
}Channel_CFG_tagdef;

//返回校准参数结构体
typedef struct{
	unsigned char Message_type;					//消息类型
	unsigned char Channel;							//通道
	unsigned char Calibration_zero[4];  //0点值高字节
	unsigned char Calibration_half[4];  //75%量程高字节
	unsigned char Reserve1;   			  	//保留字节1
	unsigned char Reserve2;   			  	//保留字节2
}Calibration_return_tagdef;

//返回采样结果结构体
typedef struct{
	unsigned char Message_type;				//消息类型
	unsigned char Channel;						//通道
	unsigned char Result[4];					//结果值
	unsigned char Reserve1;						//保留字节1
	unsigned char Reserve2;						//保留字节2
}Sample_results_tagdef;

//返回ACK结构体
typedef struct{											//定义消息返回
	unsigned char Message_type;				//消息类型
	unsigned char Error_type;					//错误类型
	unsigned char Reserve1;						//保留字节1
	unsigned char Reserve2;						//保留字节2
}Ack_tagdef;

//接收查询信息结构体
typedef struct{
	//unsigned char Message_type;	
	unsigned char Reserve1;						//保留字节1
	unsigned char Reserve2;						//保留字节2
}Inquire_CFG_tagdef;


//
//typedef struct{
//	//unsigned char Message_type;	
//	unsigned char Bridge_Type;			//电桥类型
//	unsigned char Filter_Val;				//滤波器参数
//	short int K_Val;								//定义K值
//	signed long adc;								//定义ADC采样值
//	signed long Old_DATA;						//定义上次采样值
//	float adcV;											//定义ADC采样数据，电压值，float型
//	float strainVal;								//定义应变值
//	float Zero_offset;							//定义0基准点值
//	float Half_offset;							//半量程值
//	unsigned char Reserve1;					//保留字节1
//	unsigned char Reserve2;					//保留字节2
//}Channel_DATASET_tagdef;


typedef struct{
	float A_Zero_offset;
	float A_Half_offset;
	short int A_K_Val;
	unsigned char A_Filter_Val;
	float B_Zero_offset;
	float B_Half_offset;
	short int B_K_Val;
	unsigned char B_Filter_Val;
	float C_Zero_offset;
	float C_Half_offset;
	short int C_K_Val;
	unsigned char C_Filter_Val;
	float D_Zero_offset;
	float D_Half_offset;
	short int D_K_Val;
	unsigned char D_Filter_Val;
}EEPROM_DATA_tagdef;



//接收配置信息结构体
typedef struct{
	//unsigned char Message_type;	
	unsigned char Bridge_Type;			//电桥类型
	//unsigned char Filter_Val;				//滤波器参数
	//short int K_Val;								//定义K值
	signed long adc;								//定义ADC采样值
	signed long Old_DATA;						//定义上次采样值
	float adcV;											//定义ADC采样数据，电压值，float型
	float strainVal;								//定义应变值
	//float Zero_offset;							//定义0基准点值
	//float Half_offset;							//半量程值
	unsigned char Reserve1;					//保留字节1
	unsigned char Reserve2;					//保留字节2
}Channel_SET_tagdef;

//接收读取校准参数
typedef struct{
	unsigned char Channel_Select;
	unsigned char Reserve1;					//保留字节1
	unsigned char Reserve2;					//保留字节2	
}Inquire_CALIBRATION_CFG_tagdef;

//接收手动校准参数
typedef struct{
	unsigned char Channel_Select;
	short int Zero;
	short int Half;
	unsigned char Reserve1;					//保留字节1
	unsigned char Reserve2;					//保留字节2
}CALIBRATION_tagdef;

//接收自动校准
typedef struct{
	//unsigned char Message_type;	
	unsigned char Channel_Select;
	unsigned char CALIBRATION_type;
	unsigned char Reserve1;			//保留字节1
	unsigned char Reserve2;			//保留字节2
}AUTO_CALIBRATION_tagdef;

//接收采样指令结构体
typedef struct{								//定义采样指令
	//unsigned char Message_type;	
	unsigned char Channel_OE;		//通道使能
	unsigned char Sample_Rate;	//采样速率
	unsigned char Sample_Type;	//采样类型
	short int     Sample_Time;	//采样次数
	unsigned char Reserve1;			//保留字节1
	unsigned char Reserve2;			//保留字节2
}Sample_Instruction_tagdef;


//GPIO
void GPIO_Configuration(void);
//PWM
//void PWM_Configuration(unsigned int PwmFreq,unsigned char DutyCycles);
//void PWM_Enable(void);
//void PWM_Disable(void);
//void Stim(void );
void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void Delay(__IO uint32_t nTime);
extern __IO uint32_t TimingDelay;
extern uint16_t 	DelayTime;//延时计数值(ms)
extern uint8_t 	DelayTimeFlag;//计时标志
//extern unsigned char USART2_RXDATA[9];
//extern unsigned char RxCounter2;
//extern unsigned char work;
//extern unsigned char Wait;
extern unsigned char Sample_Flag;
//extern unsigned char Overrange_Flag;
extern unsigned char A_Channel_CFG_Flag,B_Channel_CFG_Flag,C_Channel_CFG_Flag,D_Channel_CFG_Flag;
extern unsigned char Send_Flag_TPA;
extern unsigned char Set_Sample_Speed , Set_Bridge_Type ;

extern unsigned char ADS1_PGA,ADS2_PGA,ADS3_PGA,ADS4_PGA;

extern float A_HalfSubZero,B_HalfSubZero,C_HalfSubZero,D_HalfSubZero;
extern unsigned int count;
extern unsigned int  SysTick_Count;
extern unsigned int TIM_ACC;
extern unsigned int SampleTime;

extern int A_Channel_DATAbuf[6],B_Channel_DATAbuf[6],C_Channel_DATAbuf[6],D_Channel_DATAbuf[6];
extern unsigned char Channel_DATASize;


extern unsigned char *data_p;
extern unsigned char Channel_Select;
extern Channel_SET_tagdef  A_Channel_CFG,B_Channel_CFG,C_Channel_CFG,D_Channel_CFG;
extern Sample_Instruction_tagdef  Sample_Control;
extern Inquire_CFG_tagdef 				  Inquire_CFG;
extern AUTO_CALIBRATION_tagdef     AUTO_CALIBRATION;
extern EEPROM_DATA_tagdef					EEPROM_DATA;

#endif


