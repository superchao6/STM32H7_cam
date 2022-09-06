/*******************************************************************************
* 文件名		: mlx90614.h
* 作  者	: 
* 版  本	: 
* 日  期	: 2013-08-07
* 描  述	: mlx90614函数
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IIC_H
#define __IIC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(uint8_t);
uint8_t SMBus_SendByte(uint8_t);
uint8_t SMBus_ReceiveBit(void);
uint8_t SMBus_ReceiveByte(uint8_t);
//void SMBus_Delay(uint16_t);
void SMBus_Init(void);
uint16_t SMBus_ReadMemory(uint8_t, uint8_t);
uint8_t PEC_Calculation(uint8_t*);
float SMBus_ReadTemp(void); //获取温度值


void PY_usDelayTest(void);
void PY_usDelayOptimize(void);
void SMBus_Delay(uint32_t Delay);

#endif

/*********************************END OF FILE*********************************/
