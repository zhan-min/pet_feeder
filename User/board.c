/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
 
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "wifi.h"
#include "bsp_stepper_motor.h"
#include "bsp_hx711_granary.h"
#include "bsp_hx711_export.h"
#include "bsp_key_exti.h"


#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024*10
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
  /* 初始化SysTick */
  SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );	
	LED2_ON;
	USART_Config();
	wifi_protocol_init();
	LED_GPIO_Config();
	STEP_MOTOR_GPIO_Config();
	EXTI_Key_Config();
	HX711_EXPORT_GPIO_Config();
	HX711_GRANARY_GPIO_Config();
	
	
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}


/**
  * @brief  重映射串口DEBUG_USARTx到rt_kprintf()函数
  *   Note：DEBUG_USARTx是在bsp_usart.h中定义的宏，默认使用串口1
  * @param  str：要输出到串口的字符串
  * @retval 无
  *
  * @attention
  * 
  */
void rt_hw_console_output(const char *str)
{	
	/* 进入临界段 */
    rt_enter_critical();

	/* 直到字符串结束 */
    while (*str!='\0')
	{
		/* 换行 */
        if (*str=='\n')
		{
			USART_SendData(SYS_USARTx, '\r'); 
			while (USART_GetFlagStatus(SYS_USARTx, USART_FLAG_TXE) == RESET);
		}

		USART_SendData(SYS_USARTx, *str++); 				
		while (USART_GetFlagStatus(SYS_USARTx, USART_FLAG_TXE) == RESET);	
	}	

	/* 退出临界段 */
    rt_exit_critical();
}


/**
  * @brief  为finsh提供控制台读入接口
  *   Note：DEBUG_USARTx是在bsp_usart.h中定义的宏，默认使用串口1
  * @param  str：要输出到串口的字符串
  * @retval 无
  *
  * @attention
  * 
  */
char rt_hw_console_getchar(void)
{
	int ch = -1;
	
	if( USART_GetFlagStatus(SYS_USARTx, USART_FLAG_RXNE) != RESET )
	{
		ch = (int)USART_ReceiveData(SYS_USARTx);
	}
	else
	{
		if( USART_GetFlagStatus(SYS_USARTx, USART_FLAG_ORE) != RESET )
		{
			USART_ClearFlag(SYS_USARTx, USART_FLAG_TC);
		}
		rt_thread_delay(10);
	}
	return ch;
}

