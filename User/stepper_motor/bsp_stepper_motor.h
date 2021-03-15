#ifndef __BSP_STEPPER_MOTOR_H
#define __BSP_STEPPER_MOTOR_H

#include "stm32f10x.h"

/* 定义步进电机的方向、脉冲、脱机控制口*/
#define STEP_MOTOR_DIR_GPIO_PORT       GPIOA
#define STEP_MOTOR_DIR_GPIO_PIN        GPIO_Pin_2
#define STEP_MOTOR_DIR_GPIO_CLK        RCC_APB2Periph_GPIOA

#define STEP_MOTOR_PLUSE_GPIO_PORT     GPIOA
#define STEP_MOTOR_PLUSE_GPIO_PIN      GPIO_Pin_3
#define STEP_MOTOR_PLUSE_GPIO_CLK      RCC_APB2Periph_GPIOA

#define STEP_MOTOR_OFFLINE_GPIO_PORT   GPIOA
#define STEP_MOTOR_OFFLINE_GPIO_PIN    GPIO_Pin_8
#define STEP_MOTOR_OFFLINE_GPIO_CLK    RCC_APB2Periph_GPIOA


void step_motor_enable(void);
void step_motor_offline(void);

void step_motor_forward(uint8_t n);
void step_motor_reverse(uint8_t n);


#endif /* __BSP_STEPPER_MOTOR_H */

