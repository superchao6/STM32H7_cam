/***
	************************************************************************************************
	*	@file  	usart.c
	*	@version V1.0
	*  @date    2021-10-26
	*	@author  反客科技	
	*	@brief   串口打印测试
   *************************************************************************************************
   *  @description
	*
	*	实验平台：反客STM32H7B0核心板 
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 文件说明：
	*
	*  初始化usart引脚，配置波特率等参数
	*
	************************************************************************************************
***/


#include "usart.h"
#include "main.h"
#include "syn6288.h"

UART_HandleTypeDef huart1;  // UART_HandleTypeDef 结构体变量

UART_HandleTypeDef huart2;  // UART_HandleTypeDef 结构体变量

#define RXBUFFERSIZE  256     //最大接收字节数
char Uart2RxBuffer[RXBUFFERSIZE];   //接收数据
uint8_t aUart2RxBuffer;			//接收中断缓冲
uint8_t Uart2_Rx_Cnt = 0;		//接收缓冲计数

extern float tem,light;
extern uint16_t show_flag;
/*************************************************************************************************
*	函 数 名:	HAL_UART_MspInit
*	入口参数:	huart - UART_HandleTypeDef定义的变量，即表示定义的串口
*	返 回 值:	无
*	函数功能:	初始化串口引脚
*	说    明:	无		
*************************************************************************************************/


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	
	if(huart->Instance==USART1)
	{
		__HAL_RCC_USART1_CLK_ENABLE();		// 开启 USART1 时钟

		GPIO_USART1_TX_CLK_ENABLE;				// 开启 USART1 TX 引脚的 GPIO 时钟
		GPIO_USART1_RX_CLK_ENABLE;				// 开启 USART1 RX 引脚的 GPIO 时钟

		GPIO_InitStruct.Pin 			= USART1_TX_PIN;					// TX引脚
		GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;				// 复用推挽输出
		GPIO_InitStruct.Pull 		= GPIO_PULLUP;						// 上拉
		GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;	// 速度等级 
		GPIO_InitStruct.Alternate 	= GPIO_AF7_USART1;				// 复用为USART1
		HAL_GPIO_Init(USART1_TX_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin 			= USART1_RX_PIN;					// RX引脚
		HAL_GPIO_Init(USART1_RX_PORT, &GPIO_InitStruct);		
	}
	else if(huart->Instance==USART2)
	{
		__HAL_RCC_USART2_CLK_ENABLE();		// 开启 USART2 时钟

		GPIO_USART2_TX_CLK_ENABLE;				// 开启 USART2 TX 引脚的 GPIO 时钟
		GPIO_USART2_RX_CLK_ENABLE;				// 开启 USART2 RX 引脚的 GPIO 时钟

		GPIO_InitStruct.Pin 			= USART2_TX_PIN;					// TX引脚
		GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;				// 复用推挽输出
		GPIO_InitStruct.Pull 		= GPIO_PULLUP;						// 上拉
		GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;	// 速度等级 
		GPIO_InitStruct.Alternate 	= GPIO_AF7_USART2;				// 复用为USART1
		HAL_GPIO_Init(USART2_TX_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin 			= USART2_RX_PIN;					// RX引脚
		HAL_GPIO_Init(USART2_RX_PORT, &GPIO_InitStruct);		
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
		
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */

  /** Initializes the peripherals clock
  */
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
			
    }

    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2   GSD  ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
		HAL_NVIC_SetPriority(USART2_IRQn, 3, 3);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		/* USER CODE BEGIN USART2_MspInit 1 */

		/* USER CODE END USART2_MspInit 1 */
	}

}

/*************************************************************************************************
*	函 数 名:	USART1_Init
*	入口参数:	无
*	返 回 值:	无
*	函数功能:	初始化串口配置
*	说    明:	无		 
*************************************************************************************************/

void USART1_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {

  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {

  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {

  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {

  }
}
/*************************************************************************************************
*	函 数 名:	USART2_Init
*	入口参数:	无
*	返 回 值:	无
*	函数功能:	初始化串口配置
*	说    明:	无		 
*************************************************************************************************/

void USART2_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {

  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {

  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {

  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {

  }
	
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aUart2RxBuffer, 1);
}

/*************************************************************************************************
*	函 数 名:	fputc
*	入口参数:	ch - 要输出的字符 ，  f - 文件指针（这里用不到）
*	返 回 值:	正常时返回字符，出错时返回 EOF（-1）
*	函数功能:	重定向 fputc 函数，目的是使用 printf 函数
*	说    明:	无		
*************************************************************************************************/

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);	// 发送单字节数据
	return (ch);
}

void USART1_SendData(uint8_t data)
{
	HAL_UART_Transmit(&huart1, &data, 1, 100);	// 发送单字节数据
}

void USART1_SendString(uint8_t *DAT, uint8_t len)
{
	uint8_t i;
	for(i = 0; i < len; i++)
	{
		USART1_SendData(*DAT++);
	}
}

/**
  * @brief  UART 仿printf发送
  * @param  format	输出的字符串
  * @retval 返回写入的字符总数
  */
int USART2Printf(const char* format, ...)
{
	static char sendBuff[USART2_SENDBUFF_MAX_BYTES] = { 0 };//发送缓冲区
	int bytes = 0;
	va_list list;

	va_start(list, format);
	bytes = vsprintf(sendBuff, format, list);//格式化输入
	va_end(list);
	/* 发送之前清除标志位 */
//	CLEAR_BIT(huart2.Instance->SR, USART_SR_TC_Msk);//往TC位写入0来清除TC位
	HAL_UART_Transmit(&huart2, (void*)sendBuff, bytes, 0xff);//阻塞式发送数据，发送等待时间为最大等待时间

	return bytes;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(Uart2_Rx_Cnt >= 255)  //溢出判断
	{
		Uart2_Rx_Cnt = 0;
		memset(Uart2RxBuffer,0x00,sizeof(Uart2RxBuffer));
		HAL_UART_Transmit(&huart2, (uint8_t *)"数据溢出", 10,0xFFFF); 	
        
	}
	else
	{
		Uart2RxBuffer[Uart2_Rx_Cnt++] = aUart2RxBuffer;   //接收数据转存
//			if(Uart2RxBuffer[0] == 'A'){
//				USART2Printf("%f",tem);
//				show_flag = 1;
//							while(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);//检测UART发送结束
//			}
			if(Uart2RxBuffer[0] == 'T' && show_flag != 1){
				show_flag = 2;//识别成功	
				
				//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
				//m[0~16]:0背景音乐为静音，16背景音乐音量最大
				//v[0~16]:0朗读音量为静音，16朗读音量最大
				//t[0~5]:0朗读语速最慢，5朗读语速最快
				//其他不常用功能请参考数据手册
				SYN_FrameInfo(0, "[v16][m0][t5]识别成功");				
			}else if(Uart2RxBuffer[0] == 'H'){
				show_flag = 82;//识别失败
				SYN_FrameInfo(0, "[v15][m1][t5]体温异常");
			}else if(Uart2RxBuffer[0] == 'J'){
				show_flag = 82;//识别失败
				SYN_FrameInfo(0, "[v15][m1][t5]健康码异常");
			}else if(Uart2RxBuffer[0] == 'W'){
				show_flag = 82;//识别失败
				SYN_FrameInfo(0, "[v15][m1][t5]无身份信息");
			}else if(Uart2RxBuffer[0] == 'I'){
				show_flag = 82;//识别失败
				SYN_FrameInfo(0, "[v15][m1][t5]录入成功");
			}else if(Uart2RxBuffer[0] == 'E'){
				show_flag = 82;//识别失败
				SYN_FrameInfo(0, "[v15][m1][t5]录入失败");
			}else if(Uart2RxBuffer[0] == 'O'){
				show_flag = 82;//识别失败
				SYN_FrameInfo(0, "[v15][m1][t5]未知错误");
			}
			Uart2_Rx_Cnt = 0;
			memset(Uart2RxBuffer,0x00,sizeof(Uart2RxBuffer)); //清空数组
	}
	
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aUart2RxBuffer, 1);   //再开启接收中断
}



