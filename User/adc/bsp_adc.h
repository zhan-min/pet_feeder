#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f10x.h"


//��������
#define  ADCx_1_SampleNbr             300


// ˫ģʽʱ��ADC1��ADC2ת�������ݶ������ADC1�����ݼĴ�����
// ADC1���ڵ�ʮ��λ��ADC2���ڸ�ʮ��λ
// ˫ADCģʽ�ĵ�һ��ADC��������ADC1
#define    ADCx_1                           ADC1
#define    ADCx_1_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx_1_CLK                       RCC_APB2Periph_ADC1

#define    ADCx_1_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADCx_1_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADCx_1_PORT                      GPIOC
#define    ADCx_1_PIN                       GPIO_Pin_1
#define    ADCx_1_CHANNEL                   ADC_Channel_11

// ˫ADCģʽ�ĵڶ���ADC��������ADC2
#define    ADCx_2                           ADC2
#define    ADCx_2_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx_2_CLK                       RCC_APB2Periph_ADC2

#define    ADCx_2_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADCx_2_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADCx_2_PORT                      GPIOC

//�ɼ�X��ֵ
#define    ADCx_2_PIN1                       GPIO_Pin_4
#define    ADCx_2_X_CHANNEL                  ADC_Channel_14

//�ɼ�Y��ֵ
#define    ADCx_2_PIN2                       GPIO_Pin_5
#define    ADCx_2_Y_CHANNEL                  ADC_Channel_15

#define    NOFADCx_1_CHANEL                        1
#define    NOFADCx_2_CHANEL                        1



void ADCx_Init(void);
FlagStatus Get_Trigger_Status(float d0, float d1);
void Get_Wave(void* parameter);
uint16_t Get_X_Data(void);
uint16_t Get_Y_Data(void);

#endif /* __BSP_ADC_H */

