#include "bsp_beep.h"
#include "rtthread.h"

uint16_t beep_counter = 0;//为扬声器提供节拍
FunctionalState beep_count_state = ENABLE;
uint8_t meal_voice_time = 1;//语音播放次数


const uint8_t array[]=
{
             0x18, 0x30, 0x1C, 0x10,
             0x20, 0x40, 0x1C, 0x10, 
             0x18, 0x10, 0x20, 0x10,
             0x1C, 0x10, 0x18, 0x40,
             0x1C, 0x20, 0x20, 0x20,
             0x1C, 0x20, 0x18, 0x20, 
             0x20, 0x80, 0xFF, 0x20,
             0x30, 0x1C, 0x10, 0x18,
             0x20, 0x15, 0x20, 0x1C,
             0x20, 0x20, 0x20, 0x26,
             0x40, 0x20, 0x20, 0x2B,
             0x20, 0x26, 0x20, 0x20, 
             0x20, 0x30, 0x80, 0xFF,
             0x20, 0x20, 0x1C, 0x10,
             0x18, 0x10, 0x20, 0x20,
             0x26, 0x20, 0x2B, 0x20,
             0x30, 0x20, 0x2B, 0x40,
             0x20, 0x20, 0x1C, 0x10,
             0x18, 0x10, 0x20, 0x20,
             0x26, 0x20, 0x2B, 0x20,
             0x30, 0x20, 0x2B, 0x40,
             0x20, 0x30, 0x1C, 0x10,
             0x18, 0x20, 0x15, 0x20,
             0x1C, 0x20, 0x20, 0x20,
             0x26, 0x40, 0x20, 0x20, 
             0x2B, 0x20, 0x26, 0x20, 
             0x20, 0x20, 0x30, 0x80,
             0x20, 0x30, 0x1C, 0x10,
             0x20, 0x10, 0x1C, 0x10,
             0x20, 0x20, 0x26, 0x20,
             0x2B, 0x20, 0x30, 0x20,
             0x2B, 0x40, 0x20, 0x15,
             0x1F, 0x05, 0x20, 0x10,
             0x1C, 0x10, 0x20, 0x20,
             0x26, 0x20, 0x2B, 0x20,
             0x30, 0x20, 0x2B, 0x40,
             0x20, 0x30, 0x1C, 0x10,
             0x18, 0x20, 0x15, 0x20,
             0x1C, 0x20, 0x20, 0x20,
             0x26, 0x40, 0x20, 0x20,
             0x2B, 0x20, 0x26, 0x20,
             0x20, 0x20, 0x30, 0x30, 
             0x20, 0x30, 0x1C, 0x10,
             0x18, 0x40, 0x1C, 0x20,
             0x20, 0x20, 0x26, 0x40,
             0x13, 0x60, 0x18, 0x20,
             0x15, 0x40, 0x13, 0x40,
             0x18, 0x80, 0x00
};

 /**
  * @brief  初始化控制蜂鸣器的IO
  * @param  无
  * @retval 无
  */
void BEEP_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启控制蜂鸣器的GPIO的端口时钟*/
		RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK, ENABLE); 

		/*选择要控制蜂鸣器的GPIO*/															   
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;	

		/*设置GPIO模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置GPIO速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化控制蜂鸣器的GPIO*/
		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			 
    
    /* 关闭蜂鸣器*/
		GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);	 
}



/**
  * @brief  喂食提醒音乐播放
	* @param  n: 播放次数
  * @retval None
  */
void meal_voice(uint8_t n)
{
	uint16_t i;
	uint8_t sound_signal;//定义音符大小 
	uint8_t sound_pace;//定义节拍大小

	//array[i]=0x00 代表歌曲演唱完毕
	//array[i]=0xff 代表是休止符
	while(n--)
	{
		i=0;
		while(array[i]!=0x00)
		{
			//如果是休止符，延时100ms，并终止本次循环，进入下一个循环 
			if(array[i]==0xff)
			{
			 beep_count_state = DISABLE;
			 i++;
			 rt_thread_mdelay(100);
			 continue;
			}
			//从表中取得 音符大小 
			sound_signal=array[i++];
			//从表中取得 节拍大小 
			sound_pace=array[i];
			beep_count_state = ENABLE;
			//当节拍数未达到时候，继续循环，产生该音调的声音 
			while(beep_counter!=sound_pace)
			{ 
				BEEP_TOGGLE;
				rt_thread_delay(16*sound_signal);
			}
			i++;
			beep_counter=0;//节拍计数器置0，进入下一个音调 
		}
		rt_thread_mdelay(10); //歌曲演唱完毕后,延时一段时间
	}
}




