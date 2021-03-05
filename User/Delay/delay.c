#include "delay.h"
#include "rtthread.h"
#include "stm32f10x.h"


void rt_hw_us_delay(unsigned int us)
{
	rt_uint32_t delta, current_delay, tick;
	if(us == 0)
		return;
	//�����ʱ��Ҫ������tick��
	tick = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
	//��õ�ǰʱ��
	delta = SysTick->VAL;
	
	//ѭ����õ�ǰʱ�䣬ֱ���ﵽָ��ʱ����˳�ѭ��
	do
	{
		if(delta > SysTick->VAL)
		{
			current_delay = delta - SysTick->VAL;
		}
		else
		{
			//��ʱ��Խ��һ��OS tick�ı߽�
			current_delay = SysTick->LOAD + delta - SysTick->VAL;
		}
	}while(current_delay < tick);
}



