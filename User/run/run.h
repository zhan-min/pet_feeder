#ifndef __OSC_H
#define __OSC_H

#include "rtthread.h"
#include "stm32f10x_it.h"


//ȫ�ֱ���
extern uint16_t granary_peel;
extern uint16_t export_peel;

//��Ϣ����
//extern rt_mq_t setting_data_queue;
//extern rt_mq_t getwave_status_queue;


//�ź���
static rt_sem_t quick_feed_sem;

//�߳�
//extern rt_thread_t GetWave_thread;


void Run(void);

#endif /* __OSC_H */

