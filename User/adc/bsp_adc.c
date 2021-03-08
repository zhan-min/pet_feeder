#include "bsp_adc.h"
#include "run.h"
#include "bsp_usart.h"
#include "delay.h"


/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// ADCx_1 GPIO ��ʼ��
	ADCx_1_GPIO_APBxClock_FUN(ADCx_1_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = ADCx_1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADCx_1_PORT, &GPIO_InitStructure);
	
	// ADCx_2 GPIO ��ʼ��
	ADCx_2_GPIO_APBxClock_FUN(ADCx_2_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = ADCx_2_PIN1;//��һ��ͨ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADCx_2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADCx_2_PIN2;//�ڶ���ͨ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADCx_2_PORT, &GPIO_InitStructure);
}

/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static  void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	//����ADC1����ģʽ
	ADCx_1_APBxClock_FUN(ADCx_1_CLK, ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = NOFADCx_1_CHANEL;
	
	ADC_Init(ADCx_1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12MHz
	ADC_RegularChannelConfig(ADCx_1, ADCx_1_CHANNEL, 1, ADC_SampleTime_71Cycles5);//ת��ʱ��7us
	
	ADC_ITConfig(ADCx_1, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADCx_1, ENABLE);
	
  // ��ʼ��ADC1 У׼�Ĵ���  
	ADC_ResetCalibration(ADCx_1);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADCx_1));	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADCx_1);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADCx_1));
	
	ADC_SoftwareStartConvCmd(ADCx_1, ENABLE);
	
	
	
	//����ADC2����ģʽ
	ADCx_1_APBxClock_FUN(ADCx_2_CLK, ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = NOFADCx_1_CHANEL;
	
	ADC_Init(ADCx_2, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12MHz
	
		
	//ʹ��ADCx_2���ⲿ����ת��
  ADC_ExternalTrigConvCmd(ADCx_2, ENABLE);
	
	ADC_ITConfig(ADCx_2, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADCx_2, ENABLE);
	
  // ��ʼ��ADC1 У׼�Ĵ���  
	ADC_ResetCalibration(ADCx_2);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADCx_2));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADCx_2);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADCx_2));	
}


//static void ADCx_NVIC_Config(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	//���ȼ�����
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}

void ADCx_Init(void)
{
	//ADCx_NVIC_Config();
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}




/*
uint16_t Get_X_Data(void)
{
	uint16_t XData = 0;
	ADC_RegularChannelConfig(ADCx_2, ADCx_2_X_CHANNEL, 1, ADC_SampleTime_71Cycles5);//ת��ʱ��7us
	ADC_SoftwareStartConvCmd(ADCx_2, ENABLE);
	
	while(ADC_GetITStatus(ADCx_2, ADC_IT_EOC) != SET);
	XData = ADC_GetConversionValue(ADCx_2);
	ADC_ClearITPendingBit(ADCx_2, ADC_IT_EOC);
	return XData;
}


uint16_t Get_Y_Data(void)
{
	uint16_t YData = 0;
	ADC_RegularChannelConfig(ADCx_2, ADCx_2_Y_CHANNEL, 1, ADC_SampleTime_71Cycles5);//ת��ʱ��7us
	ADC_SoftwareStartConvCmd(ADCx_2, ENABLE);
	
	while(ADC_GetITStatus(ADCx_2, ADC_IT_EOC) != SET);
	YData = ADC_GetConversionValue(ADCx_2);
	ADC_ClearITPendingBit(ADCx_2, ADC_IT_EOC);
	return YData;
}
*/

