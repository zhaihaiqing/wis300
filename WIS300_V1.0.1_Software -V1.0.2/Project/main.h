
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
#include "EXTI.h"

#pragma   pack(1)										    //�ṹ�嵥�ֽڶ��뷽ʽ      #pragma pack(n)

#define   WatchDog_EN
#define	  Debug_EN


#define	ADS_PGA							 	        128
#define SystemResetSoft								*((uint32_t *)0xe000ed0c)=0x05fa0004; //ʵ��ϵͳ�����λ
//#define LED1_ON													GPIOB->BSRRH = GPIO_Pin_14
//#define LED1_OFF												GPIOB->BSRRL = GPIO_Pin_14
#define TPA_COMMAND_INTERVAL_TIME     5


#define TWO_WIRE_SINGLE_BRIDGE				0x84					//�����Ƶ���
#define THREE_WIRE_SINGLE_BRIDGE			0x0c					//�����Ƶ���
#define HALF_BRIDGE										0x04					//����
#define	FULL_BRIDGE										0x20					//ȫ��


//����������Ϣ
#define REQUEST_CHANNEL_CFG  							1		//��ѯͨ�����ò���
#define CHANNEL_SETCFG 			 							3		//ͨ������
#define REQUEST_CFG_AND_CALIBRATION				5		//��ѯͨ�����ò�����У׼����
#define REQUEST_CALIBRATION_VAL   				21		//��ѯУ׼ֵ
#define CALIBRATION_VAL_SET       				23		//�ֶ�����У׼����
#define AUTO_CALIBRATION_VAL_SET  				29		//�Զ�У׼
#define SAMPLE_INSTRUCTION        				41		//��������

//���巵�ش�����Ϣ����
#define SUCCESSFUL_EXECUTION      0x00
#define DATA_ERROR      					0x02
#define HADRWARE_NOTREADY					0x03
#define SAMPLING									0x05
#define PARAMETERS_ILLEGAL				0x06


#define Ch_A_ID										0x01
#define Ch_B_ID										0x02
#define Ch_C_ID										0x04
#define Ch_D_ID										0x08



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

#define A_Ch_START				EXTI->IMR |= 1<<10
#define A_Ch_STOP				  EXTI->IMR &= 0<<10

#define B_Ch_START				EXTI->IMR |= 1<<2
#define B_Ch_STOP				  EXTI->IMR &= 0<<2

#define C_Ch_START				EXTI->IMR |= 1<<1
#define C_Ch_STOP				  EXTI->IMR &= 0<<1

#define D_Ch_START				EXTI->IMR |= 1<<0
#define D_Ch_STOP				  EXTI->IMR &= 0<<0

#define htons(n) ((((n) & 0x00ff) << 8) | (((n) & 0xff00) >> 8))
#define ntohs(n) htons(n)
#define htonl(n) ((((n) & 0x000000ff) << 24) |  \
                  (((n) & 0x0000ff00) << 8)  |  \
                  (((n) & 0x00ff0000) >> 8)  |  \
                  (((n) & 0xff000000) >> 24))
#define ntohl(n) htonl(n)






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
	unsigned char Message;
	unsigned char Error_type;					//��������
	unsigned char Reserve1;						//�����ֽ�1
	unsigned char Reserve2;						//�����ֽ�2
}Ack_tagdef;

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
	unsigned char A_Bridge_Type;
	unsigned char B_Bridge_Type;
	unsigned char C_Bridge_Type;
	unsigned char D_Bridge_Type;
}EEPROM_DATA_tagdef;



//���ղ�ѯ��Ϣ�ṹ��
typedef struct{
	unsigned char Message_type;	
	unsigned char Reserve1;						//�����ֽ�1
	unsigned char Reserve2;						//�����ֽ�2
}Inquire_CFGInfo_tagdef;


//����������Ϣ�ṹ��
typedef struct{
	unsigned char Message_type;
	unsigned char Ch_Select;
	unsigned char Bridge_Type;			//��������
	unsigned char Filter_Val;
	short    int  K_Val;
	unsigned char Reserve1;					//�����ֽ�1
	unsigned char Reserve2;					//�����ֽ�2
}Channel_SET_tagdef;

//���ն�ȡУ׼����
typedef struct{
	unsigned char Message_type;
	//unsigned char Ch_Select;
	unsigned char Reserve1;					//�����ֽ�1
	unsigned char Reserve2;					//�����ֽ�2	
}Inquire_CALIBRATION_CFG_tagdef;

//�����ֶ�У׼����
typedef struct{
	unsigned char Message_type;
	unsigned char Ch_Select;
	signed int Zero;
	signed int Half;
	unsigned char Reserve1;					//�����ֽ�1
	unsigned char Reserve2;					//�����ֽ�2
}CALIBRATION_tagdef;

//�����Զ�У׼
typedef struct{
	unsigned char Message_type;	
	unsigned char Ch_Select;
	unsigned char CALIBRATION_Type;
	unsigned char Reserve1;			//�����ֽ�1
	unsigned char Reserve2;			//�����ֽ�2
}AUTO_CALIBRATION_tagdef;

//���ղ���ָ��ṹ��
typedef struct{								//�������ָ��
	unsigned char Message_type;	
	unsigned char Ch_Select;		//ͨ��ʹ��
	unsigned char Sample_Rate;	//��������
	unsigned char Sample_Type;	//��������
	short    int  Sample_Time;	//��������
	unsigned char Ch_DATASize;	//���ݰ�����
	unsigned char Reserve1;			//�����ֽ�1
	unsigned char Reserve2;			//�����ֽ�2
}Sample_Instruction_tagdef;


typedef struct{
	unsigned char Bridge_Type;//��������
	unsigned char Sample_Flag;//ͨ��������־
	unsigned int  Count;       //ͨ������������
	unsigned char Ch_CFG_Flag;//ͨ�����ñ�־��ֻ�ж�Ӧͨ��������ã������ſɿ�ʼ�����򷵻�Ӳ��û׼����
	unsigned char ADS_PGAGain;//ͨ������
	int Ch_DATAbuf[6];        //ͨ���������ݻ���
	signed long Old_DATA;			//�����ϴβ���ֵ
}Ch_DATA_tagdef;


//GPIO
void GPIO_Configuration(void);

void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void Delay(__IO uint32_t nTime);
extern __IO uint32_t TimingDelay;
extern uint16_t 	DelayTime;//��ʱ����ֵ(ms)
extern uint8_t 	DelayTimeFlag;//��ʱ��־

extern unsigned char Send_Flag_TPA;
extern unsigned char Clear_Flag_TPA;
extern unsigned char Set_Sample_Speed , Set_Bridge_Type ;
extern unsigned int  SysTick_Count;
extern unsigned int SampleTime;
extern unsigned char SampleTime_Flag;

extern __IO uint8_t TpaCommandIntervalTime;//����tpaָ��ͼ��ʱ��,��ʱ��Ϊһ��ָ���ѷ������
extern __IO uint8_t TpaCommand_Sampling_Flag;//ָ���־λ,1=���յ�һ��ָ��
extern __IO uint8_t TpaCommandLen;//ָ���

extern unsigned char *data_p;
extern int data_len;
extern Ch_DATA_tagdef 						Ch_A_DATA,Ch_B_DATA,Ch_C_DATA,Ch_D_DATA;
extern Channel_SET_tagdef         Channel_CFG;
extern Sample_Instruction_tagdef  Sample_Control;
extern Inquire_CFGInfo_tagdef 		Inquire_CFG;
extern AUTO_CALIBRATION_tagdef    AUTO_CALIBRATION;
extern EEPROM_DATA_tagdef					EEPROM_DATA;
extern Inquire_CALIBRATION_CFG_tagdef   CALIBRATION_CFG;
extern CALIBRATION_tagdef               CALIBRATION_SET;

#endif


