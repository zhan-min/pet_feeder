#ifndef __BSP_HX711_GRANARY_H
#define __BSP_HX711_GRANARY_H


#include "stm32f10x.h"

//IO…Ë÷√
#define HX711_GRANARY_DOUT_GPIO_PORT    GPIOC
#define HX711_GRANARY_DOUT_GPIO_PIN     GPIO_Pin_10
#define HX711_GRANARY_DOUT_GPIO_CLK     RCC_APB2Periph_GPIOB

#define HX711_GRANARY_SCK_GPIO_PORT    GPIOC
#define HX711_GRANARY_SCK_GPIO_PIN     GPIO_Pin_11
#define HX711_GRANARY_SCK_GPIO_CLK     RCC_APB2Periph_GPIOB


extern void HX711_GRANARY_GPIO_Config(void);
extern uint32_t hx711_granary_read(void);

#endif /* __BSP_HX711_GRANARY_H */

