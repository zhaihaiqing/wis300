
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
	Return_ChannelA_Offset();
	while(DMASendDataCompleted);
	DMASendDataCompleted=1;
	Return_ChannelB_Offset();
	while(DMASendDataCompleted);
	DMASendDataCompleted=1;
	Return_ChannelC_Offset();
	while(DMASendDataCompleted);
	DMASendDataCompleted=1;
	Return_ChannelD_Offset();	
	while(DMASendDataCompleted);
	DMASendDataCompleted=1;
}
//返回A通道校准结果
void Return_ChannelA_Offset(void)
{
	Calibration_return_tagdef Calibration_return;
	int Calibration_zero=0,Calibration_half=0;	
	//读取校准参数    21

	Calibration_return.Message_type=21;
	Calibration_zero=(int)(EEPROM_DATA.A_Zero_offset*1000000000);
	Calibration_half=(int)(EEPROM_DATA.A_Half_offset*1000000000);
	Calibration_return.Calibration_zero[0]=(Calibration_zero&0xff000000)>>24;
	Calibration_return.Calibration_zero[1]=(Calibration_zero&0x00ff0000)>>16;
	Calibration_return.Calibration_zero[2]=(Calibration_zero&0x0000ff00)>>8;
	Calibration_return.Calibration_zero[3]=Calibration_zero&0x000000ff;
	
	Calibration_return.Calibration_half[0]=(Calibration_half&0xff000000)>>24;
	Calibration_return.Calibration_half[1]=(Calibration_half&0x00ff0000)>>16;
	Calibration_return.Calibration_half[2]=(Calibration_half&0x0000ff00)>>8;
	Calibration_return.Calibration_half[3]=Calibration_half&0x000000ff;
	Calibration_return.Channel=0x01;
	SendDataTPA(&Calibration_return.Message_type,sizeof(Calibration_return));
}
//返回B通道校准结果
void Return_ChannelB_Offset(void)
{
	Calibration_return_tagdef Calibration_return;
	int Calibration_zero=0,Calibration_half=0;	
	//读取校准参数    21
	
	Calibration_return.Message_type=21;
	Calibration_zero=(int)(EEPROM_DATA.B_Zero_offset*1000000000);
	Calibration_half=(int)(EEPROM_DATA.B_Half_offset*1000000000);
	Calibration_return.Calibration_zero[0]=(Calibration_zero&0xff000000)>>24;
	Calibration_return.Calibration_zero[1]=(Calibration_zero&0x00ff0000)>>16;
	Calibration_return.Calibration_zero[2]=(Calibration_zero&0x0000ff00)>>8;
	Calibration_return.Calibration_zero[3]=Calibration_zero&0x000000ff;
	
	Calibration_return.Calibration_half[0]=(Calibration_half&0xff000000)>>24;
	Calibration_return.Calibration_half[1]=(Calibration_half&0x00ff0000)>>16;
	Calibration_return.Calibration_half[2]=(Calibration_half&0x0000ff00)>>8;
	Calibration_return.Calibration_half[3]=Calibration_half&0x000000ff;
	Calibration_return.Channel=0x02;
	SendDataTPA(&Calibration_return.Message_type,sizeof(Calibration_return));	
}
//返回C通道校准结果
void Return_ChannelC_Offset(void)
{
	Calibration_return_tagdef Calibration_return;
	int Calibration_zero=0,Calibration_half=0;	
	//读取校准参数    21			
	
	Calibration_return.Message_type=21;
	Calibration_zero=(int)(EEPROM_DATA.C_Zero_offset*1000000000);
	Calibration_half=(int)(EEPROM_DATA.C_Half_offset*1000000000);
	Calibration_return.Calibration_zero[0]=(Calibration_zero&0xff000000)>>24;
	Calibration_return.Calibration_zero[1]=(Calibration_zero&0x00ff0000)>>16;
	Calibration_return.Calibration_zero[2]=(Calibration_zero&0x0000ff00)>>8;
	Calibration_return.Calibration_zero[3]=Calibration_zero&0x000000ff;
	
	Calibration_return.Calibration_half[0]=(Calibration_half&0xff000000)>>24;
	Calibration_return.Calibration_half[1]=(Calibration_half&0x00ff0000)>>16;
	Calibration_return.Calibration_half[2]=(Calibration_half&0x0000ff00)>>8;
	Calibration_return.Calibration_half[3]=Calibration_half&0x000000ff;
	Calibration_return.Channel=0x04;
	SendDataTPA(&Calibration_return.Message_type,sizeof(Calibration_return));
}
//返回D通道校准结果
void Return_ChannelD_Offset(void)
{
	Calibration_return_tagdef Calibration_return;
	int Calibration_zero=0,Calibration_half=0;
	//读取校准参数    21	
	
	Calibration_return.Message_type=21;
	Calibration_zero=(int)(EEPROM_DATA.D_Zero_offset*1000000000);
	Calibration_half=(int)(EEPROM_DATA.D_Half_offset*1000000000);
	Calibration_return.Calibration_zero[0]=(Calibration_zero&0xff000000)>>24;
	Calibration_return.Calibration_zero[1]=(Calibration_zero&0x00ff0000)>>16;
	Calibration_return.Calibration_zero[2]=(Calibration_zero&0x0000ff00)>>8;
	Calibration_return.Calibration_zero[3]=Calibration_zero&0x000000ff;
	
	Calibration_return.Calibration_half[0]=(Calibration_half&0xff000000)>>24;
	Calibration_return.Calibration_half[1]=(Calibration_half&0x00ff0000)>>16;
	Calibration_return.Calibration_half[2]=(Calibration_half&0x0000ff00)>>8;
	Calibration_return.Calibration_half[3]=Calibration_half&0x000000ff;
	Calibration_return.Channel=0x08;
	SendDataTPA(&Calibration_return.Message_type,sizeof(Calibration_return));
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
	Channel_CFG_tagdef Channel_CFG;
	Channel_CFG.A_bridge_type=0xff;//初始换为0xff，未配置电桥类型时，返回0xff
	Channel_CFG.B_bridge_type=0xff;
	Channel_CFG.C_bridge_type=0xff;
	Channel_CFG.D_bridge_type=0xff;
#ifdef	       	Debug_EN
								printf("Return Channel CFG!\r\n");
#endif
	Channel_CFG.Message_type=1;
	if(EEPROM_DATA.A_Bridge_Type==TWO_WIRE_SINGLE_BRIDGE)   Channel_CFG.A_bridge_type=0x00;
	if(EEPROM_DATA.A_Bridge_Type==THREE_WIRE_SINGLE_BRIDGE) Channel_CFG.A_bridge_type=0x01;
	if(EEPROM_DATA.A_Bridge_Type==HALF_BRIDGE) Channel_CFG.A_bridge_type=0x02;
	if(EEPROM_DATA.A_Bridge_Type==FULL_BRIDGE) Channel_CFG.A_bridge_type=0x04;
	
	if(EEPROM_DATA.B_Bridge_Type==TWO_WIRE_SINGLE_BRIDGE)   Channel_CFG.B_bridge_type=0x00;
	if(EEPROM_DATA.B_Bridge_Type==THREE_WIRE_SINGLE_BRIDGE) Channel_CFG.B_bridge_type=0x01;
	if(EEPROM_DATA.B_Bridge_Type==HALF_BRIDGE) Channel_CFG.B_bridge_type=0x02;
	if(EEPROM_DATA.B_Bridge_Type==FULL_BRIDGE) Channel_CFG.B_bridge_type=0x04;
	
	if(EEPROM_DATA.C_Bridge_Type==TWO_WIRE_SINGLE_BRIDGE)   Channel_CFG.C_bridge_type=0x00;
	if(EEPROM_DATA.C_Bridge_Type==THREE_WIRE_SINGLE_BRIDGE) Channel_CFG.C_bridge_type=0x01;
	if(EEPROM_DATA.C_Bridge_Type==HALF_BRIDGE) Channel_CFG.C_bridge_type=0x02;
	if(EEPROM_DATA.C_Bridge_Type==FULL_BRIDGE) Channel_CFG.C_bridge_type=0x04;
	
	if(EEPROM_DATA.D_Bridge_Type==TWO_WIRE_SINGLE_BRIDGE)   Channel_CFG.D_bridge_type=0x00;
	if(EEPROM_DATA.D_Bridge_Type==THREE_WIRE_SINGLE_BRIDGE) Channel_CFG.D_bridge_type=0x01;
	if(EEPROM_DATA.D_Bridge_Type==HALF_BRIDGE) Channel_CFG.D_bridge_type=0x02;
	if(EEPROM_DATA.D_Bridge_Type==FULL_BRIDGE) Channel_CFG.D_bridge_type=0x04;
		
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
	
	SendDataTPA(&Channel_CFG.Message_type,sizeof(Channel_CFG));
	while(DMASendDataCompleted);
	DMASendDataCompleted=1;
}

/*******************************************************************************
* Function Name  : Return_Ack
* Description    : ACK返回指令，根据任务执行状态返回相应的ACK
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Return_Ack(unsigned char Message_position,unsigned char Error_Message)
{
	Ack_tagdef Ack;
	Ack.Message_type=0xff;
	Ack.Message=Message_position;
	Ack.Error_type=Error_Message;
	Ack.Reserve1=0x00;
	Ack.Reserve2=0x00;
	SendDataTPA(&Ack.Message_type,sizeof(Ack));
	while(DMASendDataCompleted);
	DMASendDataCompleted=1;
}

void SendDataTPA(unsigned  char *pr,unsigned char Length)
{
	unsigned char i;
	if((!Length) &&(Length > 28))//数据长度不能大于28,否则总长度将大于40,包格式其他数据占用12字节
    {
    //printf("%s(%d)data length error\r\n",__FILE__,__LINE__);
		return;
    }
	send_message_prepare(0xa1);
	for(i = 0;i < Length;i++)
   {
    send_message_add(*pr);//单字节
    pr++;
   }
	send_message(1);
}

void DataPackage( int *pr)
{
		send_message_add( ((unsigned char)((*pr)>>24)) );//单字节组包
		send_message_add( ((unsigned char)((*pr)>>16)) );//单字节组包
		send_message_add( ((unsigned char)((*pr)>>8 )) );//单字节组包
		send_message_add( ( unsigned char)( *pr      ) );//单字节组包
}

void Return_SampleData(unsigned char Length)
{
							unsigned int i;
							send_message_prepare(0xa1);//在向TPA发送消息时，先发送此条信息
							send_message_add(41);//单字节组包
							send_message_add(Length);//单字节组包
							
																				
													if(Sample_Control.Ch_Select & Ch_A_ID)
													{
//#ifdef  								 						Debug_EN														
//																		printf("%d [A]:%.3f\r\n",i+1, ((float)Ch_A_DATA.Ch_DATAbuf[i]/1000));
																		
//#endif																
																		send_message_add(Ch_A_ID);//单字节组包	
																		for(i=0;i<Length;i++)
																		{
																			DataPackage(&Ch_A_DATA.Ch_DATAbuf[i]);
																			Ch_A_DATA.Ch_DATAbuf[i]=0;
																		}
													}
													if(Sample_Control.Ch_Select & Ch_B_ID)
													{
//#ifdef  								 						Debug_EN														
//																		printf("%d [B]:%.3f\r\n",i+1, ((float)Ch_B_DATA.Ch_DATAbuf[i]/1000));
//#endif
																		send_message_add(Ch_B_ID);//单字节组包
																		for(i=0;i<Length;i++)
																		{
																			DataPackage(&Ch_B_DATA.Ch_DATAbuf[i]);
																			Ch_B_DATA.Ch_DATAbuf[i]=0;
																		}
													}
													
													if(Sample_Control.Ch_Select & Ch_C_ID)
													{
//#ifdef  								 						Debug_EN														
//																		printf("%d [C]:%.3f\r\n",i+1, ((float)Ch_C_DATA.Ch_DATAbuf[i]/1000));
//#endif
																		send_message_add(Ch_C_ID);//单字节组包
																		for(i=0;i<Length;i++)
																		{
																			DataPackage(&Ch_C_DATA.Ch_DATAbuf[i]);
																			Ch_C_DATA.Ch_DATAbuf[i]=0;
																		}
													}
													if(Sample_Control.Ch_Select & Ch_D_ID)
													{
//#ifdef  								 						Debug_EN														
//																		printf("%d [D]:%.3f\r\n",i+1, ((float)Ch_D_DATA.Ch_DATAbuf[i]/1000));
//#endif
																		send_message_add(Ch_D_ID);//单字节组包
																		for(i=0;i<Length;i++)
																		{
																			DataPackage(&Ch_D_DATA.Ch_DATAbuf[i]);
																			Ch_D_DATA.Ch_DATAbuf[i]=0;
																		}
													}			
							
							send_message_add(0x00);//单字节组包
							send_message_add(0x00);//单字节组包
							send_message(1);//发送包，完成
}




































