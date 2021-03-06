#ifndef __BSP_KEY_EXTI_H
#define __BSP_KEY_EXTI_H

#include "stm32f10x.h"


#define  KEY_ON      1
#define  KEY_OFF     0

#define KEY1_INT_GPIO_PIN    GPIO_Pin_0
#define KEY1_INT_GPIO_PORT   GPIOA
#define KEY1_INT_GPIO_CLK    RCC_APB2Periph_GPIOA

#define KEY2_INT_GPIO_PIN    GPIO_Pin_13
#define KEY2_INT_GPIO_PORT   GPIOC
#define KEY2_INT_GPIO_CLK    RCC_APB2Periph_GPIOC

void EXTI_Key_Config(void);
	
#endif /* __BSP_KEY_EXTI_H */
