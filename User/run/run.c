#include "run.h"
#include "stm32f10x.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h" 
#include "bsp_adc.h"
#include "bsp_led.h"



#define MeasurementRange   3.3

/*
******************************************************************
*                               变量
******************************************************************
*/
//消息队列
//rt_mq_t getwave_status_queue = RT_NULL;//采集完成标志

/* 定义线程控制块 */
//rt_thread_t GetWave_thread  = RT_NULL;


//可设置项
char*    SamplStatus[] = {"Stop", "Run"};
char*    TriggerMode[] = {"Up", "Down"};
char*    SamplingMode[] = {"Auto", "Normal", "Single"};
uint32_t TimePerDiv_Group[] = {50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000};


int8_t   SamplStatusNrb =1;
int8_t   TriggerModeNrb = 0;
int8_t   SamplingModeNrb =1;
uint8_t  TimePerDivOderNbr = sizeof(TimePerDiv_Group)/sizeof(TimePerDiv_Group[0]);
int8_t   TimePerDivOder = 4;//当前每格间隔时间的序号


char*     CurSamplStatus = {"Run"};   //代号5，采样状态，0：停止采样，1：正在采样，采用中断方式设置
float     CurTriggerValue = 2.0;      //代号1，触发阀值
char*     CurTriggerMode = {"Up"};    //代号2，触发模式，0：下降沿触发，1：上升沿触发
char*     CurSamplingMode = {"Normal"}; //代号3，采样模式，0：自动，1：普通，2：单次
uint32_t  CurTimePerDiv = 1000;        //代号4，每格代表的时间间隔

//要显示的信息
double     CurWaveFrq = 0.0;           //代号0，波形频率，单位kHz
__IO  uint16_t    ADC_ConvertedValue[ADCx_1_SampleNbr] = {0};//ADC采集数据


//全局变量
uint8_t  WaveLenthSumNrb=0;//波长计算累加次数
uint16_t WaveLenth=0;//波长
FlagStatus StopSample = RESET;

/*
*************************************************************************
*                             辅助函数
*************************************************************************
*/


/**
  * @brief  执行更改设置操作
  * @param  CurSetItem：当前正在设置的参数
	* @param  Operation： 对参数调整的方向
  * @retval None
  */



/*
*************************************************************************
*                             线程定义
*************************************************************************
*/
/**
  * @brief  执行更改设置操作
  * @param  CurSetItem：当前正在设置的参数
	* @param  Operation： 对参数调整的方向
  * @retval None
  */
//void PlotWave(void* parameter)
//{
//	
//}



void Run(void)
{
	/**********创建消息队列************/
//	getwave_status_queue = rt_mq_create("getwave_status_queue", 1, 1, RT_IPC_FLAG_FIFO);
	
	/**********创建线程************/
//	GetWave_thread =                         /* 线程控制块指针 */
//    rt_thread_create( "GetWave",           /* 线程名字 */
//                      Get_Wave,       		 /* 线程入口函数 */
//                      RT_NULL,             /* 线程入口函数参数 */
//                      512,                 /* 线程栈大小 */
//                      3,                   /* 线程的优先级 */
//                      20);                 /* 线程时间片 */
//   if (GetWave_thread != RT_NULL) 
//        rt_thread_startup(GetWave_thread);
}



/* ------------------------------------------end of file---------------------------------------- */

