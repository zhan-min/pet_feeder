#include "bsp_stepper_motor.h"
#include "delay.h"

/**
  * @brief  ��ʼ�����Ʋ��������IO
  * @param  ��
  * @retval ��
  */
void STEP_MOTOR_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*�������������ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( STEP_MOTOR_DIR_GPIO_CLK | STEP_MOTOR_PLUSE_GPIO_CLK | STEP_MOTOR_OFFLINE_GPIO_CLK, ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_DIR_GPIO_PIN;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(STEP_MOTOR_DIR_GPIO_PORT, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_PLUSE_GPIO_PIN;

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(STEP_MOTOR_PLUSE_GPIO_PORT, &GPIO_InitStructure);
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_OFFLINE_GPIO_PIN;

		/*���ÿ⺯������ʼ��GPIOF*/
		GPIO_Init(STEP_MOTOR_OFFLINE_GPIO_PORT, &GPIO_InitStructure);

		/* ʹ�ܲ������	*/
		GPIO_SetBits(STEP_MOTOR_OFFLINE_GPIO_PORT, STEP_MOTOR_OFFLINE_GPIO_PIN);
		
		/* ����ת������	*/
		GPIO_SetBits(STEP_MOTOR_DIR_GPIO_PORT, STEP_MOTOR_DIR_GPIO_PIN);
}

/**
  * @brief  ʹ�ܲ������
  * @param  ��
  * @retval ��
  */
void step_motor_enable(void)
{
	GPIO_SetBits(STEP_MOTOR_OFFLINE_GPIO_PORT, STEP_MOTOR_OFFLINE_GPIO_PIN);
}

/**
  * @brief  �ѻ��������
  * @param  ��
  * @retval ��
  */
void step_motor_offline(void)
{
	GPIO_ResetBits(STEP_MOTOR_OFFLINE_GPIO_PORT, STEP_MOTOR_OFFLINE_GPIO_PIN);
}

/**
  * @brief  ���ò����������Ϊ��ת
  * @param  ��
  * @retval ��
  */
static void set_dir_forward(void)
{
	GPIO_SetBits(STEP_MOTOR_DIR_GPIO_PORT, STEP_MOTOR_DIR_GPIO_PIN);
}

/**
  * @brief  ���ò����������Ϊ��ת
  * @param  ��
  * @retval ��
  */
static void set_dir_reverse(void)
{
	GPIO_ResetBits(STEP_MOTOR_DIR_GPIO_PORT, STEP_MOTOR_DIR_GPIO_PIN);
}

/**
  * @brief  ת��һȦ
  * @param  ��
  * @retval ��
  */
static void step_motor_rotate_1(void)
{
	for(uint8_t i=0; i<200; i++)
	{
		GPIO_SetBits(STEP_MOTOR_PLUSE_GPIO_PORT, STEP_MOTOR_PLUSE_GPIO_PIN);
		rt_hw_us_delay(2);
		GPIO_ResetBits(STEP_MOTOR_PLUSE_GPIO_PORT, STEP_MOTOR_PLUSE_GPIO_PIN);
		rt_hw_us_delay(2);
	}
}


/**
  * @brief  ��תnȦ
	* @param  n:ת��Ȧ��
  * @retval ��
  */
void step_motor_forward(uint8_t n)
{
	step_motor_enable();
	set_dir_forward();
	for(uint8_t i=0; i<n; i++)
	{
		step_motor_rotate_1();
	}	
}

/**
  * @brief  ��תnȦ
	* @param  n:ת��Ȧ��
  * @retval ��
  */
void step_motor_reverse(uint8_t n)
{
	step_motor_enable();
	set_dir_reverse();
	for(uint8_t i=0; i<n; i++)
	{
		step_motor_rotate_1();
	}	
}
	


