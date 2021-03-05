/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/
/* STM32 �̼���ͷ�ļ� */
#include "stm32f10x.h"

/* ������Ӳ��bspͷ�ļ� */
#include "board.h" 
#include "bsp_ili9341_lcd.h"
#include "bsp_adc.h"
#include "delay.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key_exti.h"
#include "bsp_PS2.h"
#include "OSC.h"

/* RT-Thread���ͷ�ļ� */
#include <rthw.h>
#include <rtthread.h>



#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024
/* ���ڲ�SRAM�������һ���־�̬�ڴ�����Ϊrtt�Ķѿռ䣬��������Ϊ4KB */
static uint32_t rt_heap[RT_HEAP_SIZE];
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
  * @brief  ������Ӳ����ʼ������
  * @param  ��
  * @retval ��
  *
  * @attention
  * RTT�ѿ�������صĳ�ʼ������ͳһ�ŵ�board.c�ļ���ʵ�֣�
  * ��Ȼ���������Щ����ͳһ�ŵ�main.c�ļ�Ҳ�ǿ��Եġ�
  */
void rt_hw_board_init()
{
  /* ��ʼ��SysTick */
  SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );	
  
	/* Ӳ��BSP��ʼ��ͳͳ�����������LED�����ڣ�LCD�� */
  
	ILI9341_Init ();//LCD ��ʼ��
	LED_GPIO_Config();
	LED2_ON;
	USART_Config();
	ADCx_Init();
  EXTI_Key_Config();
	PS2_Key_Config();
	
	//����0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
	//���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
	//���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����  
  ILI9341_GramScan ( 3 );
	LCD_SetColors(WHITE, BLACK);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	
	//���Ʋ�����ʾ�����ͱ�������
	PlotBlackground();
	
	char dispBuff[100];
	uint8_t StartPos_X=10, StartPos_Y=7, Div=50;
	ILI9341_Clear(0, 0, 320, 30);
	
	sprintf(dispBuff,"%.1fkHz", CurWaveFrq);
	ILI9341_DispString_EN(Div*0+StartPos_X, StartPos_Y, dispBuff);	
	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(dispBuff,"%.1fV", CurTriggerValue);
	ILI9341_DispString_EN(Div*1+StartPos_X+20, StartPos_Y, dispBuff);
	ILI9341_DispString_EN(Div*2+StartPos_X+15, StartPos_Y, CurTriggerMode);	
	ILI9341_DispString_EN(Div*3+StartPos_X, StartPos_Y, CurSamplingMode);
	if(CurTimePerDiv < 1000)
	{
		sprintf(dispBuff,"%dus", CurTimePerDiv);
	}
	else
	{
		sprintf(dispBuff,"%dms", CurTimePerDiv/1000);
	}
	ILI9341_DispString_EN(Div*4+StartPos_X, StartPos_Y, dispBuff);
	ILI9341_DispString_EN(Div*5+StartPos_X, StartPos_Y, CurSamplStatus);
			
	
	
	
/* ���������ʼ������ (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
    
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

/**
  * @brief  SysTick�жϷ�����
  * @param  ��
  * @retval ��
  *
  * @attention
  * SysTick�жϷ������ڹ̼����ļ�stm32f10x_it.c��Ҳ�����ˣ�������
  * ��board.c���ֶ���һ�Σ���ô�����ʱ�������ظ�����Ĵ��󣬽��
  * �����ǿ��԰�stm32f10x_it.c�е�ע�ͻ���ɾ�����ɡ�
  */
void SysTick_Handler(void)
{
    /* �����ж� */
    rt_interrupt_enter();

    /* ����ʱ�� */
    rt_tick_increase();

    /* �뿪�ж� */
    rt_interrupt_leave();
}



/**
  * @brief  ��ӳ�䴮��DEBUG_USARTx��rt_kprintf()����
  *   Note��DEBUG_USARTx����bsp_usart.h�ж���ĺ꣬Ĭ��ʹ�ô���1
  * @param  str��Ҫ��������ڵ��ַ���
  * @retval ��
  *
  * @attention
  * 
  */
void rt_hw_console_output(const char *str)
{	
	/* �����ٽ�� */
    rt_enter_critical();

	/* ֱ���ַ������� */
    while (*str!='\0')
	{
		/* ���� */
        if (*str=='\n')
		{
			USART_SendData(DEBUG_USARTx, '\r'); 
			while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
		}

		USART_SendData(DEBUG_USARTx, *str++); 				
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);	
	}	

	/* �˳��ٽ�� */
    rt_exit_critical();
}
