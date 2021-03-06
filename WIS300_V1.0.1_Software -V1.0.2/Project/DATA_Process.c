
#include "main.h"

unsigned char FirstReadUart_Flag=0x00;
unsigned char ReturnData_Count=0x00;
__IO unsigned char FrameDataReceived=0;		//定义帧数据接收标志位
__IO unsigned char DMASendDataCompleted=1;//定义DMA传输完成标志位

unsigned char SendCfgAtSampling=0;		//定义采样中查询标志位

/*******************************************************************************
* Function Name  : Filter_A\B\C\D
* Description    : 对ADS1248采集到的数据进行一阶滞后滤波算法，
									 系数A_Filter_K称为灵敏度，表示当前数据在总结果中所占的权重，
									 系数越大，灵敏度越高，但数据平滑度越低。
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void Channel_A_Filter(unsigned char A_Filter_K)//Channel A
//{
//	if(Ch_A_DATA.adc<Ch_A_DATA.Old_DATA)
//	{
//		Ch_A_DATA.adc=Ch_A_DATA.Old_DATA-(((Ch_A_DATA.Old_DATA-Ch_A_DATA.adc)*A_Filter_K)/256);
//		Ch_A_DATA.Old_DATA=Ch_A_DATA.adc;
//	}
//	else if(Ch_A_DATA.adc>Ch_A_DATA.Old_DATA)
//	{
//		Ch_A_DATA.adc=Ch_A_DATA.Old_DATA+(((Ch_A_DATA.adc-Ch_A_DATA.Old_DATA)*A_Filter_K)/256);
//		Ch_A_DATA.Old_DATA=Ch_A_DATA.adc;
//	}
//	else 
//	{
//		Ch_A_DATA.Old_DATA=Ch_A_DATA.adc;
//	}
//}

/*******************************************************************************
* Function Name  : ADS1248_Sample
* Description    : ADS1248底层采样驱动程序
* Input          : None
* Output         : None
* Return         : ADC采样值，有符号长整型
*******************************************************************************/
signed long ADS1248_ReadData(void)
{
	unsigned char result[3]={0,0,0};
	signed long adcVal=0;
	//ADS1248_WriteByte(ADS_RDATA);
	result[0]=ADS1248_WriteByte(ADS_NOP);
	result[1]=ADS1248_WriteByte(ADS_NOP);
	result[2]=ADS1248_WriteByte(ADS_NOP);
//	printf(" 0x%x\r\n",result[0]);
//	printf(" 0x%x\r\n",result[0]);
//	printf(" 0x%x\r\n",result[0]);
	
	adcVal=(signed long)( result[0] << 24 | result[1] << 16 | result[2] << 8 );//数据整合
	if(adcVal>0x7fffffff)//符号位转换
	{
		adcVal=(~adcVal)+0x01;
	}
	adcVal=adcVal>>8;
	return adcVal;
}

/*******************************************************************************
* Function Name  : ADS1248_B
* Description    : ADS1248采样程序
* Input          : None
* Output         : None
* Return         : ADC采样值，浮点型
*******************************************************************************/
void ADS1248_Sample(unsigned char Ch)
{
	unsigned char DATA_temp=0,Bridgr_Type=0,PGA_Gain=0,count=0;
	//unsigned char Filter_Val=0;
	//signed long Old_DATA=0;
	signed long adc=0;
	short int K_Val=0;
	float adcV=0,HalfSubZero=0,Zero_offset=0,strainVal=0; 
#ifdef WatchDog_EN
	IWDG_ReloadCounter();//喂狗
#endif
	ADS1248_CS(Ch);
	switch(Ch)
	{
		case Ch_A_ID:
			//Filter_Val=EEPROM_DATA.A_Filter_Val;
			Bridgr_Type=EEPROM_DATA.A_Bridge_Type;
			PGA_Gain=Ch_A_DATA.ADS_PGAGain;
			K_Val=EEPROM_DATA.A_K_Val;
			HalfSubZero=EEPROM_DATA.A_Half_offset-EEPROM_DATA.A_Zero_offset;
			Zero_offset=EEPROM_DATA.A_Zero_offset;
			count=Ch_A_DATA.Count;			
			break;
		case Ch_B_ID:
			//Filter_Val=EEPROM_DATA.B_Filter_Val;
			Bridgr_Type=EEPROM_DATA.B_Bridge_Type;
			PGA_Gain=Ch_B_DATA.ADS_PGAGain;
			K_Val=EEPROM_DATA.B_K_Val;
			HalfSubZero=EEPROM_DATA.B_Half_offset-EEPROM_DATA.B_Zero_offset;
			Zero_offset=EEPROM_DATA.B_Zero_offset;
			count=Ch_B_DATA.Count;
			break;
		case Ch_C_ID:
			//Filter_Val=EEPROM_DATA.C_Filter_Val;
			Bridgr_Type=EEPROM_DATA.C_Bridge_Type;
			PGA_Gain=Ch_C_DATA.ADS_PGAGain;
			K_Val=EEPROM_DATA.C_K_Val;
			HalfSubZero=EEPROM_DATA.C_Half_offset-EEPROM_DATA.C_Zero_offset;
			Zero_offset=EEPROM_DATA.C_Zero_offset;
			count=Ch_C_DATA.Count;
			break;
		case Ch_D_ID:
			//Filter_Val=EEPROM_DATA.D_Filter_Val;
			Bridgr_Type=EEPROM_DATA.D_Bridge_Type;
			PGA_Gain=Ch_D_DATA.ADS_PGAGain;
			K_Val=EEPROM_DATA.D_K_Val;
			HalfSubZero=EEPROM_DATA.D_Half_offset-EEPROM_DATA.D_Zero_offset;
			Zero_offset=EEPROM_DATA.D_Zero_offset;
			count=Ch_D_DATA.Count;
			break;
		default:
			break;
	}
	adc=ADS1248_ReadData();///读取ADS1248数据
	//此处加滤波
	adcV=((float)adc/0x7fffff)*2.048;//采样电压值变换
	//printf("adcV:%f\r\n",adcV);
	if(HalfSubZero<0)HalfSubZero=0-HalfSubZero;
	//应变=( adcV*(R1+R2)*(R1+R2) ) / ( R1*R2*K*Uexc )
	switch(Bridgr_Type)
	{
		case 0x84:
					strainVal=adcV*384400/(122616.276*2000);
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal*1000-Zero_offset;	  //此处X1000是应为K值放大了1000倍
					strainVal=(strainVal*5000000)/HalfSubZero;//半量程修正
					strainVal=strainVal*K_Val/2000;
			break;
		case 0x0c:
					strainVal=adcV*384400/(122616.276*2000);				//应变值转换
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal*1000-Zero_offset;	//此处X1000是应为K值放大了1000倍						//修正0点	
					strainVal=strainVal*5000000/HalfSubZero;//半量程修正
					strainVal=strainVal*K_Val/2000;
			break;
		case 0x04:
					strainVal=adcV*384400/(122616.276*2000);
					strainVal=strainVal/PGA_Gain;
					//strainVal=strainVal/2;
					strainVal=strainVal*1000000000000;//X1000000,转换成微应变，X1000，保留三位小数，X1000是由于K值放大1000倍了
					strainVal=strainVal*K_Val/2000;
					//自动增益控制：应添加到对应通道的末尾处
//					if( ((adcV>2)|(adcV<-2)) && (PGA_Gain == 128) )
//					{	
//							PGA_Gain=64;
//							DATA_temp=ADS1248_ReadREG(ADS_SYS0);
//							DATA_temp=DATA_temp&0x0f;				
//							ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
//					}
//					if(((adcV<2)|(adcV>-2)) && (PGA_Gain == 64))
//					{
//							PGA_Gain=128;
//							DATA_temp=ADS1248_ReadREG(ADS_SYS0);
//							DATA_temp=DATA_temp&0x0f;
//							ADS1248_WriteReg(ADS_SYS0,0x70|DATA_temp);							
//					}
					
			break;
		case 0x20:
					strainVal=adcV*57600/(29440*2000);		//应变值转换
					strainVal=strainVal/PGA_Gain;
					//strainVal=strainVal/4;
					strainVal=strainVal*1000000000000;
					strainVal=strainVal*K_Val/2000;
					//自动增益控制：应添加到对应通道的末尾处
//					if( ((adcV>2)|(adcV<-2)) && (PGA_Gain == 128) )
//					{	
//							PGA_Gain=64;
//							DATA_temp=ADS1248_ReadREG(ADS_SYS0);
//							DATA_temp=DATA_temp&0x0f;				
//							ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
//					}
//					if( ((adcV>2)|(adcV<-2)) && (PGA_Gain == 64) )
//					{	
//							PGA_Gain=32;
//							DATA_temp=ADS1248_ReadREG(ADS_SYS0);
//							DATA_temp=DATA_temp&0x0f;				
//							ADS1248_WriteReg(ADS_SYS0,0x50|DATA_temp);
//					}
//					if(((adcV<2)|(adcV>-2)) && (PGA_Gain == 64))
//					{
//							PGA_Gain=128;
//							DATA_temp=ADS1248_ReadREG(ADS_SYS0);
//							DATA_temp=DATA_temp&0x0f;
//							ADS1248_WriteReg(ADS_SYS0,0x70|DATA_temp);							
//					}
//					if(((adcV<2)|(adcV>-2)) && (PGA_Gain == 32))
//					{
//							PGA_Gain=64;
//							DATA_temp=ADS1248_ReadREG(ADS_SYS0);
//							DATA_temp=DATA_temp&0x0f;
//							ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);							
//					}					
			break;
		default:
			break;
	}
	switch(Ch)
	{
		case Ch_A_ID:Ch_A_DATA.Ch_DATAbuf[count-1]=strainVal;break;				 
		case Ch_B_ID:Ch_B_DATA.Ch_DATAbuf[count-1]=strainVal;break;				 
		case Ch_C_ID:Ch_C_DATA.Ch_DATAbuf[count-1]=strainVal;break;				
		case Ch_D_ID:Ch_D_DATA.Ch_DATAbuf[count-1]=strainVal;break;	 
		default:break;			
	}
}

/*******************************************************************************
* Function Name  : Channel_CFG_Check
* Description    : 在采样前检查通道是否配置
* Input          : None
* Output         : None
* Return         : 如果需要采样的通道没有配置，返回ERROR，已配置返回SUCCESS，
*******************************************************************************/
//static unsigned char Channel_CFG_Check(void)
//{
//	if((Sample_Control.Ch_Select)&Ch_A_ID)
//						if(!Ch_A_DATA.Ch_CFG_Flag)return ERROR;
//	if((Sample_Control.Ch_Select)&Ch_B_ID)
//						if(!Ch_B_DATA.Ch_CFG_Flag)return ERROR;
//	if((Sample_Control.Ch_Select)&Ch_C_ID)
//						if(!Ch_C_DATA.Ch_CFG_Flag)return ERROR;
//	if((Sample_Control.Ch_Select)&Ch_D_ID)
//						if(!Ch_D_DATA.Ch_CFG_Flag)return ERROR;
//	return SUCCESS;
//}
/*******************************************************************************
* Function Name  : Sample_Instruction_Control
* Description    : 采样处理子任务，
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Sample_Instruction_Control(void)
{
	unsigned char DATA_temp=SUCCESS;	
	
	memcpy(&Sample_Control,data_p+7,*(data_p+4));
	Sample_Control.Sample_Time=htons(Sample_Control.Sample_Time);

	if((Sample_Control.Ch_Select&0x0f)!=ERROR)												//检查通道
	{
		if(Sample_Control.Sample_Rate<0x04)														//检查采样频率
		{
			if( (Sample_Control.Sample_Type==0x00)|(Sample_Control.Sample_Type==0x01))//检查采样类型
			{												
				if(    (Sample_Control.Ch_DATASize>0x00) &&  (Sample_Control.Ch_DATASize<0x07)  )//检查数据包长度
				{;}
				else{Return_Ack(SAMPLE_INSTRUCTION,PARAMETERS_ILLEGAL);return;}
			}
			else{Return_Ack(SAMPLE_INSTRUCTION,PARAMETERS_ILLEGAL);return;}
		}
		else{Return_Ack(SAMPLE_INSTRUCTION,PARAMETERS_ILLEGAL);return;}				
	}
	else{Return_Ack(SAMPLE_INSTRUCTION,PARAMETERS_ILLEGAL);return;}
	Ch_A_DATA.ADS_PGAGain=0x80;
	Ch_B_DATA.ADS_PGAGain=0x80;
	Ch_C_DATA.ADS_PGAGain=0x80;
	Ch_D_DATA.ADS_PGAGain=0x80;

	if(Sample_Control.Ch_Select & Ch_A_ID)
	{
		if( (0x84 == EEPROM_DATA.A_Bridge_Type) || (0x0c == EEPROM_DATA.A_Bridge_Type) || \
			  (0x04 == EEPROM_DATA.A_Bridge_Type) || (0x20 == EEPROM_DATA.A_Bridge_Type)    )
				{ADS1248_CS(Ch_A_ID);ADS1248_Init(0x00,EEPROM_DATA.A_Bridge_Type);}
		else{Return_Ack(SAMPLE_INSTRUCTION,HADRWARE_NOTREADY);return;}
	}
	if(Sample_Control.Ch_Select & Ch_B_ID)
	{
		if( (0x84 == EEPROM_DATA.B_Bridge_Type) || (0x0c == EEPROM_DATA.B_Bridge_Type) || \
			  (0x04 == EEPROM_DATA.B_Bridge_Type) || (0x20 == EEPROM_DATA.B_Bridge_Type)    )
				{ADS1248_CS(Ch_B_ID);ADS1248_Init(0x00,EEPROM_DATA.B_Bridge_Type);}
		else{Return_Ack(SAMPLE_INSTRUCTION,PARAMETERS_ILLEGAL);return;}	
	}
	if(Sample_Control.Ch_Select & Ch_C_ID)
	{
		if( (0x84 == EEPROM_DATA.C_Bridge_Type) || (0x0c == EEPROM_DATA.C_Bridge_Type) || \
			  (0x04 == EEPROM_DATA.C_Bridge_Type) || (0x20 == EEPROM_DATA.C_Bridge_Type)    )
				{ADS1248_CS(Ch_C_ID);ADS1248_Init(0x00,EEPROM_DATA.C_Bridge_Type);}
		else{Return_Ack(SAMPLE_INSTRUCTION,PARAMETERS_ILLEGAL);return;}	
	}
	if(Sample_Control.Ch_Select & Ch_D_ID)
	{
		if( (0x84 == EEPROM_DATA.B_Bridge_Type) || (0x0c == EEPROM_DATA.B_Bridge_Type) || \
			  (0x04 == EEPROM_DATA.B_Bridge_Type) || (0x20 == EEPROM_DATA.B_Bridge_Type)    )
				{ADS1248_CS(Ch_D_ID);ADS1248_Init(0x00,EEPROM_DATA.D_Bridge_Type);}
		else{Return_Ack(SAMPLE_INSTRUCTION,PARAMETERS_ILLEGAL);return;}	
	}	
	ADS1248_CS(0x00);

//测量速度可选择5、10、 20、40、80、160、320、640、1000、2000SPS
//配置采样速度
	Return_Ack(SAMPLE_INSTRUCTION,SUCCESSFUL_EXECUTION);
	if(Sample_Control.Ch_Select & Ch_A_ID)GPIOA->BSRRH=GPIO_Pin_4;
	if(Sample_Control.Ch_Select & Ch_B_ID)GPIOC->BSRRH=GPIO_Pin_15;
	if(Sample_Control.Ch_Select & Ch_C_ID)GPIOA->BSRRH=GPIO_Pin_0;
	if(Sample_Control.Ch_Select & Ch_D_ID)GPIOA->BSRRH=GPIO_Pin_1;
	
	ADS1248_WriteReg(ADS_SYS0, (3-Sample_Control.Sample_Rate) | 0x70);//配置采样速率
	ADS1248_WriteByte(ADS_RDATAC);							          //发送开始转换指令
	ADS1248_CS(0x00);
	
	if(Sample_Control.Ch_Select & Ch_A_ID)A_Ch_START;
	if(Sample_Control.Ch_Select & Ch_B_ID)B_Ch_START;
	if(Sample_Control.Ch_Select & Ch_C_ID)C_Ch_START;
	if(Sample_Control.Ch_Select & Ch_D_ID)D_Ch_START;
	
//单次采样    20150815已完成
	if(Sample_Control.Sample_Type == 0x00)
	{		
			Sample_Control.Ch_DATASize=1;
			Delay(210);//等待数据转换完成
			if(Ch_A_DATA.Sample_Flag){A_Ch_STOP;Ch_A_DATA.Sample_Flag=0;Ch_A_DATA.Count++;ADS1248_Sample(Ch_A_ID);ADS1248_WriteByte(ADS_SDATAC);}//屏蔽通道A中断//清零采样标志位//采样次数计数器+1//读取A通道//发送停止采样命令
			if(Ch_B_DATA.Sample_Flag){B_Ch_STOP;Ch_B_DATA.Sample_Flag=0;Ch_B_DATA.Count++;ADS1248_Sample(Ch_B_ID);ADS1248_WriteByte(ADS_SDATAC);}						
			if(Ch_C_DATA.Sample_Flag){C_Ch_STOP;Ch_C_DATA.Sample_Flag=0;Ch_C_DATA.Count++;ADS1248_Sample(Ch_C_ID);ADS1248_WriteByte(ADS_SDATAC);}
			if(Ch_D_DATA.Sample_Flag){D_Ch_STOP;Ch_D_DATA.Sample_Flag=0;Ch_D_DATA.Count++;ADS1248_Sample(Ch_D_ID);ADS1248_WriteByte(ADS_SDATAC);}
			Return_SampleData(Sample_Control.Ch_DATASize);//返回结果值				
#ifdef	Debug_EN
			printf("Sampling has been completed...\r\n");
#endif
			GPIO_SetBits(GPIOB,GPIO_Pin_15);
	    return;
	}
//非单次采样
	if( Sample_Control.Sample_Type == 0x01 )
	{	
		if(Sample_Control.Sample_Time==0x00){SampleTime_Flag=0;SampleTime=1;}//一直采样//不使用SYStick标志位//时间设为1，一直采样
		if(Sample_Control.Sample_Time != 0x00){SampleTime_Flag=1;SampleTime=Sample_Control.Sample_Time;}//按时长采样//使用SYStick计时//设置采样时长
	
#ifdef	Debug_EN
		printf("Sampling,Please waiting...\r\n");
#endif
		while(SampleTime)
		{
/////////////////////////////////////////////////////////////////////////////////////////////////////////
			////以下代码：用于在采样过程中，TPA查询协处理器配置
				if(TpaCommand_Sampling_Flag)
				{
					if(FirstReadUart_Flag == 0x00)
					{
					 data_p = get_active_message(&data_len);//确保进来后只读取一次串口数据
					 FirstReadUart_Flag=0x01;
					}						
					if( (*(data_p+6)==0xa1) && (SendCfgAtSampling!=0) )//如果串口数据正常，且该标志位不为0，进入发送配置信息
					{
						switch(*(data_p+7))//此处只提供查询功能
						{
							case  REQUEST_CHANNEL_CFG:	//查询通道配置信息01
										switch(ReturnData_Count)
										{
											case 1: Return_Channel_CFG();break;
											case 2: Return_Ack(REQUEST_CHANNEL_CFG,SUCCESSFUL_EXECUTION);
													//清零标志位，放在ACK发送之后//先清理数据长度//清零标志位//清空串口缓存
													FirstReadUart_Flag=0x00;TpaCommandLen = 0;TpaCommand_Sampling_Flag = 0;USART1_ClearBuf();ReturnData_Count=0;break;										
											default:break;		
										}
										break;
							case  REQUEST_CFG_AND_CALIBRATION:		//查询通道配置参数及校准参数  05
										switch(ReturnData_Count)
										{
											case 1: Return_Ack(REQUEST_CHANNEL_CFG,SUCCESSFUL_EXECUTION);SendCfgAtSampling=0;break;
											case 2: Return_Channel_CFG();SendCfgAtSampling=0;break;								
											case 3: Return_ChannelA_Offset();SendCfgAtSampling=0;break;			
											case 4: Return_ChannelB_Offset();SendCfgAtSampling=0;break;						
											case 5: Return_ChannelC_Offset();SendCfgAtSampling=0;break;
											case 6: Return_ChannelD_Offset();SendCfgAtSampling=0;break;																										
											case 7: Return_Ack(REQUEST_CFG_AND_CALIBRATION,SUCCESSFUL_EXECUTION);
													//清零标志位，放在ACK发送之后//先清理数据长度//清零标志位//清空串口缓存
													FirstReadUart_Flag=0x00;TpaCommandLen = 0;TpaCommand_Sampling_Flag = 0;ReturnData_Count=0;SendCfgAtSampling=0;USART1_ClearBuf();break;														
											default: break;	
										}
										break;
							case  REQUEST_CALIBRATION_VAL:		//读取校准参数  21
										switch(ReturnData_Count)
										{
											case 1: Return_ChannelA_Offset();break;
											case 2:	Return_ChannelB_Offset();break;
											case 3: Return_ChannelC_Offset();break;
											case 4: Return_ChannelD_Offset();break;
											case 5:	Return_Ack(REQUEST_CALIBRATION_VAL,SUCCESSFUL_EXECUTION);
													//清零标志位，放在ACK发送之后//先清理数据长度//清零标志位//清空串口缓存
													FirstReadUart_Flag=0x00;TpaCommandLen = 0;TpaCommand_Sampling_Flag = 0;USART1_ClearBuf();ReturnData_Count=0;break;														
											default: break;		
										}
										break;
							default:		break;																
						}
					}
				}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////		
				//思想：以固定通道来确定同步信号，以最先产生中断信号的通道作为标志实现拉高拉低中断信号

				if( (Send_Flag_TPA != 0) && (FrameDataReceived == Sample_Control.Ch_Select) )//已经向TPA发送过中断，然后想TPA发送数据
				{
					GPIO_SetBits(GPIOB,GPIO_Pin_3);Delay(1);//向TPA发送中断//延时1ms，避免TPA处理不过来
					Return_SampleData(Sample_Control.Ch_DATASize);//向TPA发送数据
						
					Send_Flag_TPA=0;FrameDataReceived=0; //清零标志位
					if(TpaCommand_Sampling_Flag)		//在数据包直接，分次将查询信息发送给TPA
						{
							while(DMASendDataCompleted);//等待DMA数据传输完成
							DMASendDataCompleted=1;
							Delay(2);					//延时2ms确保TPA能够处理完
							ReturnData_Count++;
							SendCfgAtSampling=1;		//置位标志位
						}
				}
				
				if(Ch_A_DATA.Sample_Flag){Ch_A_DATA.Sample_Flag=0;Ch_A_DATA.Count++;ADS1248_Sample(Ch_A_ID);}//如果是A通道来的中断//清零A通道采样完成标志位//数据位+1//读取并处理A通道数据												
				if(Ch_B_DATA.Sample_Flag){Ch_B_DATA.Sample_Flag=0;Ch_B_DATA.Count++;ADS1248_Sample(Ch_B_ID);}						
				if(Ch_C_DATA.Sample_Flag){Ch_C_DATA.Sample_Flag=0;Ch_C_DATA.Count++;ADS1248_Sample(Ch_C_ID);}					
				if(Ch_D_DATA.Sample_Flag){Ch_D_DATA.Sample_Flag=0;Ch_D_DATA.Count++;ADS1248_Sample(Ch_D_ID);}
					
				if(Sample_Control.Ch_DATASize==Ch_A_DATA.Count){Ch_A_DATA.Count=0;FrameDataReceived |= 0x01;}//如果已经采够一包数据//清零采样计数器	
				if(Sample_Control.Ch_DATASize==Ch_B_DATA.Count){Ch_B_DATA.Count=0;FrameDataReceived |= 0x02;}
				if(Sample_Control.Ch_DATASize==Ch_C_DATA.Count){Ch_C_DATA.Count=0;FrameDataReceived |= 0x04;}
				if(Sample_Control.Ch_DATASize==Ch_D_DATA.Count){Ch_D_DATA.Count=0;FrameDataReceived |= 0x08;}							
		}	
		SampleTime_Flag=0;
		if(Sample_Control.Ch_Select & Ch_A_ID){A_Ch_STOP;Ch_A_DATA.Sample_Flag=0;ADS1248_CS(Ch_A_ID);ADS1248_WriteByte(ADS_SDATAC);Ch_A_DATA.Count=0;}//屏蔽中断//清零标志位//拉低片选//发送停止采样命令//清零采样计数器
		if(Sample_Control.Ch_Select & Ch_B_ID){B_Ch_STOP;Ch_B_DATA.Sample_Flag=0;ADS1248_CS(Ch_B_ID);ADS1248_WriteByte(ADS_SDATAC);Ch_B_DATA.Count=0;}
		if(Sample_Control.Ch_Select & Ch_C_ID){C_Ch_STOP;Ch_C_DATA.Sample_Flag=0;ADS1248_CS(Ch_C_ID);ADS1248_WriteByte(ADS_SDATAC);Ch_C_DATA.Count=0;}	
		if(Sample_Control.Ch_Select & Ch_D_ID){D_Ch_STOP;Ch_D_DATA.Sample_Flag=0;ADS1248_CS(Ch_D_ID);ADS1248_WriteByte(ADS_SDATAC);Ch_D_DATA.Count=0;}

#ifdef	Debug_EN
		printf("Sampling has been completed...\r\n");
#endif
		Send_Flag_TPA=0;
		Clear_Flag_TPA=0;
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
		return;
	}	
}


 

