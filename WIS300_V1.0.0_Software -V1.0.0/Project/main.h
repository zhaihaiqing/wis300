
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

#pragma   pack(1)										    //�ṹ�嵥�ֽڶ��뷽ʽ      #pragma pack(n)

#define   WatchDog_EN
#define		Debug_EN


#define	ADS_PGA							 	        128
#define SystemResetSoft								*((uint32_t *)0xe000ed0c)=0x05fa0004; //ʵ��ϵͳ�����λ


#define TWO_WIRE_SINGLE_BRIDGE				0x84					//�����Ƶ���
#define THREE_WIRE_SINGLE_BRIDGE			0x0c					//�����Ƶ���
#define HALF_BRIDGE										0x04					//����
#define	FULL_BRIDGE										0x20					//ȫ��


//����������Ϣ
#define REQUEST_CHANNEL_CFG  			0x01		//��ѯͨ�����ò���
#define CHANNEL_SETCFG 			 			0x03		//ͨ������
#define REQUEST_CALIBRATION_VAL   0x21		//��ѯУ׼ֵ
#define CALIBRATION_VAL_SET       0x23		//�ֶ�����У׼����
#define AUTO_CALIBRATION_VAL_SET  0x29		//�Զ�У׼
#define SAMPLE_INSTRUCTION        0x41		//��������

//���巵�ش�����Ϣ����
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



//#define Voltage_EXC										1					//�ⲿ2V��ѹ��������ѡ�õ�ѹ����ʱ��Ĭ�Ϲرյ�������
//#define Current_EXC1_OFF							1
////1mA��������
//#define Current_EXC1_50uA							1					//50uA��������
//#define Current_EXC1_100uA						1					//100uA��������
//#define Current_EXC1_250uA						1					//250uA��������
//#define Current_EXC1_500uA						1					//500uA��������
//#define Current_EXC1_750uA						1					//750uA��������
//#define Current_EXC1_1000uA						1					//1000uA��������
//#define Current_EXC1_1500uA						1					//1500uA��������


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
//���ز�ѯ������Ϣ�ṹ��
typedef struct{
	unsigned char Message_type;				//��Ϣ����
	unsigned char A_bridge_type;			//ͨ��A��������
	unsigned char B_bridge_type;			//ͨ��B��������
	unsigned char C_bridge_type;			//ͨ��C��������
	unsigned char D_bridge_type;			//ͨ��D��������
	unsigned char A_filter_val; 			//ͨ��A�˲�������
	unsigned char B_filter_val; 			//ͨ��B�˲�������
	unsigned char C_filter_val; 			//ͨ��C�˲�������
	unsigned char D_filter_val; 			//ͨ��D�˲�������
	unsigned char A_k_val_h;   			  //ͨ��A Kֵ���ֽ�
	unsigned char A_k_val_l;   			  //ͨ��A Kֵ���ֽ�
	unsigned char B_k_val_h;   			  //ͨ��B Kֵ���ֽ�
	unsigned char B_k_val_l;   			  //ͨ��B Kֵ���ֽ�
	unsigned char C_k_val_h;   			  //ͨ��C Kֵ���ֽ�
	unsigned char C_k_val_l;   			  //ͨ��C Kֵ���ֽ�
	unsigned char D_k_val_h;   			  //ͨ��D Kֵ���ֽ�
	unsigned char D_k_val_l;   			  //ͨ��D Kֵ���ֽ�
	unsigned char Reserve1;   			  //�����ֽ�1
	unsigned char Reserve2;   			  //�����ֽ�2
}Channel_CFG_tagdef;

//����У׼�����ṹ��
typedef struct{
	unsigned char Message_type;					//��Ϣ����
	unsigned char Channel;							//ͨ��
	unsigned char Calibration_zero[4];  //0��ֵ���ֽ�
	unsigned char Calibration_half[4];  //75%���̸��ֽ�
	unsigned char Reserve1;   			  	//�����ֽ�1
	unsigned char Reserve2;   			  	//�����ֽ�2
}Calibration_return_tagdef;

//���ز�������ṹ��
typedef struct{
	unsigned char Message_type;				//��Ϣ����
	unsigned char Channel;						//ͨ��
	unsigned char Result[4];					//���ֵ
	unsigned char Reserve1;						//�����ֽ�1
	unsigned char Reserve2;						//�����ֽ�2
}Sample_results_tagdef;

//����ACK�ṹ��
typedef struct{											//������Ϣ����
	unsigned char Message_type;				//��Ϣ����
	unsigned char Error_type;					//��������
	unsigned char Reserve1;						//�����ֽ�1
	unsigned char Reserve2;						//�����ֽ�2
}Ack_tagdef;

//���ղ�ѯ��Ϣ�ṹ��
typedef struct{
	//unsigned char Message_type;	
	unsigned char Reserve1;						//�����ֽ�1
	unsigned char Reserve2;						//�����ֽ�2
}Inquire_CFG_tagdef;


//
//typedef struct{
//	//unsigned char Message_type;	
//	unsigned char Bridge_Type;			//��������
//	unsigned char Filter_Val;				//�˲�������
//	short int K_Val;								//����Kֵ
//	signed long adc;								//����ADC����ֵ
//	signed long Old_DATA;						//�����ϴβ���ֵ
//	float adcV;											//����ADC�������ݣ���ѹֵ��float��
//	float strainVal;								//����Ӧ��ֵ
//	float Zero_offset;							//����0��׼��ֵ
//	float Half_offset;							//������ֵ
//	unsigned char Reserve1;					//�����ֽ�1
//	unsigned char Reserve2;					//�����ֽ�2
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



//����������Ϣ�ṹ��
typedef struct{
	//unsigned char Message_type;	
	unsigned char Bridge_Type;			//��������
	//unsigned char Filter_Val;				//�˲�������
	//short int K_Val;								//����Kֵ
	signed long adc;								//����ADC����ֵ
	signed long Old_DATA;						//�����ϴβ���ֵ
	float adcV;											//����ADC�������ݣ���ѹֵ��float��
	float strainVal;								//����Ӧ��ֵ
	//float Zero_offset;							//����0��׼��ֵ
	//float Half_offset;							//������ֵ
	unsigned char Reserve1;					//�����ֽ�1
	unsigned char Reserve2;					//�����ֽ�2
}Channel_SET_tagdef;

//���ն�ȡУ׼����
typedef struct{
	unsigned char Channel_Select;
	unsigned char Reserve1;					//�����ֽ�1
	unsigned char Reserve2;					//�����ֽ�2	
}Inquire_CALIBRATION_CFG_tagdef;

//�����ֶ�У׼����
typedef struct{
	unsigned char Channel_Select;
	short int Zero;
	short int Half;
	unsigned char Reserve1;					//�����ֽ�1
	unsigned char Reserve2;					//�����ֽ�2
}CALIBRATION_tagdef;

//�����Զ�У׼
typedef struct{
	//unsigned char Message_type;	
	unsigned char Channel_Select;
	unsigned char CALIBRATION_type;
	unsigned char Reserve1;			//�����ֽ�1
	unsigned char Reserve2;			//�����ֽ�2
}AUTO_CALIBRATION_tagdef;

//���ղ���ָ��ṹ��
typedef struct{								//�������ָ��
	//unsigned char Message_type;	
	unsigned char Channel_OE;		//ͨ��ʹ��
	unsigned char Sample_Rate;	//��������
	unsigned char Sample_Type;	//��������
	short int     Sample_Time;	//��������
	unsigned char Reserve1;			//�����ֽ�1
	unsigned char Reserve2;			//�����ֽ�2
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
extern uint16_t 	DelayTime;//��ʱ����ֵ(ms)
extern uint8_t 	DelayTimeFlag;//��ʱ��־
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


