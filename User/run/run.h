#ifndef __OSC_H
#define __OSC_H

#include "rtthread.h"
#include "stm32f10x_it.h"


//全局变量
struct date_time 
{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t week;
};

extern struct date_time time_now;
extern uint16_t granary_peel;
extern uint16_t export_peel;

//消息队列
//extern rt_mq_t setting_data_queue;
//extern rt_mq_t getwave_status_queue;


//信号量
static rt_sem_t quick_feed_sem;

//线程
//extern rt_thread_t GetWave_thread;


void Run(void);

#endif /* __OSC_H */

