#include "main.h"
#include "cmsis_os.h"

#include "led.h"
#include "usart.h"
#include "rtc.h"
#include "ospi_w25q64.h"
#include "sdmmc_sd.h"
#include "lcd_spi_096.h"

#include "memory_test.h"  
#include "lcd_test.h"  			// LCD�������
#include "camera_test.h"  		// ����ͷ�������

#include "iic.h"
#include "timer.h"
#include "adc.h"

#include "syn6288.h"
float tem,light; 
/*************************************** RTOS�������� ***************************************************/

const osThreadAttr_t Main_Task_Attr = 	// ���������ȼ����
{
	.priority = osPriorityHigh5,
	.stack_size = 8196,
};

const osThreadAttr_t Msg_Task_Attr = 	// ��Ϣ��������
{
	.priority = osPriorityLow2,
	.stack_size = 4096,
};

const osThreadAttr_t LED_Task_Attr = 	// �������
{
	.priority = osPriorityLow1,
	.stack_size = 512,
};

osThreadId_t Main_Task_Id 	= NULL;	// ������ID
osThreadId_t Msg_Task_Id 	= NULL;	// ��Ϣ��������ID
osThreadId_t LED_Task_Id 	= NULL;	// �������ID

/********************************************** �������� *******************************************/

extern RTC_HandleTypeDef hrtc;	// RTC���

RTC_TimeTypeDef Time_Struct;		// ����
RTC_DateTypeDef Date_Struct;		// ʱ��

uint16_t show_flag = 0;
/********************************************** �������� *******************************************/
void SystemClock_Config(void);		// ʱ�ӳ�ʼ��
void MPU_Config(void);					// MPU����

void RTOS_Main_Task(void *argument);	// ������ ����ͷ+��Ļ
void RTOS_Msg_Task(void *argument);		// ��Ϣ��������
void RTOS_LED_Task(void *argument);		// ��Դ��ѹ�ɼ�+�¶Ȳɼ�


/***************************************************************************************************
*	�� �� ��: main
*	
*	��������: ��ʼ�������ϵͳ
*	
****************************************************************************************************/

int main(void)
{
	MPU_Config();				// MPU����
	SCB_EnableICache();		// ʹ��ICache
	SCB_EnableDCache();		// ʹ��DCache
	HAL_Init();					// ��ʼ��HAL��	
	SystemClock_Config();	// ����ϵͳʱ�ӣ���Ƶ480MHz
	LED_Init();					// ��ʼ��LED����
	USART1_Init();				// USART1��ʼ��	115200
	
	USART2_Init();				// USART2��ʼ��	115200
	//osDelay(2000);
	MX_RTC_Init();				// RTC��ʼ��
	OSPI_W25Qxx_Init();		// W25Qxx��ʼ��
	
	TIM3_PWM_Init(500-1,240-1); //200M/200=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ500����ôPWMƵ��Ϊ1M/500=2kHZ
  MX_ADC1_Init();
	
	SD_Status = BSP_SD_Init(SD_Instance);	// ��ʼ��SD���� SD_Status ��־λ������ SD_Test() �����ж�
		
	SMBus_Init();		//��ʼ��GY906
	
	osKernelInitialize();	// ��ʼ��RTOS
	PY_usDelayTest();
	PY_usDelayOptimize();
	Main_Task_Id = osThreadNew(RTOS_Main_Task, 	NULL, &Main_Task_Attr);  	// ����������
	Msg_Task_Id  = osThreadNew(RTOS_Msg_Task, 	NULL, &Msg_Task_Attr);  	//	��Ϣ��������	
	LED_Task_Id  = osThreadNew(RTOS_LED_Task, 	NULL, &LED_Task_Attr);  	//	LED����

	osKernelStart();	// ����ϵͳ


	while (1)
	{

	}

}


/***************************************************************************************************
*	�� �� ��: RTOS_Main_Task
*
*	��������: ���������ȼ����
*
*	˵    ��: ��
****************************************************************************************************/

void RTOS_Main_Task(void *argument)
{
	(void)argument;
	
	LCD_identification();		// ��Ļ���ʶ��
	
	SPI_LCD_Init();     			// Һ�����Լ�SPI��ʼ�� 

//	if( SD_Test()!=BSP_ERROR_NONE )	// ����ʧ��
//	{
//		LCD_Printf(	"SD Error!!!! ",1);
//	}
//	
//	if( OSPI_W25Qxx_Test()!=OSPI_W25Qxx_OK )	// ����ʧ��
//	{
//		LCD_Printf(	"W25Q64  Error!!!! ",1);
//	
	
	LCD_SetDirection(Direction_V_Flip);
	
	Camera_Init();				// ��ʼ��OV2640����5640
	
	LCD_Progress(1);			// ������Ч������ʱԼΪ 220 * x ms, ͬʱҲ��ʾ RTC ʱ��
	
	
	int x=19,y=21,i=50;
	int x1=219,x11=x1-50,i1=50;
	int y1=259,y11=y1+50;

	int flag_back=0;
	while(1)
	{	
		if(flag_back==0)
		{
			i-=2;
			x++;
			x1--;
			x11++;
			y11--;
			y1++;
			i1-=2;
			y++;			
		}
		else
		{
			i+=2;
			x--;
			x1++;
			y1--;
			x11--;
			y11++;
			i1+=2;
			y--;	
		}
		if(y==31)
		{
			flag_back=1;
		}
		if(y==21)
		{
			flag_back=0;
		}		
		LCD_SetColor( LCD_YELLOW);
		Camera_Work();	// ��������ͷ
		
		
//		LCD_DisplayString(0,0,"L:");
//		LCD_DisplayDecimals(20,0,(double)light,5,2);
		LCD_DisplayString(70,0,"Tem:");
		LCD_DisplayDecimals(110,0,(double)tem+3,5,2);
		
		//��̬catch��
		LCD_DrawLine_V(x,y,i);
		LCD_DrawLine_H(x,y,i);
		
		LCD_DrawLine_V(x1,y,i);
		LCD_DrawLine_H(x11,y,i1);
		
		LCD_DrawLine_V(x,y1,i);
		LCD_DrawLine_H(x,y11,i1);
		
		LCD_DrawLine_V(x1,y1,i);
		LCD_DrawLine_H(x11,y11,i1);
		osDelay(30);
	}
}

/***************************************************************************************************
*	�� �� ��: RTOS_Msg_Task
*
*	��������: ��Ϣ��������
*
*	˵    ��: ��
****************************************************************************************************/

void RTOS_Msg_Task(void *argument)
{
	(void)argument;

//	printf("RTOS ͨ������\r\n");
	
	while(1)
	{
		if(tem+3 >= 38){
			show_flag = 1;
			SYN_FrameInfo(0, "[v15][m1][t5]�����쳣");
//			osDelay(1500);
//				show_flag = 0;
		}
//		HAL_RTC_GetTime(&hrtc,&Time_Struct,RTC_FORMAT_BIN);	// ��ȡʱ��

	//		STM32��RTCΪȷ���� ���롢ʱ�䡢���� 3��ֵ����ͬһʱ�̵㣬
	//		��ȡ ���루RTC_SSR ���� ʱ�䣨RTC_TR ��ʱ�������߽�Ӱ�ӼĴ����е�ֵ��ֱ����ȡRTC_DR��	
	//		Ҳ����˵�û��������ڵĻ���ʱ��ֵ�ᱻ����	
		//HAL_RTC_GetDate(&hrtc,&Date_Struct,RTC_FORMAT_BIN);	// ��ȡ����
//		printf("%d-%d-%d ",Date_Struct.Year+2000,Date_Struct.Month,Date_Struct.Date);
//		printf("%d:%d:%d\r\n",Time_Struct.Hours,Time_Struct.Minutes,Time_Struct.Seconds);
		
//		SYN_FrameInfo(0, "[v15][m1][t5]ʶ��ɹ�");
		osDelay(1500);
				show_flag = 0;
	}
}

/***************************************************************************************************
*	�� �� ��: RTOS_LED_Task
*
*	��������: ��ص������
*
*	˵    ��: ��
****************************************************************************************************/

void RTOS_LED_Task(void *argument)
{
	(void)argument;
	while(1)
	{
//		LED1_Toggle;	// LED1 ״̬��ת
		tem = SMBus_ReadTemp();
//		printf("�¶ȣ�%.2f\r\n",tem);

		light = get_adc_data();
//		printf("ADCֵ��%.2f\r\n",V);
		
		//LED����
		if(light >= 35)
			TIM_SetTIM3Compare4(500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		else if(light <= 15)
			TIM_SetTIM3Compare4(0);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		else
			TIM_SetTIM3Compare4((light-23)*25);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//		printf("%.2f",tem);
		//���Լ��
//		if(tem >= 28 && show_flag == 0){
//			USART2Printf("A");
//			show_flag = 1;
//		}
		osDelay(100);
	}
}

/****************************************************************************************************/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 1 (AHB  Clock  280MHz)
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 112
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
  
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  
  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
	  
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);	  	  
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;  
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 112;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  
  	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI4|RCC_PERIPHCLK_OSPI|RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_SDMMC;				
  
/***************************************** SPI4�ں�ʱ������ **************************************************/
/*** FANKE ***
>>>>> ����˵����
	*
	*	1.	SPI4 ��������������ʱ��Ϊ75M��������Բ��ģ�7B0�����ֲ� ��6.3.36.3С��  SPI interface characteristics
   *
	*	2. Ϊ�˷������ú��û���ֲ�����ｫ2��Ƶ��� pclk��140M�� ��Ϊ SPI4 ���ں�ʱ�ӣ�Ȼ����SPI������������Ϊ2��Ƶ�õ� 70M ��SCK����ʱ��
   *
   *	3. ��Ȼ��Ļ������ST7789��������������ʱ��Ϊ62.5M����ʵ�ʲ����У���ʹ����Ϊ70MҲ�ǳ��ȶ������û�
  *		��SPIʱ�����ϸ�Ҫ�󣬿����޸�SPI�ں�ʱ����ʵ��
   *
*** FANKE ***/  
 
	PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;	
  
	/* Ϊ�����÷��㣬�� ϵͳ��ʱ��HCLK ��ΪOSPI���ں�ʱ�ӣ��ٶ�Ϊ280M���پ���2��Ƶ�õ�140M����ʱ��  */  
	/* HCLK Ҳ��OSPI��Ĭ�ϵ��ں�ʱ�ӣ���ʹ��XIPʱ����OSPI�������д��룩��Ҳֻ��ʹ��Ĭ�ϵ�ʱ�� */
	/* ��ˣ���Ȼ140M�Ѿ������� W25Q64JV ����������ʱ�ӣ������ǽ�OSPI������ʱ������Ϊ140M */
	/* ʵ�ʲ����У�������ʱ����������200M��w25q64���ǿ���������д������ֻ����7M��Ƶ�ʲ��õ����ȶ���*/
 	PeriphClkInitStruct.OspiClockSelection = RCC_OSPICLKSOURCE_D1HCLK; //ʹ�� ϵͳ��ʱ��HCLK ��ΪOSPI���ں�ʱ�ӣ��ٶ�Ϊ280M���پ���2��Ƶ�õ�140M����ʱ��  
  
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
	
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}    
  
}

//	����MPU
//
void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	HAL_MPU_Disable();		// �Ƚ�ֹMPU

	MPU_InitStruct.Enable 				= MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress 		= 0x24000000;
	MPU_InitStruct.Size 					= MPU_REGION_SIZE_512KB;
	MPU_InitStruct.AccessPermission 	= MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable 		= MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable 		= MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable 		= MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number 				= MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField 		= MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable 	= 0x00;
	MPU_InitStruct.DisableExec 		= MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);	

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);	// ʹ��MPU
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


