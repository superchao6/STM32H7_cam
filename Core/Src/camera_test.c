/***
	*************************************************************************************************
	*	@file  	camera_test.c
	*	@version V1.0
	*  @date    2022-3-22
	*	@author  反客科技	
	*	@brief   核心板测试函数
   *************************************************************************************************
   *  @description
	*
	*	实验平台：反客STM32H750VBT6核心板 （型号：FK750M1-VBT6）
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 文件说明：
	*
	*	摄像头测试函数
	*
	************************************************************************************************
***/

#include "camera_test.h"  
#include "syn6288.h"

volatile int8_t Camera_panle = 0;

uint16_t	LCD_FPS_X =0;	// 用于显示帧率的x、y坐标
uint16_t	LCD_FPS_Y =0;

extern uint16_t show_flag;

int8_t Camera_Init(void)	// 初始化OV2640或者5640
{	
//	printf ("\r\n**************************** 摄像头 测试 **************************** \r\n");	
	
	if( lcd_panle == panle_096 )
	{
		LCD_FPS_X = 8;
		Display_Width 	= LCD_Width;
	}
	else if( lcd_panle == panle_114 )
	{
		LCD_FPS_X = 40;	
		Display_Width 	= LCD_Width+1;		
	}
	else//2寸
	{
		LCD_FPS_X = 70;	
		Display_Width 	= LCD_Width;
	}
	
	Display_Height	= LCD_Height;	
	LCD_FPS_Y = Display_Height-40;
	
	if( DCMI_OV2640_Init() == OV2640_Success )	// 初始SCCB、DCMI、DMA以及配置OV2640
	{
		LCD_Printf(	"Camera:OV2640 >>>",OV2640_Success);	// 显示结果到屏幕	
//		printf("摄像头模块:OV2640-200W像素\r\n");
		OV2640_DMA_Transmit_Continuous(Camera_Buffer, OV2640_BufferSize);  // 启动DMA连续传输

		Camera_panle = Camera_OV2640;

	}
	else if( DCMI_OV5640_Init() == OV5640_Success )		
	{
		LCD_Printf(	"Camera:OV5640 >>>",OV5640_Success);	// 显示结果到屏幕	
//		printf("摄像头模块:OV5640-500W像素\r\n");
		OV5640_AF_Download_Firmware();	// 写入自动对焦固件
		OV5640_AF_Trigger_Constant();		// 自动对焦 ，持续 触发，当OV5640检测到当前画面不在焦点时，会一直触发对焦
	//	OV5640_AF_Trigger_Single();		//	自动对焦 ，触发 单次 
		
	//	120度和160度的广角镜头默认的方向和带自动对焦的镜头不一样，用户可以根据实际去调整
		OV5640_Set_Vertical_Flip( OV5640_Disable );		// 取消垂直翻转
		OV5640_Set_Horizontal_Mirror( OV5640_Enable );	// 水平镜像

		OV5640_DMA_Transmit_Continuous(Camera_Buffer, OV5640_BufferSize);  // 启动DMA连续传输				
		Camera_panle = Camera_OV5640;
	}
	else
	{
		LCD_Printf(	"Camera Error!!!!!",Camera_ERROR);	// 显示结果到屏幕	
//		printf("未检测到摄像头模块！！！！！\r\n");
		Camera_panle = Camera_ERROR;
	}
	

	return Camera_panle;	
	
}

void Camera_Work(void)	// 运行摄像头
{
	if (DCMI_FrameState == 1)	// 采集到了一帧图像
	{		
		DCMI_FrameState = 0;		// 清零标志位
		LCD_CopyBuffer(0,0,Display_Width,Display_Height, (uint16_t *)Camera_Buffer);	// 将图像数据复制到屏幕
		//LCD_SetBackColor(LIGHT_GREY);

		LCD_DisplayText( LCD_FPS_X ,  LCD_FPS_Y,"识别中..."); 	

		if(show_flag >= 2 && show_flag <= 42){
			LCD_DisplayText( LCD_FPS_X ,  LCD_FPS_Y,"SUCCESS!");
			show_flag++ ;
			if(show_flag == 42)
				show_flag = 0;
		}else if(show_flag >= 42 && show_flag <= 82){

			LCD_DisplayText( LCD_FPS_X ,  LCD_FPS_Y,"FAILED!");
			show_flag-- ;
			if(show_flag == 42)
				show_flag = 0;
		}
		//LCD_DisplayNumber( LCD_FPS_X+52,LCD_FPS_Y, OV2640_FPS,2) ;	// 显示帧率					
	}	
}
