#include "run.h"
#include "stm32f10x.h"
#include "wifi.h"
#include "bsp_stepper_motor.h"
#include "bsp_key_exti.h"
#include "bsp_hx711_granary.h"
#include "bsp_hx711_export.h"


/*�����ɾ������*/
#include "bsp_usart.h" 
#include "bsp_adc.h"
#include "bsp_led.h"




/*
******************************************************************
*                               ����
******************************************************************
*/

//ȫ�ֱ���
uint32_t granary_peel = 0;
uint32_t export_peel  = 0;


//��Ϣ����
//rt_mq_t getwave_status_queue = RT_NULL;//�ɼ���ɱ�־

//�ź������ƿ�
static rt_sem_t quick_feed_sem = RT_NULL;

/* �����߳̿��ƿ� */
static rt_thread_t granary_weight_thread = NULL;
static rt_thread_t export_weight_thread = NULL;
static rt_thread_t key1_scan_thread  = RT_NULL;
static rt_thread_t wifi_uart_service_thread  = RT_NULL;
static rt_thread_t quick_feed_thread  = RT_NULL;



/*
*************************************************************************
*                             ��������
*************************************************************************
*/





/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/

/**
 * @brief  ��Ͱ�����������
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
 * @brief  �ѳ���ʣ���������
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
 * @brief  ����1ɨ��
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
			// ���ּ��
			while( GPIO_ReadInputDataBit(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == KEY_ON );
			rt_sem_release(quick_feed_sem);
		}
	}	
}


/**
 * @brief  wifi�������ݴ������
 * @param  Null
 * @return Null
 * @note   ��ͿѻSDK�ṩ
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
  * @brief  ����ιʳ
  * @param  ��
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
	/**********������Ϣ����************/
//	getwave_status_queue = rt_mq_create("getwave_status_queue", 1, 1, RT_IPC_FLAG_FIFO);
	
	/**********�����ź���************/
	quick_feed_sem = rt_sem_create("quick_feed_n",     /* �ź������� */
																	0,                /* ��ʼֵ */
																	RT_IPC_FLAG_PRIO); /* �ź���ģʽ */
	
	/**********�����߳�************/
	
//	granary_weight_thread =                                   /* �߳̿��ƿ�ָ�� */
//							rt_thread_create( "granary_weight",           /* �߳����� */
//																granary_weight,             /* �߳���ں��� */
//																RT_NULL,               /* �߳���ں������� */
//																512,                   /* �߳�ջ��С */
//																6,                     /* �̵߳����ȼ� */
//																20);                   /* �߳�ʱ��Ƭ */
//	
//	if (granary_weight_thread != RT_NULL) 
//        rt_thread_startup(granary_weight_thread);
	
	export_weight_thread =                                   /* �߳̿��ƿ�ָ�� */
							rt_thread_create( "export_weight",           /* �߳����� */
																export_weight,             /* �߳���ں��� */
																RT_NULL,               /* �߳���ں������� */
																512,                   /* �߳�ջ��С */
																6,                     /* �̵߳����ȼ� */
																20);                   /* �߳�ʱ��Ƭ */
	
	if (export_weight_thread != RT_NULL) 
        rt_thread_startup(export_weight_thread);
	
	key1_scan_thread =                                   /* �߳̿��ƿ�ָ�� */
							rt_thread_create( "key1_scan",           /* �߳����� */
																key1_scan,             /* �߳���ں��� */
																RT_NULL,               /* �߳���ں������� */
																512,                   /* �߳�ջ��С */
																6,                     /* �̵߳����ȼ� */
																20);                   /* �߳�ʱ��Ƭ */
	
	if (key1_scan_thread != RT_NULL) 
        rt_thread_startup(key1_scan_thread);
	
	
	wifi_uart_service_thread =                           /* �߳̿��ƿ�ָ�� */
							rt_thread_create( "wifi_uart_service",   /* �߳����� */
																tuya_wifi_uart_service,/* �߳���ں��� */
																RT_NULL,               /* �߳���ں������� */
																512,                   /* �߳�ջ��С */
																5,                     /* �̵߳����ȼ� */
																20);                   /* �߳�ʱ��Ƭ */
   if (wifi_uart_service_thread != RT_NULL) 
        rt_thread_startup(wifi_uart_service_thread);
	 
	 
	 quick_feed_thread =                                 /* �߳̿��ƿ�ָ�� */
							rt_thread_create( "quick_feed",          /* �߳����� */
																quick_feed,            /* �߳���ں��� */
																RT_NULL,               /* �߳���ں������� */
																512,                   /* �߳�ջ��С */
																4,                     /* �̵߳����ȼ� */
																20);                   /* �߳�ʱ��Ƭ */
   if (quick_feed_thread != RT_NULL) 
        rt_thread_startup(quick_feed_thread);
}



/* ------------------------------------------end of file---------------------------------------- */

