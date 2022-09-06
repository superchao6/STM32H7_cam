#ifndef __USART_H
#define __USART_H

#include "stm32h7xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#include "stm32h7b0xx.h"

/*-------------------------------------------- USART1配置宏 ---------------------------------------*/
#define  USART1_BaudRate  115200

#define  USART1_TX_PIN									GPIO_PIN_9								// TX 引脚
#define	USART1_TX_PORT									GPIOA										// TX 引脚端口
#define 	GPIO_USART1_TX_CLK_ENABLE        	   __HAL_RCC_GPIOA_CLK_ENABLE()	 	// TX 引脚时钟


#define  USART1_RX_PIN									GPIO_PIN_10             			// RX 引脚
#define	USART1_RX_PORT									GPIOA                 				// RX 引脚端口
#define 	GPIO_USART1_RX_CLK_ENABLE         	   __HAL_RCC_GPIOA_CLK_ENABLE()		// RX 引脚时钟


/*-------------------------------------------- USART2配置宏 ---------------------------------------*/
#define  USART2_BaudRate  115200

#define  USART2_TX_PIN									GPIO_PIN_2								// TX 引脚
#define	USART2_TX_PORT									GPIOA										// TX 引脚端口
#define 	GPIO_USART2_TX_CLK_ENABLE        	   __HAL_RCC_GPIOA_CLK_ENABLE()	 	// TX 引脚时钟


#define  USART2_RX_PIN									GPIO_PIN_3             			// RX 引脚
#define	USART2_RX_PORT									GPIOA                 				// RX 引脚端口
#define 	GPIO_USART2_RX_CLK_ENABLE         	   __HAL_RCC_GPIOA_CLK_ENABLE()		// RX 引脚时钟

#define USART2_SENDBUFF_MAX_BYTES	100U	//串口1发送缓冲区大小 单位字节

/*---------------------------------------------- 函数声明 ---------------------------------------*/

void USART1_Init(void) ;	// USART1初始化函数

void USART2_Init(void) ;	// USART1初始化函数

int USART2Printf(const char* format, ...);


void USART1_SendData(uint8_t data);
void USART1_SendString(uint8_t *DAT, uint8_t len);

#endif //__USART_H





