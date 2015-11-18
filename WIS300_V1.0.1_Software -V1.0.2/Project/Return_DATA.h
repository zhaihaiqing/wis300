
#ifndef __Return_DATA_H
#define __Return_DATA_H


void Return_Channel_CFG(void);									//声明返回通道配置函数
void Return_Ack(unsigned char Message_position,unsigned char Error_Message);		//声明返回ACK应答函数
void Return_Channel_Offset(void);								//声明返回通道校准参数函数
void SendDataTPA(unsigned  char *pr,unsigned char Length);

void DataPackage(int *pr);
void Return_SampleData(unsigned char Channel_ID,unsigned char Length);


#endif



