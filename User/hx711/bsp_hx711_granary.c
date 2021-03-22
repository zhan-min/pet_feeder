#include "bsp_hx711_granary.h"



 /**
  * @brief  ��ʼ������hx711��IO
  * @param  ��
  * @retval ��
  */
void HX711_GRANARY_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����hx711��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( HX711_GRANARY_DOUT_GPIO_CLK | HX711_GRANARY_SCK_GPIO_CLK, ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = HX711_GRANARY_DOUT_GPIO_PIN;	

		/*��������ģʽΪ��������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(HX711_GRANARY_DOUT_GPIO_PORT, &GPIO_InitStructure);	
		
		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = HX711_GRANARY_SCK_GPIO_PIN;

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(HX711_GRANARY_SCK_GPIO_PORT, &GPIO_InitStructure);
}


//****************************************************
//��ʱ����
//****************************************************
static void Delay__hx711_us(void)
{
	uint8_t i=2;
	while(i--);
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
//��ȡHX711
//****************************************************
uint32_t hx711_granary_read(void)	//����128
{
	uint32_t count; 
	uint8_t i; 
	set_hx711_dout(); 
	Delay__hx711_us();
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
  count=count^0x800000;//��25�������½�����ʱ��ת������
	Delay__hx711_us();
	reset_hx711_sck(); 
	return(count);
}

