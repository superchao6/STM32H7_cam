/***
	*************************************************************************************************
	*	@file  	camera_test.c
	*	@version V1.0
	*  @date    2022-3-22
	*	@author  ���ͿƼ�	
	*	@brief   ���İ���Ժ���
   *************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨������STM32H750VBT6���İ� ���ͺţ�FK750M1-VBT6��
	*	�Ա���ַ��https://shop212360197.taobao.com
	*	QQ����Ⱥ��536665479
	*
>>>>> �ļ�˵����
	*
	*	����ͷ���Ժ���
	*
	************************************************************************************************
***/

#include "camera_test.h"  
#include "syn6288.h"

volatile int8_t Camera_panle = 0;

uint16_t	LCD_FPS_X =0;	// ������ʾ֡�ʵ�x��y����
uint16_t	LCD_FPS_Y =0;

extern uint16_t show_flag;

int8_t Camera_Init(void)	// ��ʼ��OV2640����5640
{	
//	printf ("\r\n**************************** ����ͷ ���� **************************** \r\n");	
	
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
	else//2��
	{
		LCD_FPS_X = 70;	
		Display_Width 	= LCD_Width;
	}
	
	Display_Height	= LCD_Height;	
	LCD_FPS_Y = Display_Height-40;
	
	if( DCMI_OV2640_Init() == OV2640_Success )	// ��ʼSCCB��DCMI��DMA�Լ�����OV2640
	{
		LCD_Printf(	"Camera:OV2640 >>>",OV2640_Success);	// ��ʾ�������Ļ	
//		printf("����ͷģ��:OV2640-200W����\r\n");
		OV2640_DMA_Transmit_Continuous(Camera_Buffer, OV2640_BufferSize);  // ����DMA��������

		Camera_panle = Camera_OV2640;

	}
	else if( DCMI_OV5640_Init() == OV5640_Success )		
	{
		LCD_Printf(	"Camera:OV5640 >>>",OV5640_Success);	// ��ʾ�������Ļ	
//		printf("����ͷģ��:OV5640-500W����\r\n");
		OV5640_AF_Download_Firmware();	// д���Զ��Խ��̼�
		OV5640_AF_Trigger_Constant();		// �Զ��Խ� ������ ��������OV5640��⵽��ǰ���治�ڽ���ʱ����һֱ�����Խ�
	//	OV5640_AF_Trigger_Single();		//	�Զ��Խ� ������ ���� 
		
	//	120�Ⱥ�160�ȵĹ�Ǿ�ͷĬ�ϵķ���ʹ��Զ��Խ��ľ�ͷ��һ�����û����Ը���ʵ��ȥ����
		OV5640_Set_Vertical_Flip( OV5640_Disable );		// ȡ����ֱ��ת
		OV5640_Set_Horizontal_Mirror( OV5640_Enable );	// ˮƽ����

		OV5640_DMA_Transmit_Continuous(Camera_Buffer, OV5640_BufferSize);  // ����DMA��������				
		Camera_panle = Camera_OV5640;
	}
	else
	{
		LCD_Printf(	"Camera Error!!!!!",Camera_ERROR);	// ��ʾ�������Ļ	
//		printf("δ��⵽����ͷģ�飡��������\r\n");
		Camera_panle = Camera_ERROR;
	}
	

	return Camera_panle;	
	
}

void Camera_Work(void)	// ��������ͷ
{
	if (DCMI_FrameState == 1)	// �ɼ�����һ֡ͼ��
	{		
		DCMI_FrameState = 0;		// �����־λ
		LCD_CopyBuffer(0,0,Display_Width,Display_Height, (uint16_t *)Camera_Buffer);	// ��ͼ�����ݸ��Ƶ���Ļ
		//LCD_SetBackColor(LIGHT_GREY);

		LCD_DisplayText( LCD_FPS_X ,  LCD_FPS_Y,"ʶ����..."); 	

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
		//LCD_DisplayNumber( LCD_FPS_X+52,LCD_FPS_Y, OV2640_FPS,2) ;	// ��ʾ֡��					
	}	
}
