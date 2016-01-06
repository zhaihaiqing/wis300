
#include "main.h"

unsigned char FirstReadUart_Flag=0x00;
unsigned char ReturnData_Count=0x00;

/*******************************************************************************
* Function Name  : Filter_A\B\C\D
* Description    : ��ADS1248�ɼ��������ݽ���һ���ͺ��˲��㷨��
									 ϵ��A_Filter_K��Ϊ�����ȣ���ʾ��ǰ�������ܽ������ռ��Ȩ�أ�
									 ϵ��Խ��������Խ�ߣ�������ƽ����Խ�͡�
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
* Description    : ADS1248�ײ������������
* Input          : None
* Output         : None
* Return         : ADC����ֵ���з��ų�����
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
	
	adcVal=(signed long)( result[0] << 24 | result[1] << 16 | result[2] << 8 );//��������
	if(adcVal>0x7fffffff)//����λת��
	{
		adcVal=(~adcVal)+0x01;
	}
	adcVal=adcVal>>8;
	return adcVal;
}

/*******************************************************************************
* Function Name  : ADS1248_B
* Description    : ADS1248��������
* Input          : None
* Output         : None
* Return         : ADC����ֵ��������
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
	IWDG_ReloadCounter();//ι��
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
	adc=ADS1248_ReadData();
	//�˴����˲�
	adcV=((float)adc/0x7fffff)*2.0477;//������ѹֵ�任
	if(HalfSubZero<0)HalfSubZero=0-HalfSubZero;
	switch(Bridgr_Type)
	{
		case 0x84:
					strainVal=adcV*384400/(122616.276*K_Val);
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal*1000-Zero_offset;	//�˴�X1000��ӦΪKֵ�Ŵ���1000��
					strainVal=(strainVal*5000000)/HalfSubZero;//����������			
			break;
		case 0x0c:
					strainVal=adcV*384400/(122616.276*K_Val);																 //Ӧ��ֵת��
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal*1000-Zero_offset;	//�˴�X1000��ӦΪKֵ�Ŵ���1000��																					 //����0��	
					strainVal=strainVal*5000000/HalfSubZero;//����������		
			break;
		case 0x04:
					strainVal=adcV*384400/(122616.276*K_Val);
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal/2;
					strainVal=strainVal*1000000000000;
			 //�Զ�������ƣ�Ӧ���ӵ���Ӧͨ����ĩβ��
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
		case 0x20:
					strainVal=adcV*57600/(29486.88*K_Val);				//Ӧ��ֵת��
					strainVal=strainVal/PGA_Gain;
					strainVal=strainVal/4;
					strainVal=strainVal*1000000000000;
					//�Զ�������ƣ�Ӧ���ӵ���Ӧͨ����ĩβ��
					if((adcV>2)|(adcV<-2))
					{
							PGA_Gain=PGA_Gain>>1;
							DATA_temp=ADS1248_ReadREG(ADS_SYS0);//ע��˴��Ƿ����ֶ�������
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
* Description    : �ڲ���ǰ���ͨ���Ƿ�����
* Input          : None
* Output         : None
* Return         : �����Ҫ������ͨ��û�����ã�����ERROR�������÷���SUCCESS��
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
* Description    : ��������������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Sample_Instruction_Control(void)
{
	unsigned char DATA_temp=SUCCESS;	
	
	memcpy(&Sample_Control,data_p+7,*(data_p+4));
	Sample_Control.Sample_Time=htons(Sample_Control.Sample_Time);

	if((Sample_Control.Ch_Select&0x0f)!=ERROR)												//���ͨ��
	{
		if(Sample_Control.Sample_Rate<0x04)														//������Ƶ��
		{
			if( (Sample_Control.Sample_Type==0x00)|(Sample_Control.Sample_Type==0x01))//����������
			{												
				if(    (Sample_Control.Ch_DATASize>0x00) &&  (Sample_Control.Ch_DATASize<0x07)  )//������ݰ�����
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

//�����ٶȿ�ѡ��5��10�� 20��40��80��160��320��640��1000��2000SPS
//���ò����ٶ�
	Return_Ack(SAMPLE_INSTRUCTION,SUCCESSFUL_EXECUTION);
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
	ADS1248_WriteReg(ADS_SYS0, Sample_Control.Sample_Rate | 0x70);
	ADS1248_WriteByte(ADS_RDATAC);
	ADS1248_CS(0x00);
	
	if(Sample_Control.Ch_Select & Ch_A_ID)A_Ch_START;
	if(Sample_Control.Ch_Select & Ch_B_ID)B_Ch_START;
	if(Sample_Control.Ch_Select & Ch_C_ID)C_Ch_START;
	if(Sample_Control.Ch_Select & Ch_D_ID)D_Ch_START;
	
//���β���    20150815�����
	if(Sample_Control.Sample_Type == 0x00)
	{		
			Sample_Control.Ch_DATASize=1;
//			while(! (Ch_A_DATA.Sample_Flag | Ch_B_DATA.Sample_Flag  | \
//							 Ch_C_DATA.Sample_Flag | Ch_D_DATA.Sample_Flag) ) ;//�ȴ�����ת�����
			Delay(210);//�ȴ�����ת�����
			if(Ch_A_DATA.Sample_Flag)
									{	
											A_Ch_STOP;//����ͨ��A�ж�
											Ch_A_DATA.Sample_Flag=0;//���������־λ
											Ch_A_DATA.Count++;			//��������������+1
											ADS1248_Sample(Ch_A_ID);//��ȡAͨ��
											ADS1248_WriteByte(ADS_SDATAC);//����ֹͣ��������
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
												Return_SampleData(Ch_A_ID,Sample_Control.Ch_DATASize);//���ؽ��ֵ
												Ch_A_DATA.Count=0;//�����������������
									}//���㻺��
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
	//�ǵ��β���
	if( Sample_Control.Sample_Type == 0x01 )
	{	
		if(Sample_Control.Sample_Time==0x00)//һֱ����
		{
			SampleTime_Flag=0;//����SYStick��־λ
			SampleTime=1;
		}
		if(Sample_Control.Sample_Time != 0x00)//��ʱ������
		{
			SampleTime_Flag=1;
			SampleTime=Sample_Control.Sample_Time;
		}
#ifdef	Debug_EN
		printf("Sampling,Please waiting...\r\n");
#endif
		while(SampleTime)
		{
				if(TpaCommand_Sampling_Flag)
				{
					if(FirstReadUart_Flag == 0x00)
					{
					 data_p = get_active_message(&data_len);//ȷ��������ֻ��ȡһ�δ�������
					 FirstReadUart_Flag=0x01;
					}						
					if(*(data_p+6)==0xa1)
					{
						ReturnData_Count++;
						switch(*(data_p+7))//�˴�ֻ�ṩ��ѯ����
						{
							case 	 REQUEST_CHANNEL_CFG:			//��ѯͨ��������Ϣ01
													switch(ReturnData_Count)
													{
														case 1:
															Return_Channel_CFG();
														break;
														case 2:
															Return_Ack(REQUEST_CHANNEL_CFG,SUCCESSFUL_EXECUTION);
															//�����־λ������ACK����֮��
															FirstReadUart_Flag=0x00;
															TpaCommandLen = 0;//���������ݳ���
															TpaCommand_Sampling_Flag = 0;//�����־λ
															USART1_ClearBuf();//��մ��ڻ���
															ReturnData_Count=0;
														break;
														default:
															break;
													}
													break;
							case   REQUEST_CFG_AND_CALIBRATION:		//��ѯͨ�����ò�����У׼����  05
													switch(ReturnData_Count)
													{
														case 1:
															Return_Channel_CFG();
															//printf("1\r\n");
														break;
														case 2:
															Return_ChannelA_Offset();
															//printf("2\r\n");
														break;
														case 3:
															Return_ChannelB_Offset();
															//printf("3\r\n");
														break;
														case 4:
															Return_ChannelC_Offset();
															//printf("4\r\n");
														break;
														case 5:
															Return_ChannelD_Offset();
															//printf("5\r\n");
														break;
														case 6:
															Return_Ack(REQUEST_CFG_AND_CALIBRATION,SUCCESSFUL_EXECUTION);
															//printf("6\r\n");
															//�����־λ������ACK����֮��
															FirstReadUart_Flag=0x00;
															TpaCommandLen = 0;//���������ݳ���
															TpaCommand_Sampling_Flag = 0;//�����־λ
															USART1_ClearBuf();//��մ��ڻ���
															ReturnData_Count=0;
														break;
														default:
															break;
														
													}
													break;
							case 	 REQUEST_CALIBRATION_VAL:		//��ȡУ׼����   21
													switch(ReturnData_Count)
													{
														case 1:
															Return_ChannelA_Offset();
														break;
														case 2:
															Return_ChannelB_Offset();
														break;
														case 3:
															Return_ChannelC_Offset();
														break;
														case 4:
															Return_ChannelD_Offset();
														break;
														case 5:
															Return_Ack(REQUEST_CALIBRATION_VAL,SUCCESSFUL_EXECUTION);
															//�����־λ������ACK����֮��
															FirstReadUart_Flag=0x00;
															TpaCommandLen = 0;//���������ݳ���
															TpaCommand_Sampling_Flag = 0;//�����־λ
															USART1_ClearBuf();//��մ��ڻ���
															ReturnData_Count=0;
														break;
														default:
															break;
													}
													break;
							default:														
													break;
						}
					}
				}		
				//˼�룺�Թ̶�ͨ����ȷ��ͬ���źţ������Ȳ����ж��źŵ�ͨ����Ϊ��־ʵ�����������ж��ź�
				if(Ch_A_DATA.Sample_Flag)
										{
												//�ж��Ƿ��Ѿ���TPA�����жϣ�û�з������ͣ����򲻷���
												Ch_A_DATA.Sample_Flag=0;//�����־λ
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
												Ch_A_DATA.Count=0;//�������������
												Return_SampleData(Ch_A_ID,Sample_Control.Ch_DATASize);//��������
												Clear_Flag_TPA |= 0x01;
				}
				if(   Ch_A_ID & Clear_Flag_TPA & Send_Flag_TPA  )
				{GPIO_ResetBits(GPIOB,GPIO_Pin_3);Send_Flag_TPA=0;Clear_Flag_TPA &=0x00; }//����TPA�жϣ����㷢���жϱ�־λ
		
				if(Sample_Control.Ch_DATASize==Ch_B_DATA.Count)
				{
												Ch_B_DATA.Count=0;
												Return_SampleData(Ch_B_ID,Sample_Control.Ch_DATASize);
												Clear_Flag_TPA |= 0x02;
				}
				if(   Ch_B_ID & Clear_Flag_TPA & Send_Flag_TPA   )
				{GPIO_ResetBits(GPIOB,GPIO_Pin_3);Send_Flag_TPA=0;Clear_Flag_TPA &=0x00; }//����TPA�жϣ����㷢���жϱ�־λ
		
				if(Sample_Control.Ch_DATASize==Ch_C_DATA.Count)
										{
												Ch_C_DATA.Count=0;
												Return_SampleData(Ch_C_ID,Sample_Control.Ch_DATASize);
												Clear_Flag_TPA |= 0x04;
										}
				if(   Ch_C_ID & Clear_Flag_TPA & Send_Flag_TPA   )
				{GPIO_ResetBits(GPIOB,GPIO_Pin_3);Send_Flag_TPA=0;Clear_Flag_TPA &=0x00; }//����TPA�жϣ����㷢���жϱ�־λ
		
				if(Sample_Control.Ch_DATASize==Ch_D_DATA.Count)
										{
												Ch_D_DATA.Count=0;
												Return_SampleData(Ch_D_ID,Sample_Control.Ch_DATASize);
												Clear_Flag_TPA |= 0x08;
										}
				if(   Ch_D_ID & Clear_Flag_TPA & Send_Flag_TPA   )
				{GPIO_ResetBits(GPIOB,GPIO_Pin_3);Send_Flag_TPA=0;Clear_Flag_TPA &=0x00; }//����TPA�жϣ����㷢���жϱ�־λ
		}	
		Clear_Flag_TPA=0;
		SampleTime_Flag=0;
		if(Sample_Control.Ch_Select & Ch_A_ID)
		{
			A_Ch_STOP;//�����ж�
			Ch_A_DATA.Sample_Flag=0;//�����־λ
			ADS1248_CS(Ch_A_ID);//����Ƭѡ
			ADS1248_WriteByte(ADS_SDATAC);//����ֹͣ��������
			Ch_A_DATA.Count=0;//�������������
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

 
















