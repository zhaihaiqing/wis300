#ifndef __DATA_Process_H
#define __DATA_Process_H

void Channel_A_Filter(unsigned char A_Filter_K);//����Aͨ��һ���ͺ��˲�������

void ADS1248_Sample(unsigned char Ch);														//����ADS1248���ݴ�����

signed long ADS1248_ReadData(void);								//����ADS1248��������

void Sample_Instruction_Control(void);					//��������ָ����ƺ���


#endif


