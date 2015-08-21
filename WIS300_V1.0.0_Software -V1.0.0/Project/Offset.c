
#include "main.h"



//通道归零，电压激励，采样率5SPS，电桥类型，三线制1/4桥
/*******************************************************************************
* Function Name  : Bubble_sort
* Description    : 对对应的通道采样21次，采用冒泡排序，取中间7个数据，求均值
* Input          : None
* Output         : None
* Return         : 直接将校准结果存储
*******************************************************************************/
float Bubble_sort(void)
{
	unsigned char i,j;
	float Data_temp[21],temp;

	for(i=0;i<21;i++)
	{
		temp=ADS1248_Sample();
		Delay(205);
		ADS1248_WriteByte(ADS_RDATA);
		Data_temp[i]=(((float)temp/0x7fffff)*2.0477);//应变极性转换
	}
	//冒泡排序，采样21次，从小到大排序，取中间7个数，求均值
	for(i=0;i<21;i++)
	{
		for(j=i+1;j<21;j++)
		{
			if(Data_temp[i]>Data_temp[j])//从小到大，改为"<"变为从大到小
			{
				temp=Data_temp[i];
				Data_temp[i]=Data_temp[j];
				Data_temp[j]=temp;
			}
		}
	}
	temp=(Data_temp[8]+Data_temp[9]+Data_temp[10]+Data_temp[11]+Data_temp[12]+Data_temp[13]+Data_temp[14])/7;
	temp=temp*((500+120)*(500+120))/(499*120*2.00*2.0477);
	return temp;
}
/*******************************************************************************
* Function Name  : Zero_Offsetx(),Half_offsetx()
* Description    : 通道校准程序，对对应的通道采样21次，由小到大排序取中间7个数据求平均作为最终的结果值
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Zero_Offset1(void)
{
	//设置ADS1248
#ifdef	Debug_EN
	printf("Channel A Setting to Zero,Please wait...\r\n");
#endif
	SPI1_CS1_LOW();
	ADS1248_Init(0x00,THREE_WIRE_SINGLE_BRIDGE);
	Delay(205);

	EEPROM_DATA.A_Zero_offset=Bubble_sort();
	EEPROM_DATA.A_Zero_offset=EEPROM_DATA.A_Zero_offset/ADS_PGA;
	//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取	
	I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS,(unsigned char *)&EEPROM_DATA.A_Zero_offset,sizeof(EEPROM_DATA.A_Zero_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
	printf("Channel A Setting OK!\r\n");
#endif
}
void Half_Offset1(void)
{
	//设置ADS1248
#ifdef	Debug_EN
	printf("Channel A Setting to Half,Please wait...\r\n");	
#endif
	SPI1_CS1_LOW();
	ADS1248_Init(0x00,THREE_WIRE_SINGLE_BRIDGE);
	ADS1248_WriteByte(ADS_RDATA);
	Delay(205);

	EEPROM_DATA.A_Half_offset=Bubble_sort();
	EEPROM_DATA.A_Half_offset=EEPROM_DATA.A_Half_offset/ADS_PGA;
	//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取	
	I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+4,(unsigned char *)&EEPROM_DATA.A_Half_offset,sizeof(EEPROM_DATA.A_Half_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
	printf("Channel A Setting OK!\r\n");
#endif
}
///通道B
void Zero_Offset2(void)
{
	//设置ADS1248
#ifdef	Debug_EN
	printf("Channel B Setting to Zero,Please wait...\r\n");
#endif
	SPI1_CS2_LOW();
	ADS1248_Init(0x00,THREE_WIRE_SINGLE_BRIDGE);
	ADS1248_WriteByte(ADS_RDATA);
	Delay(205);

	EEPROM_DATA.B_Zero_offset=Bubble_sort();
	EEPROM_DATA.B_Zero_offset=EEPROM_DATA.B_Zero_offset/ADS_PGA;
	//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取	
	I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+11,(unsigned char *)&EEPROM_DATA.B_Zero_offset,sizeof(EEPROM_DATA.B_Zero_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
	printf("Channel B Setting OK!\r\n");
#endif
}
void Half_Offset2(void)
{
	//设置ADS1248
#ifdef	Debug_EN
	printf("Channel B Setting to Half,Please wait...\r\n");	
#endif
	SPI1_CS2_LOW();
	ADS1248_Init(0x00,THREE_WIRE_SINGLE_BRIDGE);
	ADS1248_WriteByte(ADS_RDATA);
	Delay(205);

	EEPROM_DATA.B_Half_offset=Bubble_sort();
	EEPROM_DATA.B_Half_offset=EEPROM_DATA.B_Half_offset/ADS_PGA;
	//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取	
	I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+15,(unsigned char *)&EEPROM_DATA.B_Half_offset,sizeof(EEPROM_DATA.B_Half_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
	printf("Channel B Setting OK!\r\n");
#endif
}
///通道C
void Zero_Offset3(void)
{
	//设置ADS1248
#ifdef	Debug_EN
	printf("Channel C Setting to Zero,Please wait...\r\n");
#endif
	SPI1_CS3_LOW();
	ADS1248_Init(0x00,THREE_WIRE_SINGLE_BRIDGE);
	ADS1248_WriteByte(ADS_RDATA);
	Delay(205);
	
	EEPROM_DATA.C_Zero_offset=Bubble_sort();
	EEPROM_DATA.C_Zero_offset=EEPROM_DATA.C_Zero_offset/ADS_PGA;
	//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取	
	I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+22,(unsigned char *)&EEPROM_DATA.C_Zero_offset,sizeof(EEPROM_DATA.C_Zero_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
	printf("Channel C Setting OK!\r\n");
#endif
	//printf("%s(%d) Channel C Setting OK!\r\n",__FILE__,__LINE__);
}
void Half_Offset3(void)
{
	//设置ADS1248
#ifdef	Debug_EN
	printf("Channel C Setting to Half,Please wait...\r\n");	
#endif
	SPI1_CS3_LOW();
	ADS1248_Init(0x00,THREE_WIRE_SINGLE_BRIDGE);
	ADS1248_WriteByte(ADS_RDATA);
	Delay(205);

	EEPROM_DATA.C_Half_offset=Bubble_sort();
	EEPROM_DATA.C_Half_offset=EEPROM_DATA.C_Half_offset/ADS_PGA;
	//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取	
	I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+26,(unsigned char *)&EEPROM_DATA.C_Half_offset,sizeof(EEPROM_DATA.C_Half_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
	printf("Channel C Setting OK!\r\n");
#endif
}
///通道D
void Zero_Offset4(void)
{
	//设置ADS1248
#ifdef	Debug_EN
	printf("Channel D Setting to Zero,Please wait...\r\n");
#endif
	SPI1_CS4_LOW();
	ADS1248_Init(0x00,THREE_WIRE_SINGLE_BRIDGE);
	ADS1248_WriteByte(ADS_RDATA);
	Delay(205);

	EEPROM_DATA.D_Zero_offset=Bubble_sort();
	EEPROM_DATA.D_Zero_offset=EEPROM_DATA.D_Zero_offset/ADS_PGA;
	//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取	
	I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+33,(unsigned char *)&EEPROM_DATA.D_Zero_offset,sizeof(EEPROM_DATA.D_Zero_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
	printf("Channel D Setting OK!\r\n");
#endif
}
void Half_Offset4(void)
{
	//设置ADS1248
#ifdef	Debug_EN
	printf("Channel D Setting to Half,Please wait...\r\n");	
#endif
	SPI1_CS4_LOW();
	ADS1248_Init(0x00,THREE_WIRE_SINGLE_BRIDGE);
	ADS1248_WriteByte(ADS_RDATA);
	Delay(205);

	EEPROM_DATA.D_Half_offset=Bubble_sort();
	EEPROM_DATA.D_Half_offset=EEPROM_DATA.D_Half_offset/ADS_PGA;
	//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取	
	I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+37,(unsigned char *)&EEPROM_DATA.D_Half_offset,sizeof(EEPROM_DATA.D_Half_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
	printf("Channel D Setting OK!\r\n");
#endif
}

/*******************************************************************************
* Function Name  : Channel_Offset
* Description    : 通道自动校准指令，收到校准指令后进入该函数，根据指令完成校准
* Input          : None
* Output         : None
* Return         : 直接将校准结果存储
*******************************************************************************/
unsigned char Channel_Offset(void)
{
	unsigned char DATA_temp1,DATA_temp2;
	DATA_temp1 = *(data_p+8);/////////////此处可使用内存copy技术  memcpy
	DATA_temp2 = *(data_p+9);
	if(DATA_temp1 & 0x01)
	{
		if(DATA_temp2 == 0x00)
		{
			Zero_Offset1();//通道A 0点校准
		}
		else if(DATA_temp2 == 0x01)
		{
			Half_Offset1();//通道A 半量程校准
		}
		else
		{
			Return_Ack(PARAMETERS_ILLEGAL);//返回错误信息
			return ERROR;									 //返回
		}
	}
	if(DATA_temp1 & 0x02)
	{
		if(DATA_temp2 == 0x00)
		{
			Zero_Offset2();//通道B 0点校准
		}
		else if(DATA_temp2 == 0x01)
		{
			Half_Offset2();//通道B 半量程校准
		}
		else
		{
			Return_Ack(PARAMETERS_ILLEGAL);//返回错误信息
			return ERROR;									 //返回
		}
	}
	if(DATA_temp1 & 0x04)
	{
		if(DATA_temp2 == 0x00)
		{
			Zero_Offset3();//通道C 0点校准
		}
		else if(DATA_temp2 == 0x01)
		{
			Half_Offset3();//通道C 半量程校准
		}
		else
		{
			Return_Ack(PARAMETERS_ILLEGAL);//返回错误信息
			return ERROR;									 //返回
		}
	}
	if(DATA_temp1 & 0x08)
	{
		if(DATA_temp2 == 0x00)
		{
			Zero_Offset4();//通道D 0点校准
		}
		else if(DATA_temp2 == 0x01)
		{
			Half_Offset4();//通道D 半量程校准
		}
		else
		{
			Return_Ack(PARAMETERS_ILLEGAL);//返回错误信息
			return ERROR;									 //返回
		}
	}
	Return_Ack(SUCCESSFUL_EXECUTION);
	return SUCCESS;
}


/*******************************************************************************
* Function Name  : CALIBRATION_PARAMETERS_SET()
* Description    : 手动校准，无意义
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CALIBRATION_PARAMETERS_SET(void)
{
	int Zero_offsetDATA,Half_offsetDATA;
	unsigned char Channel_Select;
	Channel_Select=*(data_p+8);
	Zero_offsetDATA=( ((*(data_p+9))<<24)  | ((*(data_p+10))<<16) | ((*(data_p+11))<<8) | ((*(data_p+12))) );
	Half_offsetDATA=( ((*(data_p+13))<<24) | ((*(data_p+14))<<16) | ((*(data_p+15))<<8) | ((*(data_p+16))) );
	
	//printf("%x  %x\r\n",Zero_offsetDATA,Half_offsetDATA);
	if(Channel_Select == 0x01)
	{
		EEPROM_DATA.A_Zero_offset=((float)Zero_offsetDATA/1000000000);
		EEPROM_DATA.A_Half_offset=((float)Half_offsetDATA/1000000000);
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS,(unsigned char *)&EEPROM_DATA.A_Zero_offset,sizeof(EEPROM_DATA.A_Zero_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+4,(unsigned char *)&EEPROM_DATA.A_Half_offset,sizeof(EEPROM_DATA.A_Half_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
		printf("Channel A Setting OK!\r\n");
#endif
		Return_Ack(SUCCESSFUL_EXECUTION);
	}
	if(Channel_Select == 0x02)
	{
		EEPROM_DATA.B_Zero_offset=((float)Zero_offsetDATA/1000000000);
		EEPROM_DATA.B_Half_offset=((float)Half_offsetDATA/1000000000);
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+11,(unsigned char *)&EEPROM_DATA.B_Zero_offset,sizeof(EEPROM_DATA.B_Zero_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+15,(unsigned char *)&EEPROM_DATA.B_Half_offset,sizeof(EEPROM_DATA.B_Half_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
		printf("Channel B Setting OK!\r\n");
#endif
		Return_Ack(SUCCESSFUL_EXECUTION);
	}
		if(Channel_Select == 0x04)
	{
		EEPROM_DATA.C_Zero_offset=((float)Zero_offsetDATA/1000000000);
		EEPROM_DATA.C_Half_offset=((float)Half_offsetDATA/1000000000);
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+22,(unsigned char *)&EEPROM_DATA.C_Zero_offset,sizeof(EEPROM_DATA.C_Zero_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+26,(unsigned char *)&EEPROM_DATA.C_Half_offset,sizeof(EEPROM_DATA.C_Half_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
		printf("Channel C Setting OK!\r\n");
#endif
		Return_Ack(SUCCESSFUL_EXECUTION);
	}
		if(Channel_Select == 0x08)
	{
		EEPROM_DATA.D_Zero_offset=((float)Zero_offsetDATA/1000000000);
		EEPROM_DATA.D_Half_offset=((float)Half_offsetDATA/1000000000);
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+33,(unsigned char *)&EEPROM_DATA.D_Zero_offset,sizeof(EEPROM_DATA.D_Zero_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
		I2C_WriteS_24C(EEPROM_DATA_BASEADDRESS+37,(unsigned char *)&EEPROM_DATA.D_Half_offset,sizeof(EEPROM_DATA.D_Half_offset));//校准完成后，将数据写入EEPROM，，主程序在上电时从EEPROM中读取
#ifdef	Debug_EN
		printf("Channel D Setting OK!\r\n");
#endif
		Return_Ack(SUCCESSFUL_EXECUTION);
	}
}





