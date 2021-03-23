#include "bsp_hx711_granary.h"
#include "rtthread.h"



 /**
  * @brief  初始化控制hx711的IO
  * @param  无
  * @retval 无
  */
void HX711_GRANARY_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启hx711相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( HX711_GRANARY_DOUT_GPIO_CLK | HX711_GRANARY_SCK_GPIO_CLK, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = HX711_GRANARY_DOUT_GPIO_PIN;	

		/*设置引脚模式为浮空输入*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(HX711_GRANARY_DOUT_GPIO_PORT, &GPIO_InitStructure);	
		
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = HX711_GRANARY_SCK_GPIO_PIN;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(HX711_GRANARY_SCK_GPIO_PORT, &GPIO_InitStructure);
}



static void set_hx711_sck(void)
{
	GPIO_SetBits(HX711_GRANARY_SCK_GPIO_PORT, HX711_GRANARY_SCK_GPIO_PIN);
}

static void reset_hx711_sck(void)
{
	GPIO_ResetBits(HX711_GRANARY_SCK_GPIO_PORT, HX711_GRANARY_SCK_GPIO_PIN);
}

static void set_hx711_dout(void)
{
	GPIO_SetBits(HX711_GRANARY_DOUT_GPIO_PORT, HX711_GRANARY_DOUT_GPIO_PIN);
}

//static void reset_hx711_dout(void)
//{
//	GPIO_ResetBits(HX711_GRANARY_DOUT_GPIO_PORT, HX711_GRANARY_DOUT_GPIO_PIN);
//}

static uint8_t read_dout(void)
{
	return GPIO_ReadInputDataBit(HX711_GRANARY_DOUT_GPIO_PORT, HX711_GRANARY_DOUT_GPIO_PIN);
}

//****************************************************
//读取HX711
//****************************************************
uint16_t hx711_granary_read(void)	//增益128
{
	uint32_t count;
	uint16_t granary_weight;
	uint8_t i; 
	set_hx711_dout(); 
	rt_thread_delay(5);
	reset_hx711_sck();
	count=0;
	while(read_dout());
	for(i=0;i<24;i++)
	{ 
		set_hx711_sck(); 
		count=count<<1; 
		reset_hx711_sck(); 
		if(read_dout())
			count++; 
	} 
 	set_hx711_sck(); 
  count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	rt_thread_delay(5);
	reset_hx711_sck(); 
	granary_weight = (unsigned long)((float)count/gapvalue_granary);
	return(granary_weight);
}

