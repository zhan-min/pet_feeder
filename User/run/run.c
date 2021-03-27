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
struct date_time time_now;
struct meal_plan_struct meal_plan[10];
struct meal_plan_struct nearly_meal_plan;


uint16_t granary_peel = 0;
uint16_t export_peel  = 0;
uint8_t meal_plan_amount;


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
static rt_thread_t time_sys_thread = RT_NULL;



/*
*************************************************************************
*                             辅助函数
*************************************************************************
*/

/**
 * @brief  粮桶余粮重量检测
 * @param  Null
 * @return Null
 * @note   Null
 */
static void meal_plan_check(void)
{
	if(((nearly_meal_plan.week & 0x80) == 0) && time_now.min == nearly_meal_plan.min && time_now.hour == nearly_meal_plan.hour)
	{
		feed(nearly_meal_plan.amount);
		get_nearly_meal_plan();
	}
}



/*
*************************************************************************
*                             线程定义
*************************************************************************
*/



void time_sys(void* parameter)
{
	time_now.year = 2020;
	time_now.month = 1;
	time_now.day = 1;
	time_now.hour = 0;
	time_now.min = 0;
	time_now.sec = 0;
	time_now.week = 1;
	time_now.updata_state = ERROR;
	
	while(1)
	{
		//rt_kprintf("星期%d %d:%d:%d\n", time_now.week, time_now.hour, time_now.min, time_now.sec);
		//rt_kprintf("nearly meal plan: %d %d %d %d\n", nearly_meal_plan.week, nearly_meal_plan.hour, nearly_meal_plan.min,nearly_meal_plan.amount);
		rt_thread_mdelay(1000);
		time_now.sec ++;
		
		if(time_now.updata_state != SUCCESS)
		{
			mcu_get_system_time();//更新时间日期
			get_nearly_meal_plan();
		}
		
		if(time_now.sec >= 60)
		{
			time_now.sec = 0;
			time_now.min ++;
			
			meal_plan_check();
			
			if(time_now.min >= 60)
			{
				time_now.min = 0;
				time_now.hour ++;
				time_now.updata_state = ERROR;//日期的更新靠联网实现	
				if(time_now.hour >= 24)
				{
					time_now.hour = 0;
					time_now.week++;
					if(time_now.week >= 8)
					{
						time_now.week = 1;
					}
					time_now.updata_state = ERROR;//日期的更新靠联网实现	
				}
			}
		}
	}
}



/**
 * @brief  粮桶余粮重量检测
 * @param  Null
 * @return Null
 * @note   Null
 */
void granary_weight(void* parameter)
{
	uint32_t tem, granary_weight;
	uint8_t granary_weight_percent;
	while(1)
	{
		rt_thread_delay(50);
		tem = hx711_granary_read() - granary_peel;
		

		if(tem - granary_weight > full_granary/100)
		{
			granary_weight = tem;
			
			if( granary_weight >= full_granary )
			{
				granary_weight_percent = 100;
				mcu_dp_value_update(DPID_SURPLUS_GRAIN, granary_weight_percent);
			}
			else
			{
				granary_weight_percent = (granary_weight) / (full_granary/100);
				mcu_dp_value_update(DPID_SURPLUS_GRAIN, granary_weight_percent);
			}
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
		rt_thread_delay(50);
		tem = hx711_export_read() - export_peel;
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
		rt_thread_delay(50);
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
	
	time_sys_thread =                                   /* 线程控制块指针 */
							rt_thread_create( "time_sys",           /* 线程名字 */
																time_sys,             /* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																256,                   /* 线程栈大小 */
																2,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
	
	if (time_sys_thread != RT_NULL) 
        rt_thread_startup(time_sys_thread);
	
	granary_weight_thread =                                   /* 线程控制块指针 */
							rt_thread_create( "granary_weight",           /* 线程名字 */
																granary_weight,             /* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																256,                   /* 线程栈大小 */
																6,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
	
	if (granary_weight_thread != RT_NULL) 
        rt_thread_startup(granary_weight_thread);
	
	export_weight_thread =                                   /* 线程控制块指针 */
							rt_thread_create( "export_weight",           /* 线程名字 */
																export_weight,             /* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																256,                   /* 线程栈大小 */
																6,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
	
	if (export_weight_thread != RT_NULL) 
        rt_thread_startup(export_weight_thread);
	
	key1_scan_thread =                                   /* 线程控制块指针 */
							rt_thread_create( "key1_scan",           /* 线程名字 */
																key1_scan,             /* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																256,                   /* 线程栈大小 */
																6,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
	
	if (key1_scan_thread != RT_NULL) 
        rt_thread_startup(key1_scan_thread);
	
	
	wifi_uart_service_thread =                           /* 线程控制块指针 */
							rt_thread_create( "wifi_uart_service",   /* 线程名字 */
																tuya_wifi_uart_service,/* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																1024,                   /* 线程栈大小 */
																5,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
   if (wifi_uart_service_thread != RT_NULL) 
        rt_thread_startup(wifi_uart_service_thread);
	 
	 
	 quick_feed_thread =                                 /* 线程控制块指针 */
							rt_thread_create( "quick_feed",          /* 线程名字 */
																quick_feed,            /* 线程入口函数 */
																RT_NULL,               /* 线程入口函数参数 */
																256,                   /* 线程栈大小 */
																4,                     /* 线程的优先级 */
																20);                   /* 线程时间片 */
   if (quick_feed_thread != RT_NULL) 
        rt_thread_startup(quick_feed_thread);
	 else
	 {
		rt_kprintf("thread startup failed!!!");
	 }
}



/* ------------------------------------------end of file---------------------------------------- */

