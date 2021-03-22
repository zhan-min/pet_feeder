#ifndef __BSP_LIGHT_H
#define __BSP_LIGHT_H


#include "stm32f10x.h"


/* ����light���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�light���� */
#define LIGHT_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define LIGHT_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LIGHT_GPIO_PIN		  GPIO_Pin_5			        /* ���ӵ�SCLʱ���ߵ�GPIO */




#define ON  0
#define OFF 1


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LIGHT_TOGGLE		 digitalToggle(LIGHT_GPIO_PORT,LIGHT_GPIO_PIN)
#define LIGHT_OFF		     digitalHi(LIGHT_GPIO_PORT,LIGHT_GPIO_PIN)
#define LIGHT_ON			   digitalLo(LIGHT_GPIO_PORT,LIGHT_GPIO_PIN)



void LIGHT_GPIO_Config(void);


#endif /* __BSP_LIGHT_H */

