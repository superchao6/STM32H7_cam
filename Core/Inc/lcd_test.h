#ifndef __LCD_TEST_H
#define __LCD_TEST_H

#include "stm32h7xx_hal.h"
#include <string.h>
#include "lcd_spi_096.h"
#include "lcd_identification.h"
#include	"lcd_image.h"
#include "camera_test.h"  		// ����ͷ�������

void 	LCD_Printf(char *pText,int8_t flag);
void 	LCD_Test_Clear(void);			// ��������
void 	LCD_Test_Color(void);			// ����������
void 	LCD_Test_Image(void);			// ͼƬ��ʾ
void  LCD_Test_Direction(void);	   // ������ʾ����
void 	LCD_Progress(uint16_t time);

#endif //__LCD_TEST_H

