#ifndef _TIMER_H
#define _TIMER_H
#include "stm32h7xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3PWM��� 
extern TIM_OC_InitTypeDef TIM3_CH4Handler; 	//��ʱ��3ͨ��4���

void TIM3_Init(uint16_t arr,uint16_t psc);    		//��ʱ����ʼ��
void TIM3_PWM_Init(uint16_t arr,uint16_t psc);
void TIM_SetTIM3Compare4(uint32_t compare);

#endif

