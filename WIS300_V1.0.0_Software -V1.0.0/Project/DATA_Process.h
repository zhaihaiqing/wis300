#ifndef __DATA_Process_H
#define __DATA_Process_H




void Channel_A_Filter(unsigned char A_Filter_K);//����Aͨ��һ���ͺ��˲�������
void Channel_B_Filter(unsigned char B_Filter_K);//����Bͨ��һ���ͺ��˲�������
void Channel_C_Filter(unsigned char C_Filter_K);//����Cͨ��һ���ͺ��˲�������
void Channel_D_Filter(unsigned char D_Filter_K);//����Dͨ��һ���ͺ��˲�������

void ADS1248_B(void);														//����ADS1248���ݴ�����

signed long ADS1248_Sample(void);								//����ADS1248��������

void Sample_Instruction_Control(void);					//��������ָ����ƺ���





#endif


