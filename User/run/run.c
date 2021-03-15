#include "run.h"
#include "stm32f10x.h"
#include "wifi.h"


/*看情况删除或保留*/
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h" 
#include "bsp_adc.h"
#include "bsp_led.h"



#define MeasurementRange   3.3

/*
******************************************************************
*                               变量
******************************************************************
*/
//消息队列
//rt_mq_t getwave_status_queue = RT_NULL;//采集完成标志

/* 定义线程控制块 */
rt_thread_t wifi_uart_service_thread  = RT_NULL;



/*
*************************************************************************
*                             辅助函数
*************************************************************************
*/


/**
  * @brief  执行更改设置操作
  * @param  CurSetItem：当前正在设置的参数
	* @param  Operation： 对参数调整的方向
  * @retval None
  */



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
	wifi_uart_service();
}


void Run(void)
{
	/**********创建消息队列************/
//	getwave_status_queue = rt_mq_create("getwave_status_queue", 1, 1, RT_IPC_FLAG_FIFO);
	
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
}



/* ------------------------------------------end of file---------------------------------------- */

