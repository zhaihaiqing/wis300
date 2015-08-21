#ifndef __DATA_Process_H
#define __DATA_Process_H




void Channel_A_Filter(unsigned char A_Filter_K);//声明A通道一阶滞后滤波器函数
void Channel_B_Filter(unsigned char B_Filter_K);//声明B通道一阶滞后滤波器函数
void Channel_C_Filter(unsigned char C_Filter_K);//声明C通道一阶滞后滤波器函数
void Channel_D_Filter(unsigned char D_Filter_K);//声明D通道一阶滞后滤波器函数

void ADS1248_B(void);														//声明ADS1248数据处理函数

signed long ADS1248_Sample(void);								//声明ADS1248采样函数

void Sample_Instruction_Control(void);					//声明采样指令控制函数





#endif


