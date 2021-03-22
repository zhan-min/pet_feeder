#ifndef __BSP_LIGHT_H
#define __BSP_LIGHT_H


#include "stm32f10x.h"


/* 定义light连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的light引脚 */
#define LIGHT_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LIGHT_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LIGHT_GPIO_PIN		  GPIO_Pin_5			        /* 连接到SCL时钟线的GPIO */




#define ON  0
#define OFF 1


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LIGHT_TOGGLE		 digitalToggle(LIGHT_GPIO_PORT,LIGHT_GPIO_PIN)
#define LIGHT_OFF		     digitalHi(LIGHT_GPIO_PORT,LIGHT_GPIO_PIN)
#define LIGHT_ON			   digitalLo(LIGHT_GPIO_PORT,LIGHT_GPIO_PIN)



void LIGHT_GPIO_Config(void);


#endif /* __BSP_LIGHT_H */

