
#include "main.h"

/*******************************************************************************
* Function Name  : Return_Channel_Offset
* Description    : 通道校准参数返回，接收到读取校准参数指令后进入该程序，通过串口将数据传出
* Input          : None
* Output         : None
* Return         : 直接将校准结果存储
*******************************************************************************/
void Return_Channel_Offset(void)
{
	Calibration_return_tagdef Calibration_return;
	unsigned char Channel_OE;
	unsigned char *pr=NULL;//定义指针，指向空
	unsigned char i;
	int Calibration_zero,Calibration_half;
	Channel_OE=*(data_p+8);
	Calibration_return.Message_type=0x21;
	if(Channel_OE & 0x01)
		{
					Calibration_zero=(int)(EEPROM_DATA.A_Zero_offset*1000000000);
					Calibration_half=(int)(EEPROM_DATA.A_Half_offset*1000000000);
		}
	if(Channel_OE & 0x02)
		{
					Calibration_zero=(int)(EEPROM_DATA.B_Zero_offset*1000000000);
					Calibration_half=(int)(EEPROM_DATA.B_Half_offset*1000000000);
		}
	if(Channel_OE & 0x04)
		{
					Calibration_zero=(int)(EEPROM_DATA.C_Zero_offset*1000000000);
					Calibration_half=(int)(EEPROM_DATA.C_Half_offset*1000000000);
		}
	if(Channel_OE & 0x08)
		{
					Calibration_zero=(int)(EEPROM_DATA.D_Zero_offset*1000000000);
					Calibration_half=(int)(EEPROM_DATA.D_Half_offset*1000000000);
		}

	Calibration_return.Calibration_zero[0]=(Calibration_zero&0xff000000)>>24;
	Calibration_return.Calibration_zero[1]=(Calibration_zero&0x00ff0000)>>16;
	Calibration_return.Calibration_zero[2]=(Calibration_zero&0x0000ff00)>>8;
	Calibration_return.Calibration_zero[3]=Calibration_zero&0x000000ff;
	
	Calibration_return.Calibration_half[0]=(Calibration_half&0xff000000)>>24;
	Calibration_return.Calibration_half[1]=(Calibration_half&0x00ff0000)>>16;
	Calibration_return.Calibration_half[2]=(Calibration_half&0x0000ff00)>>8;
	Calibration_return.Calibration_half[3]=Calibration_half&0x000000ff;

	Calibration_return.Channel=Channel_OE;
	pr=&Calibration_return.Message_type;//指针赋初值
	send_message_prepare(0xa1);//在向TPA发送消息时，先发送此条信息
	for(i=0;i<sizeof(Calibration_return);i++)//数组组包
	{
		send_message_add(*pr);
		pr++;
	}
	send_message(1);//发送包，完成
	Return_Ack(SUCCESSFUL_EXECUTION);
}


/*******************************************************************************
* Function Name  : Return_Channel_CFG
* Description    : 通道配置参数返回，接收到读取通道配置参数指令后进入该程序，通过串口将数据传出
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Return_Channel_CFG(void)
{
	unsigned char *pr=NULL;//定义指针，指向空
	unsigned char i;
	Channel_CFG_tagdef Channel_CFG;
	Channel_CFG.A_bridge_type=0xff;//初始换为0xff，未配置电桥类型时，返回0xff
	Channel_CFG.B_bridge_type=0xff;
	Channel_CFG.C_bridge_type=0xff;
	Channel_CFG.D_bridge_type=0xff;
	
	Channel_CFG.Message_type=0x01;
	SPI1_CS1_LOW();
	if(ADS1248_ReadREG(ADS_GPIODAT)==TWO_WIRE_SINGLE_BRIDGE)
		Channel_CFG.A_bridge_type=0x00;
	if(ADS1248_ReadREG(ADS_GPIODAT)==THREE_WIRE_SINGLE_BRIDGE)
		Channel_CFG.A_bridge_type=0x01;
	if(ADS1248_ReadREG(ADS_GPIODAT)==HALF_BRIDGE)
		Channel_CFG.A_bridge_type=0x02;
	if(ADS1248_ReadREG(ADS_GPIODAT)==FULL_BRIDGE)
		Channel_CFG.A_bridge_type=0x04;
	SPI1_CS2_LOW();
	if(ADS1248_ReadREG(ADS_GPIODAT)==TWO_WIRE_SINGLE_BRIDGE)
		Channel_CFG.B_bridge_type=0x00;
	if(ADS1248_ReadREG(ADS_GPIODAT)==THREE_WIRE_SINGLE_BRIDGE)
		Channel_CFG.B_bridge_type=0x01;
	if(ADS1248_ReadREG(ADS_GPIODAT)==HALF_BRIDGE)
		Channel_CFG.B_bridge_type=0x02;
	if(ADS1248_ReadREG(ADS_GPIODAT)==FULL_BRIDGE)
		Channel_CFG.B_bridge_type=0x04;
	SPI1_CS3_LOW();
	if(ADS1248_ReadREG(ADS_GPIODAT)==TWO_WIRE_SINGLE_BRIDGE)
		Channel_CFG.C_bridge_type=0x00;
	if(ADS1248_ReadREG(ADS_GPIODAT)==THREE_WIRE_SINGLE_BRIDGE)
		Channel_CFG.C_bridge_type=0x01;
	if(ADS1248_ReadREG(ADS_GPIODAT)==HALF_BRIDGE)
		Channel_CFG.C_bridge_type=0x02;
	if(ADS1248_ReadREG(ADS_GPIODAT)==FULL_BRIDGE)
		Channel_CFG.C_bridge_type=0x04;
	SPI1_CS4_LOW();
	if(ADS1248_ReadREG(ADS_GPIODAT)==TWO_WIRE_SINGLE_BRIDGE)
		Channel_CFG.D_bridge_type=0x00;
	if(ADS1248_ReadREG(ADS_GPIODAT)==THREE_WIRE_SINGLE_BRIDGE)
		Channel_CFG.D_bridge_type=0x01;
	if(ADS1248_ReadREG(ADS_GPIODAT)==HALF_BRIDGE)
		Channel_CFG.D_bridge_type=0x02;
	if(ADS1248_ReadREG(ADS_GPIODAT)==FULL_BRIDGE)
		Channel_CFG.D_bridge_type=0x04;
	Channel_CFG.A_filter_val=EEPROM_DATA.A_Filter_Val;
	Channel_CFG.B_filter_val=EEPROM_DATA.B_Filter_Val;
	Channel_CFG.C_filter_val=EEPROM_DATA.C_Filter_Val;
	Channel_CFG.D_filter_val=EEPROM_DATA.D_Filter_Val;
	Channel_CFG.A_k_val_h=EEPROM_DATA.A_K_Val>>8;
	Channel_CFG.A_k_val_l=(unsigned char)EEPROM_DATA.A_K_Val;
	
	Channel_CFG.B_k_val_h=EEPROM_DATA.B_K_Val>>8;
	Channel_CFG.B_k_val_l=(unsigned char)EEPROM_DATA.B_K_Val;
	
	Channel_CFG.C_k_val_h=EEPROM_DATA.C_K_Val>>8;
	Channel_CFG.C_k_val_l=(unsigned char)EEPROM_DATA.C_K_Val;
	
	Channel_CFG.D_k_val_h=EEPROM_DATA.D_K_Val>>8;
	Channel_CFG.D_k_val_l=(unsigned char)EEPROM_DATA.D_K_Val;
	Channel_CFG.Reserve1=0;
	Channel_CFG.Reserve2=0;
	
	pr=&Channel_CFG.Message_type;//指针赋初值
	
	send_message_prepare(0xa1);//在向TPA发送消息时，先发送此条信息
	for(i=0;i<sizeof(Channel_CFG);i++)//数组组包
	{
		send_message_add(*pr);
		pr++;
	}
	send_message(1);//发送包，完成
	Return_Ack(SUCCESSFUL_EXECUTION);
}

/*******************************************************************************
* Function Name  : Return_Ack
* Description    : ACK返回指令，根据任务执行状态返回相应的ACK
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Return_Ack(unsigned char Error_Message)
{
	Ack_tagdef Ack;
	unsigned char *pr=NULL;//定义指针，指向空
	unsigned char i;
	Ack.Message_type=0xff;
	Ack.Error_type=Error_Message;
	Ack.Reserve1=0x00;
	Ack.Reserve2=0x00;
	
	pr=&Ack.Message_type;//指针赋初值
	send_message_prepare(0xa1);//在向TPA发送消息时，先发送此条信息
	for(i=0;i<sizeof(Ack);i++)//数组组包
	{
		send_message_add(*pr);
		pr++;
	}
	send_message(1);//发送包，完成
}

void DataPackage(int *pr)
{
		send_message_add( ((unsigned char)((*pr)>>24)) );//单字节组包
		send_message_add( ((unsigned char)((*pr)>>16)) );//单字节组包
		send_message_add( ((unsigned char)((*pr)>>8 )) );//单字节组包
		send_message_add( ( unsigned char)( *pr      ) );//单字节组包
}

void Return_SampleData(unsigned char Channel_ID,unsigned char Length)
{
							unsigned int i;
							send_message_prepare(0xa1);//在向TPA发送消息时，先发送此条信息
							send_message_add(0x41);//单字节组包
							send_message_add(Channel_ID);//单字节组包	
							for(i=0;i<Length;i++)
							{													
													if(Channel_ID==Ch_A_ID)
													{
#ifdef  								 						Debug_EN														
																		printf("%d [A]:%.3f\r\n",i+1, ((float)A_Channel_DATAbuf[i]/1000));
#endif
																		DataPackage(&A_Channel_DATAbuf[i]);
																		A_Channel_DATAbuf[i]=0;
													}
													else if(Channel_ID==Ch_B_ID)
													{
#ifdef  								 						Debug_EN														
																		printf("%d [B]:%.3f\r\n",i+1, ((float)B_Channel_DATAbuf[i]/1000));
#endif
																		DataPackage(&B_Channel_DATAbuf[i]);
																		B_Channel_DATAbuf[i]=0;
													}
													
													else if(Channel_ID==Ch_C_ID)
													{
#ifdef  								 						Debug_EN														
																		printf("%d [C]:%.3f\r\n",i+1, ((float)C_Channel_DATAbuf[i]/1000));
#endif
																		DataPackage(&C_Channel_DATAbuf[i]);
																		C_Channel_DATAbuf[i]=0;
													}
													
													else
													{
#ifdef  								 						Debug_EN														
																		printf("%d [D]:%.3f\r\n",i+1, ((float)D_Channel_DATAbuf[i]/1000));
#endif
																		DataPackage(&D_Channel_DATAbuf[i]);
																		D_Channel_DATAbuf[i]=0;
													}			
							}
							send_message_add(0x00);//单字节组包
							send_message_add(0x00);//单字节组包
							send_message(1);//发送包，完成
}



//将接收到的命令通过调试串口打印出来






























