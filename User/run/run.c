#include "run.h"
#include "stm32f10x.h"
#include "wifi.h"
#include "bsp_stepper_motor.h"
#include "bsp_key_exti.h"
#include "bsp_hx711_granary.h"
#include "bsp_hx711_export.h"


/*看情况删除或保留*/
#include "bsp_usart.h" 
#include "bsp_adc.h"
#include "bsp_led.h"




/*
******************************************************************
*                               变量
******************************************************************
*/

//全局变量
uint32_t granary_peel = 0;
uint32_t export_peel  = 0;


//消息队列
//rt_mq_t getwave_status_queue = RT_NULL;//采集完成标志

//信号量控制块
static rt_sem_t quick_feed_sem = RT_NULL;

/* 定义线程控制块 */
static rt_thread_t granary_weight_thread = NULL;
static rt_thread_t export_weight_thread = NULL;
static rt_thread_t key1_scan_thread  = RT_NULL;
static rt_thread_t wifi_uart_service_thread  = RT_NULL;
static rt_thread_t quick_feed_thread  = RT_NULL;



/*
*************************************************************************
*                             辅助函数
*************************************************************************
*/





/*
*************************************************************************
*                             线程定义
*************************************************************************
*/

/**
 * @brief  粮桶余粮重量检测
 * @param  Null
 * @return Null
 * @note   Null
 */
void granary_weight(void* parameter)
{
	uint32_t tem, granary_weight = 0;
	while(1)
	{
		LED2_ON;
		tem = hx711_granary_read() - granary_peel;
		if(tem - granary_weight > 50)
		{
			granary_weight = tem;
			mcu_dp_value_update(DPID_SURPLUS_GRAIN, granary_weight);
		}
	}
}


/**
 * @brief  已出粮剩余重量检测
 * @param  Null
 * @return Null
 * @note   Null
 */
void export_weight(void* parameter)
{
	uint32_t tem, export_weight = 0;
	while(1)
	{
		tem = hx711_export_read() - export_peel;
		export_weight = tem;
		mcu_dp_value_update(DPID_WEIGHT, export_weight);
		if(tem - export_weight > 50)
		{
			export_weight = tem;
			mcu_dp_value_update(DPID_WEIGHT, export_weight);
		}
	}
}


/**
 * @brief  按键1扫描
 * @param  Null
 * @return Null
 * @note   Null
 */
void key1_scan(void* parameter)
{
	while(1)
	{
		if( GPIO_ReadInputDataBit(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == KEY_ON )
		{
			// 松手检测
			while( GPIO_ReadInputDataBit(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == KEY_ON );
			rt_sem_release(quick_feed_sem);
		}
	}	
}


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
		LED2_TOGGLE;
		rt_thread_delay(50);
		wifi_uart_service();
	}	
}


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
		feed(1);
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
	
//	granary_weight_thread =                                   /* 线程控制块指针 */
//							rt_thread_create( "granary_weight",           /* 线程名字 */
//																granary_weight,             /* 线程入口函数 */
//																RT_NULL,               /* 线程入口函数参数 */
//																512,                   /* 线程栈大小 */
//																6,                     /* 线程的优先级 */
//																20);                   /* 线程时间片 */
//	
//	if (granary_weight_thread != RT_NULL) 
//        rt_thread_startup(granary_weight_thread);
	
	export_weight_thread =                                   /* 线程控制块指针 */
							rt_thread_create( "export_weight",           /* 线程名字 */
																export_weight,             /* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																512,                   /* 线程栈大小 */
																6,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
	
	if (export_weight_thread != RT_NULL) 
        rt_thread_startup(export_weight_thread);
	
	key1_scan_thread =                                   /* 线程控制块指针 */
							rt_thread_create( "key1_scan",           /* 线程名字 */
																key1_scan,             /* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																512,                   /* 线程栈大小 */
																6,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
	
	if (key1_scan_thread != RT_NULL) 
        rt_thread_startup(key1_scan_thread);
	
	
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

