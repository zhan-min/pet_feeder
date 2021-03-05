#include "stm32f10x_gpio.h"
#include "bsp_PS2.h"
#include "bsp_adc.h"

//X��Y��������ADC��

static void EXTI_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	//����PS2��SW�ж�
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);	
}

void PS2_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	//�����ж����ȼ�
	EXTI_NVIC_Config();
	
	//��ʼ��GPIO
	RCC_APB2PeriphClockCmd(PS2_SW_CLK, ENABLE);
	
	//��ʼ��SW
	GPIO_InitStruct.GPIO_Pin = PS2_SW_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(PS2_SW_PORT, &GPIO_InitStruct);
	
	//��ʼ��EXTI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	
	//��ʼ��SW�ж�
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
}

uint8_t Read_X_Data(void)
{
	return Get_X_Data();
}

uint8_t Read_Y_Data(void)
{
	return Get_Y_Data();
}

uint8_t Read_SW_Data(void)
{
	return GPIO_ReadInputDataBit(PS2_SW_PORT, PS2_SW_PIN);
}
