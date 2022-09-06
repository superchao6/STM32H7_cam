#include "main.h"
#include "cmsis_os.h"

#include "led.h"
#include "usart.h"
#include "rtc.h"
#include "ospi_w25q64.h"
#include "sdmmc_sd.h"
#include "lcd_spi_096.h"

#include "memory_test.h"  
#include "lcd_test.h"  			// LCD测试相关
#include "camera_test.h"  		// 摄像头测试相关

#include "iic.h"
#include "timer.h"
#include "adc.h"

#include "syn6288.h"
float tem,light; 
/*************************************** RTOS任务属性 ***************************************************/

const osThreadAttr_t Main_Task_Attr = 	// 主任务，优先级最高
{
	.priority = osPriorityHigh5,
	.stack_size = 8196,
};

const osThreadAttr_t Msg_Task_Attr = 	// 消息处理任务
{
	.priority = osPriorityLow2,
	.stack_size = 4096,
};

const osThreadAttr_t LED_Task_Attr = 	// 点灯任务
{
	.priority = osPriorityLow1,
	.stack_size = 512,
};

osThreadId_t Main_Task_Id 	= NULL;	// 主任务ID
osThreadId_t Msg_Task_Id 	= NULL;	// 消息处理任务ID
osThreadId_t LED_Task_Id 	= NULL;	// 点灯任务ID

/********************************************** 变量声明 *******************************************/

extern RTC_HandleTypeDef hrtc;	// RTC句柄

RTC_TimeTypeDef Time_Struct;		// 日期
RTC_DateTypeDef Date_Struct;		// 时间

uint16_t show_flag = 0;
/********************************************** 函数声明 *******************************************/
void SystemClock_Config(void);		// 时钟初始化
void MPU_Config(void);					// MPU配置

void RTOS_Main_Task(void *argument);	// 主任务 摄像头+屏幕
void RTOS_Msg_Task(void *argument);		// 消息处理任务
void RTOS_LED_Task(void *argument);		// 电源电压采集+温度采集


/***************************************************************************************************
*	函 数 名: main
*	
*	函数功能: 初始化外设和系统
*	
****************************************************************************************************/

int main(void)
{
	MPU_Config();				// MPU配置
	SCB_EnableICache();		// 使能ICache
	SCB_EnableDCache();		// 使能DCache
	HAL_Init();					// 初始化HAL库	
	SystemClock_Config();	// 配置系统时钟，主频480MHz
	LED_Init();					// 初始化LED引脚
	USART1_Init();				// USART1初始化	115200
	
	USART2_Init();				// USART2初始化	115200
	//osDelay(2000);
	MX_RTC_Init();				// RTC初始化
	OSPI_W25Qxx_Init();		// W25Qxx初始化
	
	TIM3_PWM_Init(500-1,240-1); //200M/200=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
  MX_ADC1_Init();
	
	SD_Status = BSP_SD_Init(SD_Instance);	// 初始化SD卡， SD_Status 标志位用于在 SD_Test() 进行判断
		
	SMBus_Init();		//初始化GY906
	
	osKernelInitialize();	// 初始化RTOS
	PY_usDelayTest();
	PY_usDelayOptimize();
	Main_Task_Id = osThreadNew(RTOS_Main_Task, 	NULL, &Main_Task_Attr);  	// 创建主任务
	Msg_Task_Id  = osThreadNew(RTOS_Msg_Task, 	NULL, &Msg_Task_Attr);  	//	消息处理任务	
	LED_Task_Id  = osThreadNew(RTOS_LED_Task, 	NULL, &LED_Task_Attr);  	//	LED任务

	osKernelStart();	// 启动系统


	while (1)
	{

	}

}


/***************************************************************************************************
*	函 数 名: RTOS_Main_Task
*
*	函数功能: 主任务，优先级最高
*
*	说    明: 无
****************************************************************************************************/

void RTOS_Main_Task(void *argument)
{
	(void)argument;
	
	LCD_identification();		// 屏幕面板识别
	
	SPI_LCD_Init();     			// 液晶屏以及SPI初始化 

//	if( SD_Test()!=BSP_ERROR_NONE )	// 测试失败
//	{
//		LCD_Printf(	"SD Error!!!! ",1);
//	}
//	
//	if( OSPI_W25Qxx_Test()!=OSPI_W25Qxx_OK )	// 测试失败
//	{
//		LCD_Printf(	"W25Q64  Error!!!! ",1);
//	
	
	LCD_SetDirection(Direction_V_Flip);
	
	Camera_Init();				// 初始化OV2640或者5640
	
	LCD_Progress(1);			// 进度条效果，耗时约为 220 * x ms, 同时也显示 RTC 时间
	
	
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
		Camera_Work();	// 运行摄像头
		
		
//		LCD_DisplayString(0,0,"L:");
//		LCD_DisplayDecimals(20,0,(double)light,5,2);
		LCD_DisplayString(70,0,"Tem:");
		LCD_DisplayDecimals(110,0,(double)tem+3,5,2);
		
		//动态catch框
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
*	函 数 名: RTOS_Msg_Task
*
*	函数功能: 消息处理任务
*
*	说    明: 无
****************************************************************************************************/

void RTOS_Msg_Task(void *argument)
{
	(void)argument;

//	printf("RTOS 通信任务\r\n");
	
	while(1)
	{
		if(tem+3 >= 38){
			show_flag = 1;
			SYN_FrameInfo(0, "[v15][m1][t5]体温异常");
//			osDelay(1500);
//				show_flag = 0;
		}
//		HAL_RTC_GetTime(&hrtc,&Time_Struct,RTC_FORMAT_BIN);	// 获取时间

	//		STM32的RTC为确保这 亚秒、时间、日期 3个值来自同一时刻点，
	//		读取 亚秒（RTC_SSR ）或 时间（RTC_TR ）时会锁定高阶影子寄存器中的值，直到读取RTC_DR。	
	//		也就是说用户不读日期的话，时间值会被锁定	
		//HAL_RTC_GetDate(&hrtc,&Date_Struct,RTC_FORMAT_BIN);	// 获取日期
//		printf("%d-%d-%d ",Date_Struct.Year+2000,Date_Struct.Month,Date_Struct.Date);
//		printf("%d:%d:%d\r\n",Time_Struct.Hours,Time_Struct.Minutes,Time_Struct.Seconds);
		
//		SYN_FrameInfo(0, "[v15][m1][t5]识别成功");
		osDelay(1500);
				show_flag = 0;
	}
}

/***************************************************************************************************
*	函 数 名: RTOS_LED_Task
*
*	函数功能: 电池电量检测
*
*	说    明: 无
****************************************************************************************************/

void RTOS_LED_Task(void *argument)
{
	(void)argument;
	while(1)
	{
//		LED1_Toggle;	// LED1 状态翻转
		tem = SMBus_ReadTemp();
//		printf("温度：%.2f\r\n",tem);

		light = get_adc_data();
//		printf("ADC值：%.2f\r\n",V);
		
		//LED补光
		if(light >= 35)
			TIM_SetTIM3Compare4(500);	//修改比较值，修改占空比
		else if(light <= 15)
			TIM_SetTIM3Compare4(0);	//修改比较值，修改占空比
		else
			TIM_SetTIM3Compare4((light-23)*25);	//修改比较值，修改占空比
//		printf("%.2f",tem);
		//测试检测
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
  
/***************************************** SPI4内核时钟设置 **************************************************/
/*** FANKE ***
>>>>> 配置说明：
	*
	*	1.	SPI4 所允许的最大驱动时钟为75M，详情可以查阅：7B0数据手册 第6.3.36.3小节  SPI interface characteristics
   *
	*	2. 为了方便设置和用户移植，这里将2分频后的 pclk（140M） 作为 SPI4 的内核时钟，然后在SPI的配置中设置为2分频得到 70M 的SCK驱动时钟
   *
   *	3. 虽然屏幕控制器ST7789所允许的最高驱动时钟为62.5M，但实际测试中，即使设置为70M也非常稳定，若用户
  *		对SPI时钟有严格要求，可以修改SPI内核时钟来实现
   *
*** FANKE ***/  
 
	PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;	
  
	/* 为了设置方便，将 系统主时钟HCLK 作为OSPI的内核时钟，速度为280M，再经过2分频得到140M驱动时钟  */  
	/* HCLK 也是OSPI的默认的内核时钟，在使用XIP时（在OSPI外设运行代码），也只能使用默认的时钟 */
	/* 因此，虽然140M已经超出了 W25Q64JV 所允许的最高时钟，但还是将OSPI的驱动时钟设置为140M */
	/* 实际测试中，将驱动时钟拉到将近200M，w25q64还是可以正常读写，所以只超出7M的频率不用担心稳定性*/
 	PeriphClkInitStruct.OspiClockSelection = RCC_OSPICLKSOURCE_D1HCLK; //使用 系统主时钟HCLK 作为OSPI的内核时钟，速度为280M，再经过2分频得到140M驱动时钟  
  
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
	
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}    
  
}

//	配置MPU
//
void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	HAL_MPU_Disable();		// 先禁止MPU

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

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);	// 使能MPU
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


