
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
void Channel_A_Filter(unsigned char A_Filter_K)//Channel A
{
	if(A_Channel_CFG.adc<A_Channel_CFG.Old_DATA)
	{
		A_Channel_CFG.adc=A_Channel_CFG.Old_DATA-(((A_Channel_CFG.Old_DATA-A_Channel_CFG.adc)*A_Filter_K)/256);
		A_Channel_CFG.Old_DATA=A_Channel_CFG.adc;
	}
	else if(A_Channel_CFG.adc>A_Channel_CFG.Old_DATA)
	{
		A_Channel_CFG.adc=A_Channel_CFG.Old_DATA+(((A_Channel_CFG.adc-A_Channel_CFG.Old_DATA)*A_Filter_K)/256);
		A_Channel_CFG.Old_DATA=A_Channel_CFG.adc;
	}
	else 
	{
		A_Channel_CFG.Old_DATA=A_Channel_CFG.adc;
	}
}

void Channel_B_Filter(unsigned char Filter_K)//Channel B
{
	if(B_Channel_CFG.adc<B_Channel_CFG.Old_DATA)
	{
		B_Channel_CFG.adc=B_Channel_CFG.Old_DATA-(((B_Channel_CFG.Old_DATA-B_Channel_CFG.adc)*Filter_K)/256);
		B_Channel_CFG.Old_DATA=B_Channel_CFG.adc;
	}
	else if(B_Channel_CFG.adc>B_Channel_CFG.Old_DATA)
	{
		B_Channel_CFG.adc=B_Channel_CFG.Old_DATA+(((B_Channel_CFG.adc-B_Channel_CFG.Old_DATA)*Filter_K)/256);
		B_Channel_CFG.Old_DATA=B_Channel_CFG.adc;
	}
	else 
	{
		B_Channel_CFG.Old_DATA=B_Channel_CFG.adc;
	}
}

void Channel_C_Filter(unsigned char Filter_K)//Channel C
{
	if(C_Channel_CFG.adc<C_Channel_CFG.Old_DATA)
	{
		C_Channel_CFG.adc=C_Channel_CFG.Old_DATA-(((C_Channel_CFG.Old_DATA-C_Channel_CFG.adc)*Filter_K)/256);
		C_Channel_CFG.Old_DATA=C_Channel_CFG.adc;
	}
	else if(C_Channel_CFG.adc>C_Channel_CFG.Old_DATA)
	{
		C_Channel_CFG.adc=C_Channel_CFG.Old_DATA+(((C_Channel_CFG.adc-C_Channel_CFG.Old_DATA)*Filter_K)/256);
		C_Channel_CFG.Old_DATA=C_Channel_CFG.adc;
	}
	else 
	{
		C_Channel_CFG.Old_DATA=C_Channel_CFG.adc;
	}
}

void Channel_D_Filter(unsigned char Filter_K)//Channel D
{
	if(D_Channel_CFG.adc<D_Channel_CFG.Old_DATA)
	{
		D_Channel_CFG.adc=D_Channel_CFG.Old_DATA-(((D_Channel_CFG.Old_DATA-D_Channel_CFG.adc)*Filter_K)/256);
		D_Channel_CFG.Old_DATA=D_Channel_CFG.adc;
	}
	else if(D_Channel_CFG.adc>D_Channel_CFG.Old_DATA)
	{
		D_Channel_CFG.adc=D_Channel_CFG.Old_DATA+(((D_Channel_CFG.adc-D_Channel_CFG.Old_DATA)*Filter_K)/256);
		D_Channel_CFG.Old_DATA=D_Channel_CFG.adc;
	}
	else 
	{
		D_Channel_CFG.Old_DATA=D_Channel_CFG.adc;
	}
}

/*******************************************************************************
* Function Name  : ADS1248_Sample
* Description    : ADS1248底层采样驱动程序
* Input          : None
* Output         : None
* Return         : ADC采样值，有符号长整型
*******************************************************************************/
signed long ADS1248_Sample(void)
{
	unsigned char result[3]={0,0,0};
	signed long adcVal=0;
#ifdef WatchDog_EN
	IWDG_ReloadCounter();//喂狗
#endif
	ADS1248_WriteByte(ADS_RDATA);
	result[0]=ADS1248_WriteByte(ADS_NOP);
	result[1]=ADS1248_WriteByte(ADS_NOP);
	result[2]=ADS1248_WriteByte(ADS_NOP);
	
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
void ADS1248_B(void)
{
	unsigned char DATA_temp;
	//GPIO_SetBits(GPIOB,GPIO_Pin_3);
	/***********通道A*************/
	if(Sample_Control.Channel_OE & Ch_A_ID)
	{
		SPI1_CS1_LOW();
		A_Channel_CFG.adc=ADS1248_Sample();
		if(EEPROM_DATA.A_Filter_Val)								 //如果参数为00，滤波器失能
		{
			if(count==1)															 //给滤波器赋初值
			{
				A_Channel_CFG.Old_DATA=A_Channel_CFG.adc;//首次采样结果不进行滤波
			}
			Channel_A_Filter(EEPROM_DATA.A_Filter_Val);//一阶滤波算法，参数为滤波系数，表明本次数据所占的权重
		}
		A_Channel_CFG.adcV=((float)A_Channel_CFG.adc/0x7fffff)*2.0477;//采样电压值变换
		A_HalfSubZero=EEPROM_DATA.A_Half_offset-EEPROM_DATA.A_Zero_offset;
		if(A_HalfSubZero<0)
		{
			A_HalfSubZero=0-A_HalfSubZero;
		}
		switch(A_Channel_CFG.Bridge_Type)
		{
			 case 0x00:
					A_Channel_CFG.strainVal=A_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.A_K_Val);
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal/ADS1_PGA;
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal*1000-EEPROM_DATA.A_Zero_offset;	//此处X1000是应为K值放大了1000倍
					A_Channel_CFG.strainVal=(A_Channel_CFG.strainVal*5000000)/A_HalfSubZero;//半量程修正
					break;
			 case 0x01:
					A_Channel_CFG.strainVal=A_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.A_K_Val);																 //应变值转换
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal/ADS1_PGA;
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal*1000-EEPROM_DATA.A_Zero_offset;	//此处X1000是应为K值放大了1000倍																					 //修正0点	
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal*5000000/A_HalfSubZero;//半量程修正
					break;
			 case 0x02:
					A_Channel_CFG.strainVal=A_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.A_K_Val);
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal/ADS1_PGA;
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal/2;
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal*1000000000000;
			 //自动增益控制：应添加到对应通道的末尾处
					if((A_Channel_CFG.adcV>2)|(A_Channel_CFG.adcV<-2))
					{	
							ADS1_PGA=ADS1_PGA>>1;
							DATA_temp=ADS1248_ReadREG(ADS_SYS0);
							DATA_temp=DATA_temp&0x0f;
							if(ADS1_PGA==64)
							{					
								ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
							}
					}
					break;
			 case 0x04:
					A_Channel_CFG.strainVal=A_Channel_CFG.adcV*57600/(29486.88*EEPROM_DATA.A_K_Val);																 //应变值转换
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal/ADS1_PGA;
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal/4;
					A_Channel_CFG.strainVal=A_Channel_CFG.strainVal*1000000000000;
					//自动增益控制：应添加到对应通道的末尾处
					if((A_Channel_CFG.adcV>2)|(A_Channel_CFG.adcV<-2))
					{
							ADS1_PGA=ADS1_PGA>>1;
							DATA_temp=ADS1248_ReadREG(ADS_SYS0);//注意此处是否会出现抖动数据
							DATA_temp=DATA_temp&0x0f;
							if(ADS1_PGA==64)
							{					
								ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
							}
							if(ADS1_PGA==32)
							{
								ADS1248_WriteReg(ADS_SYS0,0x50|DATA_temp);
							}
					}
					break;
			 default:
					break;
		}
		A_Channel_DATAbuf[count-1]=A_Channel_CFG.strainVal;
//#ifdef	Debug_EN
//				printf("%d[A]:%.6fmV %.3f\r\n", count,A_Channel_CFG.adcV*1000,A_Channel_CFG.strainVal/1000);
//#endif
	}
	/***********通道B*************/
	if(Sample_Control.Channel_OE & Ch_B_ID)
	{
		SPI1_CS2_LOW();
		B_Channel_CFG.adc=ADS1248_Sample();
		if(EEPROM_DATA.B_Filter_Val)								 //如果参数为00，滤波器失能
		{
			if(count==1)															 //给滤波器赋初值
			{
				B_Channel_CFG.Old_DATA=B_Channel_CFG.adc;//首次采样结果不进行滤波
			}
			Channel_B_Filter(EEPROM_DATA.B_Filter_Val);//一阶滤波算法，参数为滤波系数，表明本次数据所占的权重
		}
		B_Channel_CFG.adcV=(((float)B_Channel_CFG.adc/0x7fffff)*2.0477);//采样电压值变换
		B_HalfSubZero=EEPROM_DATA.B_Half_offset-EEPROM_DATA.B_Zero_offset;
		if(B_HalfSubZero<0)
		{
			B_HalfSubZero=0-B_HalfSubZero;
		}
		switch(B_Channel_CFG.Bridge_Type)
		{
			case 0x00:
				B_Channel_CFG.strainVal=B_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.B_K_Val);
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal/ADS2_PGA;
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal*1000-EEPROM_DATA.B_Zero_offset;	//此处X1000是应为K值放大了1000倍
				B_Channel_CFG.strainVal=(B_Channel_CFG.strainVal*5000000)/B_HalfSubZero;//半量程修正				
				break;
			case 0x01:
				B_Channel_CFG.strainVal=B_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.B_K_Val);																 //应变值转换
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal/ADS2_PGA;
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal*1000-EEPROM_DATA.B_Zero_offset;	//此处X1000是应为K值放大了1000倍																					 //修正0点	
				B_Channel_CFG.strainVal=(B_Channel_CFG.strainVal*5000000)/B_HalfSubZero;//半量程修正
				break;	
			case 0x02:
				B_Channel_CFG.strainVal=B_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.B_K_Val);
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal/ADS2_PGA;
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal/2;
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal*1000000000000;
				//自动增益控制：应添加到对应通道的末尾处
				if((B_Channel_CFG.adcV>2)|(B_Channel_CFG.adcV<-2))
				{
						ADS2_PGA=ADS2_PGA>>1;
						DATA_temp=ADS1248_ReadREG(ADS_SYS0);
						DATA_temp=DATA_temp&0x0f;
						if(ADS2_PGA==64)
						{					
							ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
						}
				}
				break;
			case 0x04:
				B_Channel_CFG.strainVal=B_Channel_CFG.adcV*57600/(29486.88*EEPROM_DATA.B_K_Val);																 //应变值转换
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal/ADS2_PGA;
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal/4;
				B_Channel_CFG.strainVal=B_Channel_CFG.strainVal*1000000000000;
				//自动增益控制：应添加到对应通道的末尾处
				if((B_Channel_CFG.adcV>2)|(B_Channel_CFG.adcV<-2))
				{
						ADS2_PGA=ADS2_PGA>>1;
						DATA_temp=ADS1248_ReadREG(ADS_SYS0);//注意此处是否会出现抖动数据
						DATA_temp=DATA_temp&0x0f;
						if(ADS2_PGA==64)
						{					
							ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
						}
						if(ADS2_PGA==32)
						{
							ADS1248_WriteReg(ADS_SYS0,0x50|DATA_temp);
						}
				}
				break;
		  default:
				break;
		}	
		B_Channel_DATAbuf[count-1]=B_Channel_CFG.strainVal;
//			
//#ifdef	Debug_EN		
//				printf("%d[B]:%.6fmV %.3f\r\n", count,B_Channel_CFG.adcV*1000,B_Channel_CFG.strainVal/1000);
//#endif
				
	}
	/***********通道C*************/	
	if(Sample_Control.Channel_OE & Ch_C_ID)
	{
	  SPI1_CS3_LOW();
		C_Channel_CFG.adc=ADS1248_Sample();
		if(EEPROM_DATA.C_Filter_Val)								 //如果参数为00，滤波器失能
		{
			if(count==1)															 //给滤波器赋初值
			{
				C_Channel_CFG.Old_DATA=C_Channel_CFG.adc;//首次采样结果不进行滤波
			}
			Channel_C_Filter(EEPROM_DATA.C_Filter_Val);//一阶滤波算法，参数为滤波系数，表明本次数据所占的权重
		}
		C_Channel_CFG.adcV=(((float)C_Channel_CFG.adc/0x7fffff)*2.0477);//采样电压值变换
		C_HalfSubZero=EEPROM_DATA.C_Half_offset-EEPROM_DATA.C_Zero_offset;
		if(C_HalfSubZero<0)
		{
			C_HalfSubZero=0-C_HalfSubZero;
		}
		switch(C_Channel_CFG.Bridge_Type)
		{
			case 0x00:
				C_Channel_CFG.strainVal=C_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.C_K_Val);																 //应变值转换
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal/ADS3_PGA;
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal*1000-EEPROM_DATA.C_Zero_offset;//此处X1000是应为K值放大了1000倍																						 //修正0点	
				C_Channel_CFG.strainVal=(C_Channel_CFG.strainVal*5000000)/C_HalfSubZero;//半量程修正
				break;
			case 0x01:
				C_Channel_CFG.strainVal=C_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.C_K_Val);																 //应变值转换
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal/ADS3_PGA;
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal*1000-EEPROM_DATA.C_Zero_offset;//此处X1000是应为K值放大了1000倍																					 //修正0点	
				C_Channel_CFG.strainVal=(C_Channel_CFG.strainVal*5000000)/C_HalfSubZero;//半量程修正
				break;
			case 0x02:
				C_Channel_CFG.strainVal=C_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.C_K_Val);
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal/ADS3_PGA;
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal/2;
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal*1000000000000;
			//自动增益控制：应添加到对应通道的末尾处
				if((C_Channel_CFG.adcV>2)|(C_Channel_CFG.adcV<-2))
				{
						ADS3_PGA=ADS3_PGA>>1;
						DATA_temp=ADS1248_ReadREG(ADS_SYS0);
						DATA_temp=DATA_temp&0x0f;
						if(ADS3_PGA==64)
						{					
							ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
						}	
				}
				break;
			case 0x04:
				C_Channel_CFG.strainVal=C_Channel_CFG.adcV*57600/(29486.88*EEPROM_DATA.C_K_Val);																 //应变值转换
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal/ADS3_PGA;
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal/4;
				C_Channel_CFG.strainVal=C_Channel_CFG.strainVal*1000000000000;
			

				//自动增益控制：应添加到对应通道的末尾处
				if((C_Channel_CFG.adcV>2)|(C_Channel_CFG.adcV<-2))
				{

						ADS3_PGA=ADS3_PGA>>1;
						DATA_temp=ADS1248_ReadREG(ADS_SYS0);//注意此处是否会出现抖动数据
						DATA_temp=DATA_temp&0x0f;
						if(ADS3_PGA==64)
						{					
							ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
						}
						if(ADS3_PGA==32)
						{
							ADS1248_WriteReg(ADS_SYS0,0x50|DATA_temp);
						}
				}
				break;
			default:
				break;
		}
		C_Channel_DATAbuf[count-1]=C_Channel_CFG.strainVal;
//#ifdef	Debug_EN		
//				printf("%d[C]:%.6fmV %.3f\r\n", count,C_Channel_CFG.adcV*1000,C_Channel_CFG.strainVal/1000);
//#endif	
	}
	/***********通道D*************/	
	if(Sample_Control.Channel_OE & Ch_D_ID)
	{
		SPI1_CS4_LOW();
		D_Channel_CFG.adc=ADS1248_Sample();
		if(EEPROM_DATA.D_Filter_Val)								 //如果参数为00，滤波器失能
		{
			if(count==1)															 //给滤波器赋初值
			{
				D_Channel_CFG.Old_DATA=D_Channel_CFG.adc;//首次采样结果不进行滤波
			}
			Channel_D_Filter(EEPROM_DATA.D_Filter_Val);//一阶滤波算法，参数为滤波系数，表明本次数据所占的权重
		}
		D_Channel_CFG.adcV=(((float)D_Channel_CFG.adc/0x7fffff)*2.0477);//采样电压值变换
		D_HalfSubZero=EEPROM_DATA.D_Half_offset-EEPROM_DATA.D_Zero_offset;
		if(D_HalfSubZero<0)
		{
			D_HalfSubZero=0-D_HalfSubZero;
		}
		switch(D_Channel_CFG.Bridge_Type)
		{
			case 0x00:
				D_Channel_CFG.strainVal=D_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.D_K_Val);																 //应变值转换
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal/ADS4_PGA;
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal*1000-EEPROM_DATA.D_Zero_offset;//此处X1000是应为K值放大了1000倍																						 //修正0点	
				D_Channel_CFG.strainVal=(D_Channel_CFG.strainVal*5000000)/D_HalfSubZero;//半量程修正
				break;
			case 0x01:
				D_Channel_CFG.strainVal=D_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.D_K_Val);																 //应变值转换
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal/ADS4_PGA;
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal*1000-EEPROM_DATA.D_Zero_offset;//此处X1000是应为K值放大了1000倍																					 //修正0点	
				D_Channel_CFG.strainVal=(D_Channel_CFG.strainVal*5000000)/D_HalfSubZero;//半量程修正
				break;
			case 0x02:
				D_Channel_CFG.strainVal=D_Channel_CFG.adcV*384400/(122616.276*EEPROM_DATA.D_K_Val);
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal/ADS4_PGA;
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal/2;
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal*1000000000000;
				//自动增益控制：应添加到对应通道的末尾处
				if((D_Channel_CFG.adcV>2)|(D_Channel_CFG.adcV<-2))
				{
						ADS4_PGA=ADS4_PGA>>1;
						DATA_temp=ADS1248_ReadREG(ADS_SYS0);
						DATA_temp=DATA_temp&0x0f;
						if(ADS4_PGA==64)
						{					
							ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
						}
				}
				break;
			case 0x04:
				D_Channel_CFG.strainVal=D_Channel_CFG.adcV*57600/(29486.88*EEPROM_DATA.D_K_Val);																 //应变值转换
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal/ADS4_PGA;
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal/4;
				D_Channel_CFG.strainVal=D_Channel_CFG.strainVal*1000000000000;
				//自动增益控制：应添加到对应通道的末尾处
				if((D_Channel_CFG.adcV>2)|(D_Channel_CFG.adcV<-2))
				{
						ADS4_PGA=ADS4_PGA>>1;
						DATA_temp=ADS1248_ReadREG(ADS_SYS0);//注意此处是否会出现抖动数据
						DATA_temp=DATA_temp&0x0f;
						if(ADS4_PGA==64)
						{					
							ADS1248_WriteReg(ADS_SYS0,0x60|DATA_temp);
						}
						if(ADS4_PGA==32)
						{
							ADS1248_WriteReg(ADS_SYS0,0x50|DATA_temp);
						}
				}
				break;
			default:
				break;
		}
		D_Channel_DATAbuf[count-1]=D_Channel_CFG.strainVal;
//#ifdef	Debug_EN
//				printf("%d[D]:%.6fmV %.3f\r\n", count,D_Channel_CFG.adcV*1000,D_Channel_CFG.strainVal/1000);
//#endif		
	}	
//GPIO_ResetBits(GPIOB,GPIO_Pin_3);	
}
/*******************************************************************************
* Function Name  : Channel_CFG_Check
* Description    : 在采样前检查通道是否配置，如果没有配置，直接返回错误信息
* Input          : None
* Output         : None
* Return         : 配置正确返回SUCCESS，对应采样的通道如没有配置返回ERROR
*******************************************************************************/
static unsigned char Channel_CFG_Check(void)
{
	if((Sample_Control.Channel_OE)&Ch_A_ID)
	{
		if(!A_Channel_CFG_Flag)
		{
			//Return_Ack(HADRWARE_NOTREADY);
			return ERROR;
		}
	}
	if((*(data_p+8))&Ch_B_ID)
	{
		if(!B_Channel_CFG_Flag)
		{
			//Return_Ack(HADRWARE_NOTREADY);
			return ERROR;
		}
	}
	if((*(data_p+8))&Ch_C_ID)
	{
		if(!C_Channel_CFG_Flag)
		{
			//Return_Ack(HADRWARE_NOTREADY);
			return ERROR;
		}
	}
	if((*(data_p+8))&Ch_D_ID)
	{
		if(!D_Channel_CFG_Flag)
		{
			//Return_Ack(HADRWARE_NOTREADY);
			return ERROR;
		}
	}
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
	
	Sample_Control.Channel_OE=*(data_p+8);
	Sample_Control.Sample_Rate=*(data_p+9);
	Sample_Control.Sample_Type=*(data_p+10);
	Sample_Control.Sample_Time=((*(data_p+11))<<8)|(*(data_p+12));
	Channel_DATASize=*(data_p+13);
	SampleTime=Sample_Control.Sample_Time;
////////////////////////////////////////////////////////////////////////
/////////收到采样指令后，首先检查对应通道是否完成配置，否则返回硬件没有准备好信息
	DATA_temp=Channel_CFG_Check();
	if(!DATA_temp)
	{
		Return_Ack(HADRWARE_NOTREADY);
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
//测量速度可选择5、10、 20、40、80、160、320、640、1000、2000SPS
//配置采样速度
	switch(Sample_Control.Sample_Rate)
	{
		case 0x00:
				TIM_ACC=800;
				Set_Sample_Speed = 0x00;//1sps
				break;
		case 0x01:
				TIM_ACC=160;
				Set_Sample_Speed = 0x00;//5sps
				break;
		case 0x02:
				TIM_ACC=80;
				Set_Sample_Speed = 0x01;//10sps
				break;
		case 0x03:
				TIM_ACC=40;
				Set_Sample_Speed=0x02;//20sps
				break;
		case 0x04:
				TIM_ACC=20;
				Set_Sample_Speed=0x03;//40sps
				break;
		case 0x05:
				TIM_ACC=10;
				Set_Sample_Speed=0x04;//80sps
				break;
		case 0x06:
				TIM_ACC=5;
				Set_Sample_Speed=0x05;//160sps
				break;
		case 0x07:
				TIM_ACC=4;
				Set_Sample_Speed=0x06;//200sps or 320sps
				break;
		default:
				TIM_ACC=800;
				Set_Sample_Speed = 0x00;//1sps
				break;		
	}
	if(Sample_Control.Channel_OE & 0x01)
	{
		SPI1_CS1_LOW();
		ADS1248_WriteReg(ADS_SYS0,Set_Sample_Speed | 0x70); //0000 0000，Bit7:必须设置为0，Bit6-4：选择增益：1-128倍，Bit3-0：选择测量速度 ：5-2000SPS
		ADS1248_delay(1);
	}
	if(Sample_Control.Channel_OE & 0x02)
	{
		SPI1_CS2_LOW();
		ADS1248_WriteReg(ADS_SYS0,Set_Sample_Speed | 0x70); //0000 0000，Bit7:必须设置为0，Bit6-4：选择增益：1-128倍，Bit3-0：选择测量速度 ：5-2000SPS
		ADS1248_delay(1);
	}
	if(Sample_Control.Channel_OE & 0x04)
	{
		SPI1_CS3_LOW();
		ADS1248_WriteReg(ADS_SYS0,Set_Sample_Speed | 0x70); //0000 0000，Bit7:必须设置为0，Bit6-4：选择增益：1-128倍，Bit3-0：选择测量速度 ：5-2000SPS
		ADS1248_delay(1);
	}
	if(Sample_Control.Channel_OE & 0x08)
	{
		SPI1_CS4_LOW();
		ADS1248_WriteReg(ADS_SYS0,Set_Sample_Speed | 0x70); //0000 0000，Bit7:必须设置为0，Bit6-4：选择增益：1-128倍，Bit3-0：选择测量速度 ：5-2000SPS
		ADS1248_delay(1);
	}
	
	
	
	
	
	
	
	
	
	
	
	
	



	TIM3_Configuration(); //
	//如果收到相关通道采样命令，则，启动定时器3，开始采样。
	if((Sample_Control.Channel_OE&Ch_A_ID)|(Sample_Control.Channel_OE&Ch_B_ID)|(Sample_Control.Channel_OE&Ch_C_ID)|(Sample_Control.Channel_OE&Ch_D_ID))	
	{
		TIM_Cmd(TIM3, ENABLE);//启动定时器，开始采样
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	}
	Return_Ack(SUCCESSFUL_EXECUTION);
//单次采样    20150815已完成
	if(Sample_Control.Sample_Type == 0x00)
	{
		count++;
		GPIO_PinReverse(GPIOB,GPIO_Pin_1);
		//在开始采样时给TPA中断信号
		if(Send_Flag_TPA)
		{
				GPIO_ResetBits(GPIOB,GPIO_Pin_3);//向TPA发送中断
				Send_Flag_TPA=0;
		}
		ADS1248_B();		
		Channel_DATASize=1;
		if(Sample_Control.Channel_OE & Ch_A_ID)
								{
											Return_SampleData(Ch_A_ID,Channel_DATASize);	
								}//清零缓存
		if(Sample_Control.Channel_OE & Ch_B_ID)
								{
											Return_SampleData(Ch_B_ID,Channel_DATASize);	
								}
		if(Sample_Control.Channel_OE & Ch_C_ID)
								{
											Return_SampleData(Ch_C_ID,Channel_DATASize);	
								}
		if(Sample_Control.Channel_OE & Ch_D_ID)
								{
											Return_SampleData(Ch_D_ID,Channel_DATASize);	
								}
		GPIO_SetBits(GPIOB,GPIO_Pin_3);//清除
		Send_Flag_TPA=1;
				count=0;
#ifdef	Debug_EN
		printf("Sampling has been completed...\r\n");
#endif
		//Return_Ack(SAMPLE_COMPLETE);//返回采样结束
		GPIO_PinReverse(GPIOB,GPIO_Pin_1);
		return;
	}
	//一直采样   20150814已完成
	if((Sample_Control.Sample_Type == 0x01)&&(Sample_Control.Sample_Time == 0x00))
	{
#ifdef	Debug_EN
		printf("Sampling,Please waiting...\r\n");
#endif
		while(1)
		{
			if(Sample_Flag)
			{
				count++;
				GPIO_PinReverse(GPIOB,GPIO_Pin_1);
				//在开始采样时给TPA中断信号
				if(Send_Flag_TPA)
				{
					GPIO_ResetBits(GPIOB,GPIO_Pin_3);//向TPA发送中断
					Send_Flag_TPA=0;
				}
				ADS1248_B();				
				Sample_Flag=0;
				if(Channel_DATASize==count)
				{					
						if(Sample_Control.Channel_OE & Ch_A_ID)
										{
												Return_SampleData(Ch_A_ID,Channel_DATASize);	
										}
						if(Sample_Control.Channel_OE & Ch_B_ID)
									{
												Return_SampleData(Ch_B_ID,Channel_DATASize);
									}
						if(Sample_Control.Channel_OE & Ch_C_ID)
									{
												Return_SampleData(Ch_C_ID,Channel_DATASize);
									}
						if(Sample_Control.Channel_OE & Ch_D_ID)
									{
												Return_SampleData(Ch_D_ID,Channel_DATASize);
									}
						count=0;	
						GPIO_PinReverse(GPIOB,GPIO_Pin_1);
					}
				
			}
		}	
	}
	//按时长采样    20150814已完成
	if((Sample_Control.Sample_Type == 0x01)&&(Sample_Control.Sample_Time != 0x00))
	{
#ifdef	Debug_EN
		printf("Sampling,Please waiting...\r\n");
#endif
		while(SampleTime)
		{
			if(Sample_Flag)
			{				
				count++;
				GPIO_PinReverse(GPIOB,GPIO_Pin_1);
				//在开始采样时给TPA中断信号
				if(Send_Flag_TPA)
				{
					GPIO_ResetBits(GPIOB,GPIO_Pin_3);//向TPA发送中断
					Send_Flag_TPA=0;
				}
				ADS1248_B();				
				Sample_Flag=0;
				if(Channel_DATASize==count)
				{						
								if(Sample_Control.Channel_OE & Ch_A_ID)
								{	
													Return_SampleData(Ch_A_ID,Channel_DATASize);	
								}						
								if(Sample_Control.Channel_OE & Ch_B_ID)
								{
													Return_SampleData(Ch_B_ID,Channel_DATASize);
								}
								if(Sample_Control.Channel_OE & Ch_C_ID)
								{
													Return_SampleData(Ch_C_ID,Channel_DATASize);
								}
								if(Sample_Control.Channel_OE & Ch_D_ID)
								{
													Return_SampleData(Ch_D_ID,Channel_DATASize);
									
								}
								count=0;//
								GPIO_PinReverse(GPIOB,GPIO_Pin_1);
					}
				
			}	
		}
		GPIO_SetBits(GPIOB,GPIO_Pin_3);//清除
		Send_Flag_TPA=1;
#ifdef	Debug_EN
		printf("Sampling has been completed...\r\n");
#endif
		//Return_Ack(SAMPLE_COMPLETE);//返回采样结束SAMPLE_COMPLETE
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		return;
	}	
}

 

















