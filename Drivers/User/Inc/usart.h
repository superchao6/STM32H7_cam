#ifndef __USART_H
#define __USART_H

#include "stm32h7xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#include "stm32h7b0xx.h"

/*-------------------------------------------- USART1���ú� ---------------------------------------*/
#define  USART1_BaudRate  115200

#define  USART1_TX_PIN									GPIO_PIN_9								// TX ����
#define	USART1_TX_PORT									GPIOA										// TX ���Ŷ˿�
#define 	GPIO_USART1_TX_CLK_ENABLE        	   __HAL_RCC_GPIOA_CLK_ENABLE()	 	// TX ����ʱ��


#define  USART1_RX_PIN									GPIO_PIN_10             			// RX ����
#define	USART1_RX_PORT									GPIOA                 				// RX ���Ŷ˿�
#define 	GPIO_USART1_RX_CLK_ENABLE         	   __HAL_RCC_GPIOA_CLK_ENABLE()		// RX ����ʱ��


/*-------------------------------------------- USART2���ú� ---------------------------------------*/
#define  USART2_BaudRate  115200

#define  USART2_TX_PIN									GPIO_PIN_2								// TX ����
#define	USART2_TX_PORT									GPIOA										// TX ���Ŷ˿�
#define 	GPIO_USART2_TX_CLK_ENABLE        	   __HAL_RCC_GPIOA_CLK_ENABLE()	 	// TX ����ʱ��


#define  USART2_RX_PIN									GPIO_PIN_3             			// RX ����
#define	USART2_RX_PORT									GPIOA                 				// RX ���Ŷ˿�
#define 	GPIO_USART2_RX_CLK_ENABLE         	   __HAL_RCC_GPIOA_CLK_ENABLE()		// RX ����ʱ��

#define USART2_SENDBUFF_MAX_BYTES	100U	//����1���ͻ�������С ��λ�ֽ�

/*---------------------------------------------- �������� ---------------------------------------*/

void USART1_Init(void) ;	// USART1��ʼ������

void USART2_Init(void) ;	// USART1��ʼ������

int USART2Printf(const char* format, ...);


void USART1_SendData(uint8_t data);
void USART1_SendString(uint8_t *DAT, uint8_t len);

#endif //__USART_H





