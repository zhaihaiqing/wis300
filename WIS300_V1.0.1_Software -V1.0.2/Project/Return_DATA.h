
#ifndef __Return_DATA_H
#define __Return_DATA_H


void Return_Channel_CFG(void);									//��������ͨ�����ú���
void Return_Ack(unsigned char Message_position,unsigned char Error_Message);		//��������ACKӦ����
void Return_Channel_Offset(void);								//��������ͨ��У׼��������
void SendDataTPA(unsigned  char *pr,unsigned char Length);

void DataPackage(int *pr);
void Return_SampleData(unsigned char Channel_ID,unsigned char Length);


#endif



