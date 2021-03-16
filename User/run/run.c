#include "run.h"
#include "stm32f10x.h"
#include "wifi.h"
#include "bsp_stepper_motor.h"


/*看情况删除或保留*/
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h" 
#include "bsp_adc.h"
#include "bsp_led.h"




/*
******************************************************************
*                               变量
******************************************************************
*/

//全局变量
FunctionalState quick_feed_enable = ENABLE;


//消息队列
//rt_mq_t getwave_status_queue = RT_NULL;//采集完成标志

//信号量控制块
static rt_sem_t quick_feed_sem = RT_NULL;

/* 定义线程控制块 */
static rt_thread_t wifi_uart_service_thread  = RT_NULL;
static rt_thread_t quick_feed_thread  = RT_NULL;



/*
*************************************************************************
*                             辅助函数
*************************************************************************
*/


/**
  * @brief  快速喂食
  * @param  无
  * @retval None
  */
void quick_feed(void* paprameter)
{
	while(1)
	{
		rt_sem_take(quick_feed_sem, RT_WAITING_FOREVER);
		if( quick_feed_enable == ENABLE)
		{
			feed(1);
		}		
	}	
}


/*
*************************************************************************
*                             线程定义
*************************************************************************
*/
/**
 * @brief  wifi串口数据处理服务
 * @param  Null
 * @return Null
 * @note   由涂鸦SDK提供
 */
void tuya_wifi_uart_service(void* parameter)
{
	while(1)
	{
		wifi_uart_service();
	}	
}


void Run(void)
{
	/**********创建消息队列************/
//	getwave_status_queue = rt_mq_create("getwave_status_queue", 1, 1, RT_IPC_FLAG_FIFO);
	
	/**********创建信号量************/
	quick_feed_sem = rt_sem_create("quick_feed_n",     /* 信号量名字 */
																	0,                /* 初始值 */
																	RT_IPC_FLAG_PRIO); /* 信号量模式 */
	
	/**********创建线程************/
	wifi_uart_service_thread =                           /* 线程控制块指针 */
							rt_thread_create( "wifi_uart_service",   /* 线程名字 */
																tuya_wifi_uart_service,/* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																512,                   /* 线程栈大小 */
																5,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
   if (wifi_uart_service_thread != RT_NULL) 
        rt_thread_startup(wifi_uart_service_thread);
	 
	 
	 quick_feed_thread =                                 /* 线程控制块指针 */
							rt_thread_create( "quick_feed",          /* 线程名字 */
																quick_feed,            /* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																512,                   /* 线程栈大小 */
																4,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
   if (quick_feed_thread != RT_NULL) 
        rt_thread_startup(quick_feed_thread);
}



/* ------------------------------------------end of file---------------------------------------- */

