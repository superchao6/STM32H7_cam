#ifndef __Camera_TEST_H
#define __Camera_TEST_H

#include "stm32h7xx_hal.h"
#include "lcd_spi_096.h"
#include "lcd_identification.h"
#include "lcd_test.h"  			// LCD�������

#include "dcmi_ov2640.h"  
#include "dcmi_ov5640.h"  

extern volatile int8_t Camera_panle;

#define Camera_Buffer	0x24000000    // ����ͷͼ�񻺳���

#define Camera_ERROR	 -1
#define Camera_OV2640 0
#define Camera_OV5640 1


int8_t Camera_Init(void);	// ��ʼ��OV2640����5640
void Camera_Work(void);	// ��������ͷ

#endif //__Camera_TEST_H

