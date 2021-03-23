/* 该文件统一用于存放中断服务函数 */
#include "stm32f10x_it.h"
#include "rtthread.h"

/* Includes ------------------------------------------------------------------*/
#include "bsp_usart.h"
#include "mcu_api.h"
#include "run.h"


// 串口中断服务函数
void APP_USART_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(APP_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(APP_USARTx);
		uart_receive_input(ucTemp);
	}	 
}




/**
  * @brief  This function handles KEY1 interrupt request.快速喂食中断
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		rt_interrupt_enter();
		
		rt_sem_release(quick_feed_sem);
		
		rt_interrupt_leave();
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}


/**
  * @brief  This function handles KEY2 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		
	}
	EXTI_ClearITPendingBit(EXTI_Line13);
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
