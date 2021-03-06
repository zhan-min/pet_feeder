#include "run.h"
#include "stm32f10x.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h" 
#include "bsp_adc.h"
#include "bsp_led.h"



#define MeasurementRange   3.3

/*
******************************************************************
*                               ����
******************************************************************
*/
//��Ϣ����
//rt_mq_t getwave_status_queue = RT_NULL;//�ɼ���ɱ�־

/* �����߳̿��ƿ� */
//rt_thread_t GetWave_thread  = RT_NULL;


//��������
char*    SamplStatus[] = {"Stop", "Run"};
char*    TriggerMode[] = {"Up", "Down"};
char*    SamplingMode[] = {"Auto", "Normal", "Single"};
uint32_t TimePerDiv_Group[] = {50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000};


int8_t   SamplStatusNrb =1;
int8_t   TriggerModeNrb = 0;
int8_t   SamplingModeNrb =1;
uint8_t  TimePerDivOderNbr = sizeof(TimePerDiv_Group)/sizeof(TimePerDiv_Group[0]);
int8_t   TimePerDivOder = 4;//��ǰÿ����ʱ������


char*     CurSamplStatus = {"Run"};   //����5������״̬��0��ֹͣ������1�����ڲ����������жϷ�ʽ����
float     CurTriggerValue = 2.0;      //����1��������ֵ
char*     CurTriggerMode = {"Up"};    //����2������ģʽ��0���½��ش�����1�������ش���
char*     CurSamplingMode = {"Normal"}; //����3������ģʽ��0���Զ���1����ͨ��2������
uint32_t  CurTimePerDiv = 1000;        //����4��ÿ������ʱ����

//Ҫ��ʾ����Ϣ
double     CurWaveFrq = 0.0;           //����0������Ƶ�ʣ���λkHz
__IO  uint16_t    ADC_ConvertedValue[ADCx_1_SampleNbr] = {0};//ADC�ɼ�����


//ȫ�ֱ���
uint8_t  WaveLenthSumNrb=0;//���������ۼӴ���
uint16_t WaveLenth=0;//����
FlagStatus StopSample = RESET;

/*
*************************************************************************
*                             ��������
*************************************************************************
*/


/**
  * @brief  ִ�и������ò���
  * @param  CurSetItem����ǰ�������õĲ���
	* @param  Operation�� �Բ��������ķ���
  * @retval None
  */



/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/
/**
  * @brief  ִ�и������ò���
  * @param  CurSetItem����ǰ�������õĲ���
	* @param  Operation�� �Բ��������ķ���
  * @retval None
  */
//void PlotWave(void* parameter)
//{
//	
//}



void Run(void)
{
	/**********������Ϣ����************/
//	getwave_status_queue = rt_mq_create("getwave_status_queue", 1, 1, RT_IPC_FLAG_FIFO);
	
	/**********�����߳�************/
//	GetWave_thread =                         /* �߳̿��ƿ�ָ�� */
//    rt_thread_create( "GetWave",           /* �߳����� */
//                      Get_Wave,       		 /* �߳���ں��� */
//                      RT_NULL,             /* �߳���ں������� */
//                      512,                 /* �߳�ջ��С */
//                      3,                   /* �̵߳����ȼ� */
//                      20);                 /* �߳�ʱ��Ƭ */
//   if (GetWave_thread != RT_NULL) 
//        rt_thread_startup(GetWave_thread);
}



/* ------------------------------------------end of file---------------------------------------- */

