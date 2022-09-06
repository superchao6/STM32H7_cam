/***
	************************************************************************************************
	*	@file  	usart.c
	*	@version V1.0
	*  @date    2021-10-26
	*	@author  ���ͿƼ�	
	*	@brief   ���ڴ�ӡ����
   *************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨������STM32H7B0���İ� 
	*	�Ա���ַ��https://shop212360197.taobao.com
	*	QQ����Ⱥ��536665479
	*
>>>>> �ļ�˵����
	*
	*  ��ʼ��usart���ţ����ò����ʵȲ���
	*
	************************************************************************************************
***/


#include "usart.h"
#include "main.h"
#include "syn6288.h"

UART_HandleTypeDef huart1;  // UART_HandleTypeDef �ṹ�����

UART_HandleTypeDef huart2;  // UART_HandleTypeDef �ṹ�����

#define RXBUFFERSIZE  256     //�������ֽ���
char Uart2RxBuffer[RXBUFFERSIZE];   //��������
uint8_t aUart2RxBuffer;			//�����жϻ���
uint8_t Uart2_Rx_Cnt = 0;		//���ջ������

extern float tem,light;
extern uint16_t show_flag;
/*************************************************************************************************
*	�� �� ��:	HAL_UART_MspInit
*	��ڲ���:	huart - UART_HandleTypeDef����ı���������ʾ����Ĵ���
*	�� �� ֵ:	��
*	��������:	��ʼ����������
*	˵    ��:	��		
*************************************************************************************************/


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	
	if(huart->Instance==USART1)
	{
		__HAL_RCC_USART1_CLK_ENABLE();		// ���� USART1 ʱ��

		GPIO_USART1_TX_CLK_ENABLE;				// ���� USART1 TX ���ŵ� GPIO ʱ��
		GPIO_USART1_RX_CLK_ENABLE;				// ���� USART1 RX ���ŵ� GPIO ʱ��

		GPIO_InitStruct.Pin 			= USART1_TX_PIN;					// TX����
		GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;				// �����������
		GPIO_InitStruct.Pull 		= GPIO_PULLUP;						// ����
		GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;	// �ٶȵȼ� 
		GPIO_InitStruct.Alternate 	= GPIO_AF7_USART1;				// ����ΪUSART1
		HAL_GPIO_Init(USART1_TX_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin 			= USART1_RX_PIN;					// RX����
		HAL_GPIO_Init(USART1_RX_PORT, &GPIO_InitStruct);		
	}
	else if(huart->Instance==USART2)
	{
		__HAL_RCC_USART2_CLK_ENABLE();		// ���� USART2 ʱ��

		GPIO_USART2_TX_CLK_ENABLE;				// ���� USART2 TX ���ŵ� GPIO ʱ��
		GPIO_USART2_RX_CLK_ENABLE;				// ���� USART2 RX ���ŵ� GPIO ʱ��

		GPIO_InitStruct.Pin 			= USART2_TX_PIN;					// TX����
		GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;				// �����������
		GPIO_InitStruct.Pull 		= GPIO_PULLUP;						// ����
		GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;	// �ٶȵȼ� 
		GPIO_InitStruct.Alternate 	= GPIO_AF7_USART2;				// ����ΪUSART1
		HAL_GPIO_Init(USART2_TX_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin 			= USART2_RX_PIN;					// RX����
		HAL_GPIO_Init(USART2_RX_PORT, &GPIO_InitStruct);		
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
		
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
*	�� �� ��:	USART1_Init
*	��ڲ���:	��
*	�� �� ֵ:	��
*	��������:	��ʼ����������
*	˵    ��:	��		 
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
*	�� �� ��:	USART2_Init
*	��ڲ���:	��
*	�� �� ֵ:	��
*	��������:	��ʼ����������
*	˵    ��:	��		 
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
*	�� �� ��:	fputc
*	��ڲ���:	ch - Ҫ������ַ� ��  f - �ļ�ָ�루�����ò�����
*	�� �� ֵ:	����ʱ�����ַ�������ʱ���� EOF��-1��
*	��������:	�ض��� fputc ������Ŀ����ʹ�� printf ����
*	˵    ��:	��		
*************************************************************************************************/

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);	// ���͵��ֽ�����
	return (ch);
}

void USART1_SendData(uint8_t data)
{
	HAL_UART_Transmit(&huart1, &data, 1, 100);	// ���͵��ֽ�����
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
  * @brief  UART ��printf����
  * @param  format	������ַ���
  * @retval ����д����ַ�����
  */
int USART2Printf(const char* format, ...)
{
	static char sendBuff[USART2_SENDBUFF_MAX_BYTES] = { 0 };//���ͻ�����
	int bytes = 0;
	va_list list;

	va_start(list, format);
	bytes = vsprintf(sendBuff, format, list);//��ʽ������
	va_end(list);
	/* ����֮ǰ�����־λ */
//	CLEAR_BIT(huart2.Instance->SR, USART_SR_TC_Msk);//��TCλд��0�����TCλ
	HAL_UART_Transmit(&huart2, (void*)sendBuff, bytes, 0xff);//����ʽ�������ݣ����͵ȴ�ʱ��Ϊ���ȴ�ʱ��

	return bytes;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(Uart2_Rx_Cnt >= 255)  //����ж�
	{
		Uart2_Rx_Cnt = 0;
		memset(Uart2RxBuffer,0x00,sizeof(Uart2RxBuffer));
		HAL_UART_Transmit(&huart2, (uint8_t *)"�������", 10,0xFFFF); 	
        
	}
	else
	{
		Uart2RxBuffer[Uart2_Rx_Cnt++] = aUart2RxBuffer;   //��������ת��
//			if(Uart2RxBuffer[0] == 'A'){
//				USART2Printf("%f",tem);
//				show_flag = 1;
//							while(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);//���UART���ͽ���
//			}
			if(Uart2RxBuffer[0] == 'T' && show_flag != 1){
				show_flag = 2;//ʶ��ɹ�	
				
				//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
				//m[0~16]:0��������Ϊ������16���������������
				//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
				//t[0~5]:0�ʶ�����������5�ʶ��������
				//���������ù�����ο������ֲ�
				SYN_FrameInfo(0, "[v16][m0][t5]ʶ��ɹ�");				
			}else if(Uart2RxBuffer[0] == 'H'){
				show_flag = 82;//ʶ��ʧ��
				SYN_FrameInfo(0, "[v15][m1][t5]�����쳣");
			}else if(Uart2RxBuffer[0] == 'J'){
				show_flag = 82;//ʶ��ʧ��
				SYN_FrameInfo(0, "[v15][m1][t5]�������쳣");
			}else if(Uart2RxBuffer[0] == 'W'){
				show_flag = 82;//ʶ��ʧ��
				SYN_FrameInfo(0, "[v15][m1][t5]�������Ϣ");
			}else if(Uart2RxBuffer[0] == 'I'){
				show_flag = 82;//ʶ��ʧ��
				SYN_FrameInfo(0, "[v15][m1][t5]¼��ɹ�");
			}else if(Uart2RxBuffer[0] == 'E'){
				show_flag = 82;//ʶ��ʧ��
				SYN_FrameInfo(0, "[v15][m1][t5]¼��ʧ��");
			}else if(Uart2RxBuffer[0] == 'O'){
				show_flag = 82;//ʶ��ʧ��
				SYN_FrameInfo(0, "[v15][m1][t5]δ֪����");
			}
			Uart2_Rx_Cnt = 0;
			memset(Uart2RxBuffer,0x00,sizeof(Uart2RxBuffer)); //�������
	}
	
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aUart2RxBuffer, 1);   //�ٿ��������ж�
}



