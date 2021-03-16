#include "run.h"
#include "stm32f10x.h"
#include "wifi.h"
#include "bsp_stepper_motor.h"


/*�����ɾ������*/
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h" 
#include "bsp_adc.h"
#include "bsp_led.h"




/*
******************************************************************
*                               ����
******************************************************************
*/

//ȫ�ֱ���
FunctionalState quick_feed_enable = ENABLE;


//��Ϣ����
//rt_mq_t getwave_status_queue = RT_NULL;//�ɼ���ɱ�־

//�ź������ƿ�
static rt_sem_t quick_feed_sem = RT_NULL;

/* �����߳̿��ƿ� */
static rt_thread_t wifi_uart_service_thread  = RT_NULL;
static rt_thread_t quick_feed_thread  = RT_NULL;



/*
*************************************************************************
*                             ��������
*************************************************************************
*/


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
		if( quick_feed_enable == ENABLE)
		{
			feed(1);
		}		
	}	
}


/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/
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
		wifi_uart_service();
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

