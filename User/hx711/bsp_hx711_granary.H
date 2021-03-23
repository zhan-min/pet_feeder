#ifndef __BSP_HX711_GRANARY_H
#define __BSP_HX711_GRANARY_H


#include "stm32f10x.h"


/* 校准参数,当发现测试出来的重量偏大时，增加该数值。
 *如果测试出来的重量偏小时，减小改数值。
 *该值可以为小数	
*/
#define gapvalue_granary 430

#define full_granary 3000


//IO设置
#define HX711_GRANARY_DOUT_GPIO_PORT    GPIOC
#define HX711_GRANARY_DOUT_GPIO_PIN     GPIO_Pin_10
#define HX711_GRANARY_DOUT_GPIO_CLK     RCC_APB2Periph_GPIOB

#define HX711_GRANARY_SCK_GPIO_PORT    GPIOC
#define HX711_GRANARY_SCK_GPIO_PIN     GPIO_Pin_11
#define HX711_GRANARY_SCK_GPIO_CLK     RCC_APB2Periph_GPIOB


extern void HX711_GRANARY_GPIO_Config(void);
extern uint16_t hx711_granary_read(void);

#endif /* __BSP_HX711_GRANARY_H */

