#ifndef __LCD_TEST_H
#define __LCD_TEST_H

#include "stm32h7xx_hal.h"
#include <string.h>
#include "lcd_spi_096.h"
#include "lcd_identification.h"
#include	"lcd_image.h"
#include "camera_test.h"  		// 摄像头测试相关

void 	LCD_Printf(char *pText,int8_t flag);
void 	LCD_Test_Clear(void);			// 清屏测试
void 	LCD_Test_Color(void);			// 矩形填充测试
void 	LCD_Test_Image(void);			// 图片显示
void  LCD_Test_Direction(void);	   // 更换显示方向
void 	LCD_Progress(uint16_t time);

#endif //__LCD_TEST_H

