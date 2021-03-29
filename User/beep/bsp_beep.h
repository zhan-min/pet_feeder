#ifndef __BEEP_H
#define	__BEEP_H

#include "stm32f10x.h"


extern uint16_t beep_counter;//Ϊ�������ṩ����
extern FunctionalState beep_count_state;
extern uint8_t meal_voice_time;//�������Ŵ���

/* ������������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶķ��������� */
#define BEEP_GPIO_PORT    	GPIOA			              /* GPIO�˿� */
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define BEEP_GPIO_PIN		  	GPIO_Pin_8			        /* ���ӵ���������GPIO */



#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬

#define BEEP_TOGGLE		 digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

/* ���κ꣬��������������һ��ʹ�� */
#define BEEP(a)	if (a)	\
					GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);\
					else		\
					GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

void BEEP_GPIO_Config(void);
void meal_voice(uint8_t n);
					
#endif /* __BEEP_H */

					
