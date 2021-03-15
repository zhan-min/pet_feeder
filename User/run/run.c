#include "run.h"
#include "stm32f10x.h"
#include "wifi.h"


/*�����ɾ������*/
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h" 
#include "bsp_adc.h"
#include "bsp_led.h"



#define MeasurementRange   3.3

/*
******************************************************************
*                               ����
******************************************************************
*/
//��Ϣ����
//rt_mq_t getwave_status_queue = RT_NULL;//�ɼ���ɱ�־

/* �����߳̿��ƿ� */
rt_thread_t wifi_uart_service_thread  = RT_NULL;



/*
*************************************************************************
*                             ��������
*************************************************************************
*/


/**
  * @brief  ִ�и������ò���
  * @param  CurSetItem����ǰ�������õĲ���
	* @param  Operation�� �Բ��������ķ���
  * @retval None
  */



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
	wifi_uart_service();
}


void Run(void)
{
	/**********������Ϣ����************/
//	getwave_status_queue = rt_mq_create("getwave_status_queue", 1, 1, RT_IPC_FLAG_FIFO);
	
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
}



/* ------------------------------------------end of file---------------------------------------- */

