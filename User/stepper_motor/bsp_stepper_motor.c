#include "bsp_stepper_motor.h"
#include "delay.h"

/**
  * @brief  初始化控制步进电机的IO
  * @param  无
  * @retval 无
  */
void STEP_MOTOR_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启步进电机相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( STEP_MOTOR_DIR_GPIO_CLK | STEP_MOTOR_PLUSE_GPIO_CLK | STEP_MOTOR_OFFLINE_GPIO_CLK, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_DIR_GPIO_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(STEP_MOTOR_DIR_GPIO_PORT, &GPIO_InitStructure);	
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_PLUSE_GPIO_PIN;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(STEP_MOTOR_PLUSE_GPIO_PORT, &GPIO_InitStructure);
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_OFFLINE_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(STEP_MOTOR_OFFLINE_GPIO_PORT, &GPIO_InitStructure);

		/* 使能步进电机	*/
		GPIO_SetBits(STEP_MOTOR_OFFLINE_GPIO_PORT, STEP_MOTOR_OFFLINE_GPIO_PIN);
		
		/* 设置转动方向	*/
		GPIO_SetBits(STEP_MOTOR_DIR_GPIO_PORT, STEP_MOTOR_DIR_GPIO_PIN);
}

/**
  * @brief  使能步进电机
  * @param  无
  * @retval 无
  */
void step_motor_enable(void)
{
	GPIO_SetBits(STEP_MOTOR_OFFLINE_GPIO_PORT, STEP_MOTOR_OFFLINE_GPIO_PIN);
}

/**
  * @brief  脱机步进电机
  * @param  无
  * @retval 无
  */
void step_motor_offline(void)
{
	GPIO_ResetBits(STEP_MOTOR_OFFLINE_GPIO_PORT, STEP_MOTOR_OFFLINE_GPIO_PIN);
}

/**
  * @brief  设置步进电机方向为正转
  * @param  无
  * @retval 无
  */
static void set_dir_forward(void)
{
	GPIO_SetBits(STEP_MOTOR_DIR_GPIO_PORT, STEP_MOTOR_DIR_GPIO_PIN);
}

/**
  * @brief  设置步进电机方向为反转
  * @param  无
  * @retval 无
  */
static void set_dir_reverse(void)
{
	GPIO_ResetBits(STEP_MOTOR_DIR_GPIO_PORT, STEP_MOTOR_DIR_GPIO_PIN);
}

/**
  * @brief  转动一圈
  * @param  无
  * @retval 无
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
  * @brief  正转n圈
	* @param  n:转动圈数
  * @retval 无
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
  * @brief  反转n圈
	* @param  n:转动圈数
  * @retval 无
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
	


