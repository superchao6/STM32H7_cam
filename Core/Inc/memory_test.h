#ifndef __MEMORY_TEST_H
#define __MEMORY_TEST_H

#include "stm32h7xx_hal.h"
#include "ospi_w25q64.h"
#include "sdmmc_sd.h"
#include <string.h>
#include "lcd_spi_096.h"
#include "lcd_identification.h"
#include "lcd_test.h"  			// LCD�������

extern int32_t SD_Status ; 		 //SD������־λ

int8_t SD_Test(void);	 				// SD������, 				���� BSP_ERROR_NONE ������Գɹ�
int8_t OSPI_W25Qxx_Test(void);		// QSPI W25Q64��д����,	���� QSPI_W25Qxx_OK ������Գɹ�
	

#endif //__MEMORY_TEST_H

