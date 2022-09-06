/***
	*************************************************************************************************
	*	@file  	lcd_test.c
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
	*	lcd测试相关
	*
	************************************************************************************************
***/

#include "lcd_test.h"  

extern RTC_HandleTypeDef hrtc;	// RTC句柄

extern RTC_TimeTypeDef Time_Struct;		// 日期
extern RTC_DateTypeDef Date_Struct;		// 时间

/****************************************************************************************************************************************
*	函 数 名:	 LCD_Progress

*	函数功能: 进度条效果 			 
*****************************************************************************************************************************************/

void LCD_Progress(uint16_t time)	
{
	uint8_t	i = 0;			// 计数变量
	char   Char_Buffer[15];				// 用于存储转换后的字符串
	
	LCD_SetColor(LIGHT_YELLOW);		//	设置画笔色
	
	if( lcd_panle == panle_096 )
	{
		for(i=0;i<160;i++)
		{
			LCD_DrawLine_V(i,75,5);	// 绘制矩形，实现简易进度条的效果
			sprintf( Char_Buffer , "RTC-%02d:%02d:%02d",Time_Struct.Hours, Time_Struct.Minutes, Time_Struct.Seconds );	// 将 number 转换成字符串，便于显示	
			LCD_DisplayString( 10,57,  Char_Buffer);	 				
			osDelay(time);	
		}	
	}	
	else if( lcd_panle == panle_114 )
	{
		for(i=10;i<230;i++)
		{
			LCD_DrawLine_V(i,100,6);	// 绘制矩形，实现简易进度条的效果
			sprintf( Char_Buffer , "RTC-%02d:%02d:%02d",Time_Struct.Hours, Time_Struct.Minutes, Time_Struct.Seconds );	// 将 number 转换成字符串，便于显示	
			LCD_DisplayString( 10,70,  Char_Buffer);	 				
			osDelay(time);	
		}	
	}		
	else
	{
		for(i=10;i<230;i++)
		{
			LCD_DrawLine_V(i,200,6);	// 绘制矩形，实现简易进度条的效果
			sprintf( Char_Buffer , "RTC-%02d:%02d:%02d",Time_Struct.Hours, Time_Struct.Minutes, Time_Struct.Seconds );	// 将 number 转换成字符串，便于显示	
			LCD_DisplayString( 10,140,  Char_Buffer);	 			
			osDelay(time);	
		}	
	}	
	
	if( Camera_panle != Camera_ERROR)	// 如果检测到摄像头，则切换到垂直方向，显示画面
	{
		LCD_SetDirection(Direction_V_Flip);	
	}

}
/****************************************************************************************************************************************
*	函 数 名:	 LCD_SetAddress

*	函数功能: 打印测试结果到屏幕上	 			 
*****************************************************************************************************************************************/

void LCD_Printf(char *pText,int8_t flag)	
{
	static uint16_t y = 0;
	
//设置画笔色>>>>>>	
	if( flag == 0 )	// 成功标志位0
	{
		LCD_SetColor(LIGHT_GREEN);   	// 绿色画笔   
	}
	else	//错误
	{
		LCD_SetColor(LCD_RED);   	// 红色画笔   
	}	
	
//设置字体>>>>>>		
	if( (lcd_panle == panle_096)||(lcd_panle == panle_114) )
	{
		LCD_SetTextFont(&CH_Font16);			// 设置2424中文字体,ASCII字体对应为2412	
		y = y+20;
	}
	else
	{
		LCD_SetTextFont(&CH_Font24);			// 设置2424中文字体,ASCII字体对应为2412
		y = y+30;
	}

	LCD_DisplayString(10,y-20,pText);	

}

/*************************************************************************************************
*	函 数 名:	LCD_Test_Clear
*
*	函数功能:	清屏测试
*
*************************************************************************************************/

void LCD_Test_Clear(void)
{
	uint32_t	i = 0;			// 计数变量
	
	LCD_SetDirection(Direction_V);		
	LCD_SetTextFont(&CH_Font24);			// 设置2424中文字体,ASCII字体对应为2412
	
	if( lcd_panle == panle_114 )
   {
		LCD_SetDirection(Direction_H_Flip);		
	}
	else if( lcd_panle == panle_096 )
	{
		LCD_SetDirection(Direction_H_Flip);		
		LCD_SetTextFont(&CH_Font16);			
	}

	
	LCD_SetColor(LCD_BLACK);				// 设置画笔颜色

	for(i=0;i<8;i++)
	{
		LCD_SetBackColor(LCD_WHITE-i*3088960-i*45000-i*400);	// 计算一个颜色用于背景色
		LCD_Clear();		// 清屏

		if( lcd_panle == panle_096 )
		{
			LCD_DisplayText(13, 5,"STM32H7 刷屏测试");
			LCD_DisplayText(13,30,"屏幕分辨率:160*80");
			LCD_DisplayText(13,55,"控制器:ST7735");	
		}

		else if( lcd_panle == panle_114 )
		{
			LCD_DisplayText(13, 20,"STM32H7 刷屏测试");
			LCD_DisplayText(13,56,"屏幕分辨率:240*135");
			LCD_DisplayText(13,92,"控制器:ST7789");		
		}
		else
		{	
			LCD_DisplayText(13, 70,"STM32H7 刷屏测试");
			LCD_DisplayText(13,142,"控制器:ST7789");	
			if( lcd_panle == panle_130 )
			{
				LCD_DisplayText(13,106,"屏幕分辨率:240*240");
			}	
			else if( lcd_panle == panle_169 )
			{
				LCD_DisplayText(13,106,"屏幕分辨率:240*280");			
			}
			else 
			{
				LCD_DisplayText(13,106,"屏幕分辨率:240*320");	
			}					
		}
			
		osDelay(1000);	// 延时
	}
	
}

/*************************************************************************************************
*	函 数 名:	LCD_Test_Color
*
*	函数功能:	颜色测
*************************************************************************************************/
void LCD_Test_Color(void)
{
	uint16_t i;					// 计数变量
	uint16_t y;
// 颜色测试>>>>>	
	LCD_SetBackColor(LCD_BLACK); //设置背景色
	LCD_Clear(); //清屏，刷背景色

	LCD_SetDirection(Direction_H_Flip);
	LCD_SetColor(LCD_WHITE);				// 设置画笔颜色
	LCD_SetTextFont(&CH_Font20);			// 设置字体
	
	if( lcd_panle == panle_096 )
   {

		LCD_SetTextFont(&CH_Font16);			// 设置字体

		LCD_DisplayText(0,0,"RGB Color:");

		//使用画线函数绘制三基色色条
		for(i=0;i<160;i++)
		{
			LCD_SetColor( LCD_RED-(i<<16) );
			LCD_DrawLine_V(0+i,  20,10);
		}
		for(i=0;i<240;i++)
		{
			LCD_SetColor( LCD_GREEN-(i<<8) );
			LCD_DrawLine_V(0+i,  30,10);
		}
		for(i=0;i<240;i++)
		{
			LCD_SetColor( LCD_BLUE-i );
			LCD_DrawLine_V(0+i,  40,10);
		}	
		y = 50;
		LCD_SetColor(LIGHT_CYAN);    LCD_FillRect(85,y+5     ,75,10);  LCD_DisplayString(0,y     ,"LIGHT_CYAN");	   
		LCD_SetColor(LIGHT_MAGENTA); LCD_FillRect(105,y+16*1+5,55,10);  LCD_DisplayString(0,y+16*1,"LIGHT_MAGENTA");	

	}
	else  
	{
		if( lcd_panle != panle_114 )
		{
			LCD_SetDirection(Direction_V);
		}

		LCD_DisplayText(0,0,"RGB Color:");

		//使用画线函数绘制三基色色条
		for(i=0;i<240;i++)
		{
			LCD_SetColor( LCD_RED-(i<<16) );
			LCD_DrawLine_V(0+i,  20,10);
		}
		for(i=0;i<240;i++)
		{
			LCD_SetColor( LCD_GREEN-(i<<8) );
			LCD_DrawLine_V(0+i,  35,10);
		}
		for(i=0;i<240;i++)
		{
			LCD_SetColor( LCD_BLUE-i );
			LCD_DrawLine_V(0+i,  50,10);
		}	

		y = 70;
		LCD_SetColor(LIGHT_CYAN);    LCD_FillRect(150,y+5     ,90,10);  LCD_DisplayString(0,y     ,"LIGHT_CYAN");	   
		LCD_SetColor(LIGHT_MAGENTA); LCD_FillRect(150,y+20*1+5,90,10);  LCD_DisplayString(0,y+20*1,"LIGHT_MAGENTA");	
		LCD_SetColor(LIGHT_YELLOW);  LCD_FillRect(150,y+20*2+5,90,10);  LCD_DisplayString(0,y+20*2,"LIGHT_YELLOW");	
		
		if( lcd_panle != panle_114 )
		{
			LCD_SetColor(LIGHT_GREY);    LCD_FillRect(150,y+20*3+5,90,10);  LCD_DisplayString(0,y+20*3,"LIGHT_GREY");  	
			LCD_SetColor(LIGHT_RED);     LCD_FillRect(150,y+20*4+5,90,10);  LCD_DisplayString(0,y+20*4,"LIGHT_RED");  	
			LCD_SetColor(LIGHT_BLUE);    LCD_FillRect(150,y+20*5+5,90,10);  LCD_DisplayString(0,y+20*5,"LIGHT_BLUE");  	

			LCD_SetColor(DARK_CYAN);     LCD_FillRect(150,y+20*6+5,90,10);  LCD_DisplayString(0,y+20*6,"DARK_CYAN");		
			LCD_SetColor(DARK_MAGENTA);  LCD_FillRect(150,y+20*7+5,90,10);  LCD_DisplayString(0,y+20*7,"DARK_MAGENTA");	
			LCD_SetColor(DARK_YELLOW);   LCD_FillRect(150,y+20*8+5,90,10);  LCD_DisplayString(0,y+20*8,"DARK_YELLOW");	
			LCD_SetColor(DARK_GREY);     LCD_FillRect(150,y+20*9+5,90,10);	 LCD_DisplayString(0,y+20*9,"DARK_GREY");	
			LCD_SetColor(DARK_RED);   	  LCD_FillRect(150,y+20*10+5,90,10); LCD_DisplayString(0,y+20*10,"DARK_RED");	
			LCD_SetColor(DARK_GREEN);    LCD_FillRect(150,y+20*11+5,90,10); LCD_DisplayString(0,y+20*11,"DARK_GREEN");	
		}

	}

	osDelay(2000);
}

/*************************************************************************************************
*	函 数 名:	LCD_Test_Image
*
*	函数功能:	图片显示测试
*************************************************************************************************/

void LCD_Test_Image(void)
{
	LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
	LCD_Clear(); 								// 清屏
	LCD_SetDirection(Direction_H_Flip);
	LCD_SetColor( 0xffF6E58D);
	
	if( lcd_panle == panle_096 )
   {
		LCD_DrawImage( 10, 10, 60, 60, Image_Android_60x60) ;	   // 显示图片  

		LCD_SetColor( 0xff9DD3A8);
		LCD_DrawImage( 90, 10, 60, 60, Image_Toys_60x60) ;		// 显示图片		
	}
	else if( lcd_panle == panle_114 )
	{
		LCD_DrawImage( 16, 37, 60, 60, Image_Android_60x60) ;	   // 显示图片

		LCD_SetColor( 0xff9DD3A8);
		LCD_DrawImage( 90, 37, 60, 60, Image_Toys_60x60) ;		// 显示图片
		
		LCD_SetColor( 0xffFF8753);
		LCD_DrawImage( 164, 37, 60, 60, Image_Video_60x60) ;		// 显示图片		
	}
	else  
	{
		LCD_SetDirection(Direction_V);
		LCD_DrawImage( 19, 21, 214,252 , Image_face_256x256) ;	   // 显示图片

//		LCD_SetColor( 0xffDFF9FB);
//		LCD_DrawImage( 141, 21, 83, 83, Image_Message_83x83) ;	// 显示图片
//		
//		LCD_SetColor( 0xff9DD3A8);
//		LCD_DrawImage( 19, 140, 83, 83, Image_Toys_83x83) ;		// 显示图片
//		
//		LCD_SetColor( 0xffFF8753);
//		LCD_DrawImage( 141, 140, 83, 83, Image_Video_83x83) ;		// 显示图片	

	}

	osDelay(2000);	
}



/*************************************************************************************************
*	函 数 名:	LCD_Test_Direction
*
*	函数功能:	更换显示方向
*************************************************************************************************/
void  LCD_Test_Direction(void)
{
	if( lcd_panle == panle_096 )
   {
		LCD_SetDirection(Direction_V);		   
		LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
		LCD_Clear(); 								// 清屏
		LCD_SetTextFont(&CH_Font12);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(5,10,"Direction_V"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 10,50, 60, 60, Image_Android_60x60) ;	   // 显示图片
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(16,112,"反客");
		LCD_DisplayText(16,136,"科技");
		osDelay(1000);	// 延时	

		LCD_SetDirection(Direction_H);		   
		LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
		LCD_Clear(); 								// 清屏
		LCD_SetTextFont(&CH_Font16);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,0,"Direction_H"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 30,20, 60, 60, Image_Android_60x60) ;	   // 显示图片
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(100,16,"反客");
		LCD_DisplayText(100,40,"科技");
		osDelay(1000);	// 延时		

		LCD_SetDirection(Direction_V_Flip);		   
		LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
		LCD_Clear(); 								// 清屏
		LCD_SetTextFont(&CH_Font16);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,10,"_V_Flip"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 10,50, 60, 60, Image_Android_60x60) ;	   // 显示图片
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(16,112,"反客");
		LCD_DisplayText(16,136,"科技");
		osDelay(1000);	// 延时		

		LCD_SetDirection(Direction_H_Flip);		   
		LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
		LCD_Clear(); 								// 清屏
		LCD_SetTextFont(&CH_Font16);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,0,"Direction_H_Flip"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 30,20, 60, 60, Image_Android_60x60) ;	   // 显示图片
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(100,16,"反客");
		LCD_DisplayText(100,40,"科技");
		osDelay(1000);	// 延时			
	}
	else if( lcd_panle == panle_114 )
	{
		LCD_SetDirection(Direction_V);		   
		LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
		LCD_Clear(); 								// 清屏
		LCD_SetTextFont(&CH_Font24);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(0,10,"Direction_V"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 37,50, 60, 60, Image_Android_60x60) ;	   // 显示图片
//		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(35,130,"反客");
		LCD_DisplayText(35,162,"科技");
		osDelay(1000);	// 延时	

		LCD_SetDirection(Direction_H);		   
		LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
		LCD_Clear(); 								// 清屏
		LCD_SetTextFont(&CH_Font24);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,10,"Direction_H"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 30,50, 60, 60, Image_Android_60x60) ;	   // 显示图片
//		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(120,45,"反客");
		LCD_DisplayText(120,77,"科技");
		osDelay(1000);	// 延时		

		LCD_SetDirection(Direction_V_Flip);		   
		LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
		LCD_Clear(); 								// 清屏
		LCD_SetTextFont(&CH_Font16);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(0,10,"Direction_V_Flip"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 37,50, 60, 60, Image_Android_60x60) ;	   // 显示图片
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(35,130,"反客");
		LCD_DisplayText(35,162,"科技");
		osDelay(1000);	// 延时		
		
		LCD_SetDirection(Direction_H_Flip);		   
		LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
		LCD_Clear(); 								// 清屏
		LCD_SetTextFont(&CH_Font24);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,10,"Direction_H_Flip"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 30,50, 60, 60, Image_Android_60x60) ;	   // 显示图片
//		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(120,45,"反客");
		LCD_DisplayText(120,77,"科技");
		osDelay(1000);	// 延时		

	}
	else  
	{
		for(int i=0;i<4;i++)
		{  
			LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
			LCD_Clear(); 								// 清屏
			LCD_SetTextFont(&CH_Font24);  
			LCD_SetColor( 0xffDFF9FB);         
			switch (i)		// 切换背景色
			{
				case 0:  
					LCD_SetDirection(Direction_V);		   
					LCD_DisplayText(20,20,"Direction_V"); 
				break;	

				case 1:  
					LCD_SetDirection(Direction_H); 	
					LCD_DisplayText(20,20,"Direction_H"); 
				break;	

				case 2:  
					LCD_SetDirection(Direction_V_Flip); 
					LCD_DisplayText(20,20,"Direction_V_Flip"); 
				break;
				case 3: 
					LCD_SetDirection(Direction_H_Flip); 	
					LCD_DisplayText(20,20,"Direction_H_Flip"); 
				break;
		
				default:	break;			
			}
			LCD_SetColor( 0xffF6E58D);
			LCD_DrawImage( 19, 80, 83, 83, Image_Android_83x83) ;	   // 显示图片
//			LCD_SetTextFont(&CH_Font32);
			LCD_SetColor( 0xff9DD3A8);  
			LCD_DisplayText(130,90,"反客");
			LCD_DisplayText(130,130,"科技");
	 
			osDelay(1000);	// 延时
		}
	}
}


