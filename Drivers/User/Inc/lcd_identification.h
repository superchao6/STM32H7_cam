
#ifndef __spi_lcd_ide
#define __spi_lcd_ide


#include "stm32h7xx_hal.h"
#include "usart.h"
#include "cmsis_os2.h"

extern volatile uint8_t lcd_panle;

#define panle_096 0x01
#define panle_114 0x02
#define panle_130 0x03
#define panle_154 0x04
#define panle_169 0x05
#define panle_200 0x06

/*--------------------------------------------- LCD���� -----------------------------------------------*/

#define  Init_SCLK_PIN						GPIO_PIN_12				         // SCLK  ����				
#define	Init_SCLK_PORT						GPIOE									// SCLK  GPIO�˿�
#define 	GPIO_Init_SCLK_CLK_ENABLE     __HAL_RCC_GPIOE_CLK_ENABLE()	// SCLK  GPIOʱ�� 	

#define  Init_MOSI_PIN						GPIO_PIN_14				         // MOSI  ����				
#define	Init_MOSI_PORT						GPIOE									// MOSI  GPIO�˿�
#define 	GPIO_Init_MOSI_CLK_ENABLE     __HAL_RCC_GPIOE_CLK_ENABLE()	// MOSI  GPIOʱ�� 	

#define  Init_CS_PIN							GPIO_PIN_11				         // Ƭѡ  ����				
#define	Init_CS_PORT						GPIOE									// Ƭѡ  GPIO�˿�
#define 	GPIO_Init_CS_CLK_ENABLE     	__HAL_RCC_GPIOE_CLK_ENABLE()	// Ƭѡ  GPIOʱ�� 	

#define  Init_DC_PIN							GPIO_PIN_15				         // ����ָ��ѡ��  ����				
#define	Init_DC_PORT						GPIOE									// ����ָ��ѡ��  GPIO�˿�
#define 	GPIO_Init_DC_CLK_ENABLE     __HAL_RCC_GPIOE_CLK_ENABLE()		// ����ָ��ѡ��  GPIOʱ�� 	

#define  Init_BLK_PIN						GPIO_PIN_15				         // ����  ����				
#define	Init_BLK_PORT						GPIOD									// ����  GPIO�˿�
#define 	GPIO_Init_BLK_CLK_ENABLE     __HAL_RCC_GPIOD_CLK_ENABLE()		// ����  GPIOʱ�� 	


//-----------------------------------------������---------------------------------------------------- 

#define Init_SCLK_L     HAL_GPIO_WritePin(Init_SCLK_PORT, Init_SCLK_PIN, GPIO_PIN_RESET)
#define Init_SCLK_H     HAL_GPIO_WritePin(Init_SCLK_PORT, Init_SCLK_PIN, GPIO_PIN_SET)

#define Init_MOSI_L     HAL_GPIO_WritePin(Init_MOSI_PORT, Init_MOSI_PIN, GPIO_PIN_RESET)
#define Init_MOSI_H     HAL_GPIO_WritePin(Init_MOSI_PORT, Init_MOSI_PIN, GPIO_PIN_SET)

#define Init_CS_L       HAL_GPIO_WritePin(Init_CS_PORT, Init_CS_PIN, GPIO_PIN_RESET)
#define Init_CS_H       HAL_GPIO_WritePin(Init_CS_PORT, Init_CS_PIN, GPIO_PIN_SET)

#define Init_DC_L       HAL_GPIO_WritePin(Init_DC_PORT, Init_DC_PIN, GPIO_PIN_RESET)
#define Init_DC_H       HAL_GPIO_WritePin(Init_DC_PORT, Init_DC_PIN, GPIO_PIN_SET)
 		     
#define Init_BLK_L       HAL_GPIO_WritePin(Init_BLK_PORT, Init_BLK_PIN, GPIO_PIN_RESET)			  

#define Init_MOSI_READ()   HAL_GPIO_ReadPin(Init_MOSI_PORT, Init_MOSI_PIN)

#define Init_DC_READ()     HAL_GPIO_ReadPin(Init_DC_PORT, Init_DC_PIN)

#define Init_BLK_READ()    HAL_GPIO_ReadPin(Init_BLK_PORT, Init_BLK_PIN)


#define Init_CS_READ()    HAL_GPIO_ReadPin(Init_CS_PORT, Init_CS_PIN)


//----------------------------------------����---------------------------------------------------- 


uint32_t LCD_ReadID(void);
void Init_LCD(void);
void LCD_identification(void);

#endif //__spi_lcd_ide


