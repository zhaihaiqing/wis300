
#include "main.h"


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
			Bridgr_Type=Ch_A_DATA.Bridge_Type;
			PGA_Gain=Ch_A_DATA.ADS_PGAGain;
			K_Val=EEPROM_DATA.A_K_Val;
			HalfSubZero=EEPROM_DATA.A_Half_offset-EEPROM_DATA.A_Zero_offset;
			Zero_offset=EEPROM_DATA.A_Zero_offset;
			count=Ch_A_DATA.Count;			
			break;
		case Ch_B_ID:
			//Filter_Val=EEPROM_DATA.B_Filter_Val;
			Bridgr_Type=Ch_B_DATA.Bridge_Type;
			PGA_Gain=Ch_B_DATA.ADS_PGAGain;
			K_Val=EEPROM_DATA.B_K_Val;
			HalfSubZero=EEPROM_DATA.B_Half_offset-EEPROM_DATA.B_Zero_offset;
			Zero_offset=EEPROM_DATA.B_Zero_offset;
			count=Ch_B_DATA.Count;
			break;
		case Ch_C_ID:
			//Filter_Val=EEPROM_DATA.C_Filter_Val;
			Bridgr_Type=Ch_C_DATA.Bridge_Type;
			PGA_Gain=Ch_C_DATA.ADS_PGAGain;
			K_Val=EEPROM_DATA.C_K_Val;
			HalfSubZero=EEPROM_DATA.C_Half_offset-EEPROM_DATA.C_Zero_offset;
			Zero_offset=EEPROM_DATA.C_Zero_offset;
			count=Ch_C_DATA.Count;
			break;
		case Ch_D_ID:
			//Filter_Val=EEPROM_DATA.D_Filter_Val;
			Bridgr_Type=Ch_D_DATA.Bridge_Type;
			PGA_Gain=Ch_D_DATA.ADS_PGAGain;
			K_Val=EEPROM_DATA.D_K_Val;
			HalfSubZero=EEPROM_DATA.D_Half_offset-EEPROM_DATA.D_Zero_offset;
			Zero_offset=EEPROM_DATA.D_Zero_offset;
			count=Ch_D_DATA.Count;
			break;
		default:
			break;
	}
	adc=ADS1248_ReadData();
	//此处加滤波
	adcV=((float)adc/0x7fffff)*2.0477;//采样电压值变换
	if(HalfSubZero<0)HalfSubZero=0-HalfSubZero;
	switch(Bridgr_Type)
	{
		case 0x00:
					strainVal=adcV*384400/(122616.276*K_Val);
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal*1000-Zero_offset;	//此处X1000是应为K值放大了1000倍
					strainVal=(strainVal*5000000)/HalfSubZero;//半量程修正			
			break;
		case 0x01:
					strainVal=adcV*384400/(122616.276*K_Val);																 //应变值转换
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal*1000-Zero_offset;	//此处X1000是应为K值放大了1000倍																					 //修正0点	
					strainVal=strainVal*5000000/HalfSubZero;//半量程修正		
			break;
		case 0x02:
					strainVal=adcV*384400/(122616.276*K_Val);
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal/2;
					strainVal=strainVal*1000000000000;
			 //自动增益控制：应添加到对应通道的末尾处
					if((adcV>2)|(adcV<-2))
					{	
							PGA_Gain=PGA_Gain>>1;
							DATA_temp=ADS1248_ReadREG(ADS_SYS0);
							DATA_temp=DATA_temp&0x0f;
							if(PGA_Gain==64)
							{					
								ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
							}
					}
			break;
		case 0x04:
					strainVal=adcV*57600/(29486.88*K_Val);				//应变值转换
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal/4;
					strainVal=strainVal*1000000000000;
					//自动增益控制：应添加到对应通道的末尾处
					if((adcV>2)|(adcV<-2))
					{
							PGA_Gain=PGA_Gain>>1;
							DATA_temp=ADS1248_ReadREG(ADS_SYS0);//注意此处是否会出现抖动数据
							DATA_temp=DATA_temp&0x0f;
							if(PGA_Gain==64)
							{					
								ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
							}
							if(PGA_Gain==32)
							{
								ADS1248_WriteReg(ADS_SYS0,0x50|DATA_temp);
							}
					}
			break;
		default:
			break;
	}
	switch(Ch)
	{
		case Ch_A_ID:
				 Ch_A_DATA.Ch_DATAbuf[count-1]=strainVal;break;
		case Ch_B_ID:
				 Ch_B_DATA.Ch_DATAbuf[count-1]=strainVal;break;
		case Ch_C_ID:
				 Ch_C_DATA.Ch_DATAbuf[count-1]=strainVal;break;
		case Ch_D_ID:
				 Ch_D_DATA.Ch_DATAbuf[count-1]=strainVal;break;
		default:
			break;
	}
}
/*******************************************************************************
* Function Name  : Channel_CFG_Check
* Description    : 在采样前检查通道是否配置
* Input          : None
* Output         : None
* Return         : 如果需要采样的通道没有配置，返回ERROR，已配置返回SUCCESS，
*******************************************************************************/
static unsigned char Channel_CFG_Check(void)
{
	if((Sample_Control.Ch_Select)&Ch_A_ID)
						if(!Ch_A_DATA.Ch_CFG_Flag)return ERROR;
	if((Sample_Control.Ch_Select)&Ch_B_ID)
						if(!Ch_B_DATA.Ch_CFG_Flag)return ERROR;
	if((Sample_Control.Ch_Select)&Ch_C_ID)
						if(!Ch_C_DATA.Ch_CFG_Flag)return ERROR;
	if((Sample_Control.Ch_Select)&Ch_D_ID)
						if(!Ch_D_DATA.Ch_CFG_Flag)return ERROR;
	return SUCCESS;
}
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
				else{Return_Ack(PARAMETERS_ILLEGAL);return;}
			}
			else{Return_Ack(PARAMETERS_ILLEGAL);return;}
		}
		else{Return_Ack(PARAMETERS_ILLEGAL);return;}				
	}
	else{Return_Ack(PARAMETERS_ILLEGAL);return;}		

	Ch_A_DATA.ADS_PGAGain=0x80;
	Ch_B_DATA.ADS_PGAGain=0x80;
	Ch_C_DATA.ADS_PGAGain=0x80;
	Ch_D_DATA.ADS_PGAGain=0x80;
//收到采样指令后，首先检查对应通道是否完成配置，否则返回硬件没有准备好信息
	DATA_temp=Channel_CFG_Check();
	if(!DATA_temp)
	{
		Return_Ack(HADRWARE_NOTREADY);
		return;
	}
//测量速度可选择5、10、 20、40、80、160、320、640、1000、2000SPS
//配置采样速度
	Return_Ack(SUCCESSFUL_EXECUTION);
	if(Sample_Control.Ch_Select & Ch_A_ID)
	{
		GPIOA->BSRRH=GPIO_Pin_4;
	}
	if(Sample_Control.Ch_Select & Ch_B_ID)
	{
		GPIOC->BSRRH=GPIO_Pin_15;
	}
	if(Sample_Control.Ch_Select & Ch_C_ID)
	{
		GPIOA->BSRRH=GPIO_Pin_0;
	}
	if(Sample_Control.Ch_Select & Ch_D_ID)
	{
		GPIOA->BSRRH=GPIO_Pin_1;
	}
	//Sample_Control.Sample_Rate=0x03;
	ADS1248_WriteReg(ADS_SYS0, Sample_Control.Sample_Rate | 0x70);
	ADS1248_WriteByte(ADS_RDATAC);
	ADS1248_CS(0x00);
	
	if(Sample_Control.Ch_Select & Ch_A_ID)A_Ch_START;
	if(Sample_Control.Ch_Select & Ch_B_ID)B_Ch_START;
	if(Sample_Control.Ch_Select & Ch_C_ID)C_Ch_START;
	if(Sample_Control.Ch_Select & Ch_D_ID)D_Ch_START;
	
//单次采样    20150815已完成
	if(Sample_Control.Sample_Type == 0x00)
	{		
			Sample_Control.Ch_DATASize=1;
//			while(! (Ch_A_DATA.Sample_Flag | Ch_B_DATA.Sample_Flag  | \
//							 Ch_C_DATA.Sample_Flag | Ch_D_DATA.Sample_Flag) ) ;//等待数据转换完成
			Delay(210);//等待数据转换完成
			if(Ch_A_DATA.Sample_Flag)
									{	
											A_Ch_STOP;//屏蔽通道A中断
											Ch_A_DATA.Sample_Flag=0;//清零采样标志位
											Ch_A_DATA.Count++;			//采样次数计数器+1
											ADS1248_Sample(Ch_A_ID);//读取A通道
											ADS1248_WriteByte(ADS_SDATAC);//发送停止采样命令
									}
			if(Ch_B_DATA.Sample_Flag)
									{
											B_Ch_STOP;											
											Ch_B_DATA.Sample_Flag=0;
											Ch_B_DATA.Count++;
											ADS1248_Sample(Ch_B_ID);
											ADS1248_WriteByte(ADS_SDATAC);
									}
			if(Ch_C_DATA.Sample_Flag)
									{
											C_Ch_STOP;											
											Ch_C_DATA.Sample_Flag=0;
											Ch_C_DATA.Count++;
											ADS1248_Sample(Ch_C_ID);
											ADS1248_WriteByte(ADS_SDATAC);
									}
			if(Ch_D_DATA.Sample_Flag)
									{
											D_Ch_STOP;											
											Ch_D_DATA.Sample_Flag=0;
											Ch_D_DATA.Count++;
											ADS1248_Sample(Ch_D_ID);
											ADS1248_WriteByte(ADS_SDATAC);
									}														
			if(Sample_Control.Ch_DATASize==Ch_A_DATA.Count)
									{
												Return_SampleData(Ch_A_ID,Sample_Control.Ch_DATASize);//返回结果值
												Ch_A_DATA.Count=0;//清零采样次数计数器
									}//清零缓存
			if(Sample_Control.Ch_DATASize==Ch_B_DATA.Count)
									{
												Return_SampleData(Ch_B_ID,Sample_Control.Ch_DATASize);
												Ch_B_DATA.Count=0;
									}
			if(Sample_Control.Ch_DATASize==Ch_C_DATA.Count)
									{
												Return_SampleData(Ch_C_ID,Sample_Control.Ch_DATASize);
												Ch_C_DATA.Count=0;
									}
			if(Sample_Control.Ch_DATASize==Ch_D_DATA.Count)
									{
												Return_SampleData(Ch_D_ID,Sample_Control.Ch_DATASize);
												Ch_D_DATA.Count=0;
									}										
#ifdef	Debug_EN
			printf("Sampling has been completed...\r\n");
#endif
			GPIO_SetBits(GPIOB,GPIO_Pin_15);
	    return;
}
	//非单次采样
	if( Sample_Control.Sample_Type == 0x01 )
	{	
		if(Sample_Control.Sample_Time==0x00)//一直采样
		{
			SampleTime_Flag=0;//清零SYStick标志位
			SampleTime=1;
		}
		if(Sample_Control.Sample_Time != 0x00)//按时长采样
		{
			SampleTime_Flag=1;
			SampleTime=Sample_Control.Sample_Time;
		}
#ifdef	Debug_EN
		printf("Sampling,Please waiting...\r\n");
#endif
		while(SampleTime)
		{
				//思想：以固定通道来确定同步信号，以最先产生中断信号的通道作为标志实现拉高拉低中断信号
				if(Ch_A_DATA.Sample_Flag)
										{
												//判断是否已经向TPA发送中断，没有发送则发送，否则不发送
												Ch_A_DATA.Sample_Flag=0;//清零标志位
												Ch_A_DATA.Count++;
												ADS1248_Sample(Ch_A_ID);												
										}						
				if(Ch_B_DATA.Sample_Flag)
										{
												Ch_B_DATA.Sample_Flag=0;
												Ch_B_DATA.Count++;
												ADS1248_Sample(Ch_B_ID);												
										}						
				if(Ch_C_DATA.Sample_Flag)
										{
												Ch_C_DATA.Sample_Flag=0;
												Ch_C_DATA.Count++;
												ADS1248_Sample(Ch_C_ID);												
										}					
				if(Ch_D_DATA.Sample_Flag)
										{
												Ch_D_DATA.Sample_Flag=0;
												Ch_D_DATA.Count++;
												ADS1248_Sample(Ch_D_ID);												
										}
										
				if(Sample_Control.Ch_DATASize==Ch_A_DATA.Count)
				{
												Ch_A_DATA.Count=0;//清零采样计数器
												Return_SampleData(Ch_A_ID,Sample_Control.Ch_DATASize);//返回数据
												Clear_Flag_TPA |= 0x01;
				}
				if(   Ch_A_ID & Clear_Flag_TPA & Send_Flag_TPA  )
				{GPIO_ResetBits(GPIOB,GPIO_Pin_3);Send_Flag_TPA=0;Clear_Flag_TPA &=0x00; }//拉低TPA中断，清零发送中断标志位
		
				if(Sample_Control.Ch_DATASize==Ch_B_DATA.Count)
				{
												Ch_B_DATA.Count=0;
												Return_SampleData(Ch_B_ID,Sample_Control.Ch_DATASize);
												Clear_Flag_TPA |= 0x02;
				}
				if(   Ch_B_ID & Clear_Flag_TPA & Send_Flag_TPA   )
				{GPIO_ResetBits(GPIOB,GPIO_Pin_3);Send_Flag_TPA=0;Clear_Flag_TPA &=0x00; }//拉低TPA中断，清零发送中断标志位
		
				if(Sample_Control.Ch_DATASize==Ch_C_DATA.Count)
										{
												Ch_C_DATA.Count=0;
												Return_SampleData(Ch_C_ID,Sample_Control.Ch_DATASize);
												Clear_Flag_TPA |= 0x04;
										}
				if(   Ch_C_ID & Clear_Flag_TPA & Send_Flag_TPA   )
				{GPIO_ResetBits(GPIOB,GPIO_Pin_3);Send_Flag_TPA=0;Clear_Flag_TPA &=0x00; }//拉低TPA中断，清零发送中断标志位
		
				if(Sample_Control.Ch_DATASize==Ch_D_DATA.Count)
										{
												Ch_D_DATA.Count=0;
												Return_SampleData(Ch_D_ID,Sample_Control.Ch_DATASize);
												Clear_Flag_TPA |= 0x08;
										}
				if(   Ch_D_ID & Clear_Flag_TPA & Send_Flag_TPA   )
				{GPIO_ResetBits(GPIOB,GPIO_Pin_3);Send_Flag_TPA=0;Clear_Flag_TPA &=0x00; }//拉低TPA中断，清零发送中断标志位
		}	
		Clear_Flag_TPA=0;
		SampleTime_Flag=0;
		if(Sample_Control.Ch_Select & Ch_A_ID)
		{
			A_Ch_STOP;//屏蔽中断
			Ch_A_DATA.Sample_Flag=0;//清零标志位
			ADS1248_CS(Ch_A_ID);//拉低片选
			ADS1248_WriteByte(ADS_SDATAC);//发送停止采样命令
			Ch_A_DATA.Count=0;//清零采样计数器
		}
		if(Sample_Control.Ch_Select & Ch_B_ID)
		{
			B_Ch_STOP;
			Ch_B_DATA.Sample_Flag=0;
			ADS1248_CS(Ch_B_ID);
			ADS1248_WriteByte(ADS_SDATAC);
			Ch_B_DATA.Count=0;
		}
		if(Sample_Control.Ch_Select & Ch_C_ID)
		{
			C_Ch_STOP;
			Ch_C_DATA.Sample_Flag=0;
			ADS1248_CS(Ch_C_ID);
			ADS1248_WriteByte(ADS_SDATAC);
			Ch_C_DATA.Count=0;			
		}
		if(Sample_Control.Ch_Select & Ch_D_ID)
		{
			D_Ch_STOP;
			Ch_D_DATA.Sample_Flag=0;
			ADS1248_CS(Ch_D_ID);
			ADS1248_WriteByte(ADS_SDATAC);
			Ch_D_DATA.Count=0;
		}		
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

 

















