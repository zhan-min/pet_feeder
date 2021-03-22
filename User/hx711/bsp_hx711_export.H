#ifndef __BSP_HX711_H
#define __BSP_HX711_H


#include "stm32f10x.h"

//IO…Ë÷√
#define HX711_EXPORT_DOUT_GPIO_PORT    GPIOC
#define HX711_EXPORT_DOUT_GPIO_PIN     GPIO_Pin_8
#define HX711_EXPORT_DOUT_GPIO_CLK     RCC_APB2Periph_GPIOB

#define HX711_EXPORT_SCK_GPIO_PORT    GPIOC
#define HX711_EXPORT_SCK_GPIO_PIN     GPIO_Pin_9
#define HX711_EXPORT_SCK_GPIO_CLK     RCC_APB2Periph_GPIOB


extern void HX711_EXPORT_GPIO_Config(void);
extern uint32_t hx711_export_read(void);

#endif /* __BSP_HX711_H */

