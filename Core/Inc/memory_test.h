#ifndef __MEMORY_TEST_H
#define __MEMORY_TEST_H

#include "stm32h7xx_hal.h"
#include "ospi_w25q64.h"
#include "sdmmc_sd.h"
#include <string.h>
#include "lcd_spi_096.h"
#include "lcd_identification.h"
#include "lcd_test.h"  			// LCD测试相关

extern int32_t SD_Status ; 		 //SD卡检测标志位

int8_t SD_Test(void);	 				// SD卡测试, 				返回 BSP_ERROR_NONE 代表测试成功
int8_t OSPI_W25Qxx_Test(void);		// QSPI W25Q64读写测试,	返回 QSPI_W25Qxx_OK 代表测试成功
	

#endif //__MEMORY_TEST_H

