/* ���ļ�ͳһ���ڴ���жϷ����� */
#include "stm32f10x_it.h"
#include "rtthread.h"

/* Includes ------------------------------------------------------------------*/




/**
  * @brief  SW���жϴ���������SW��������״̬��
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
		
}



/**
  * @brief  This function handles KEY1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		rt_interrupt_enter();
				
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
