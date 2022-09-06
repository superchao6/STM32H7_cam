/***
	*************************************************************************************************
	*	@file  	lcd_test.c
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
	*	lcd�������
	*
	************************************************************************************************
***/

#include "lcd_test.h"  

extern RTC_HandleTypeDef hrtc;	// RTC���

extern RTC_TimeTypeDef Time_Struct;		// ����
extern RTC_DateTypeDef Date_Struct;		// ʱ��

/****************************************************************************************************************************************
*	�� �� ��:	 LCD_Progress

*	��������: ������Ч�� 			 
*****************************************************************************************************************************************/

void LCD_Progress(uint16_t time)	
{
	uint8_t	i = 0;			// ��������
	char   Char_Buffer[15];				// ���ڴ洢ת������ַ���
	
	LCD_SetColor(LIGHT_YELLOW);		//	���û���ɫ
	
	if( lcd_panle == panle_096 )
	{
		for(i=0;i<160;i++)
		{
			LCD_DrawLine_V(i,75,5);	// ���ƾ��Σ�ʵ�ּ��׽�������Ч��
			sprintf( Char_Buffer , "RTC-%02d:%02d:%02d",Time_Struct.Hours, Time_Struct.Minutes, Time_Struct.Seconds );	// �� number ת�����ַ�����������ʾ	
			LCD_DisplayString( 10,57,  Char_Buffer);	 				
			osDelay(time);	
		}	
	}	
	else if( lcd_panle == panle_114 )
	{
		for(i=10;i<230;i++)
		{
			LCD_DrawLine_V(i,100,6);	// ���ƾ��Σ�ʵ�ּ��׽�������Ч��
			sprintf( Char_Buffer , "RTC-%02d:%02d:%02d",Time_Struct.Hours, Time_Struct.Minutes, Time_Struct.Seconds );	// �� number ת�����ַ�����������ʾ	
			LCD_DisplayString( 10,70,  Char_Buffer);	 				
			osDelay(time);	
		}	
	}		
	else
	{
		for(i=10;i<230;i++)
		{
			LCD_DrawLine_V(i,200,6);	// ���ƾ��Σ�ʵ�ּ��׽�������Ч��
			sprintf( Char_Buffer , "RTC-%02d:%02d:%02d",Time_Struct.Hours, Time_Struct.Minutes, Time_Struct.Seconds );	// �� number ת�����ַ�����������ʾ	
			LCD_DisplayString( 10,140,  Char_Buffer);	 			
			osDelay(time);	
		}	
	}	
	
	if( Camera_panle != Camera_ERROR)	// �����⵽����ͷ�����л�����ֱ������ʾ����
	{
		LCD_SetDirection(Direction_V_Flip);	
	}

}
/****************************************************************************************************************************************
*	�� �� ��:	 LCD_SetAddress

*	��������: ��ӡ���Խ������Ļ��	 			 
*****************************************************************************************************************************************/

void LCD_Printf(char *pText,int8_t flag)	
{
	static uint16_t y = 0;
	
//���û���ɫ>>>>>>	
	if( flag == 0 )	// �ɹ���־λ0
	{
		LCD_SetColor(LIGHT_GREEN);   	// ��ɫ����   
	}
	else	//����
	{
		LCD_SetColor(LCD_RED);   	// ��ɫ����   
	}	
	
//��������>>>>>>		
	if( (lcd_panle == panle_096)||(lcd_panle == panle_114) )
	{
		LCD_SetTextFont(&CH_Font16);			// ����2424��������,ASCII�����ӦΪ2412	
		y = y+20;
	}
	else
	{
		LCD_SetTextFont(&CH_Font24);			// ����2424��������,ASCII�����ӦΪ2412
		y = y+30;
	}

	LCD_DisplayString(10,y-20,pText);	

}

/*************************************************************************************************
*	�� �� ��:	LCD_Test_Clear
*
*	��������:	��������
*
*************************************************************************************************/

void LCD_Test_Clear(void)
{
	uint32_t	i = 0;			// ��������
	
	LCD_SetDirection(Direction_V);		
	LCD_SetTextFont(&CH_Font24);			// ����2424��������,ASCII�����ӦΪ2412
	
	if( lcd_panle == panle_114 )
   {
		LCD_SetDirection(Direction_H_Flip);		
	}
	else if( lcd_panle == panle_096 )
	{
		LCD_SetDirection(Direction_H_Flip);		
		LCD_SetTextFont(&CH_Font16);			
	}

	
	LCD_SetColor(LCD_BLACK);				// ���û�����ɫ

	for(i=0;i<8;i++)
	{
		LCD_SetBackColor(LCD_WHITE-i*3088960-i*45000-i*400);	// ����һ����ɫ���ڱ���ɫ
		LCD_Clear();		// ����

		if( lcd_panle == panle_096 )
		{
			LCD_DisplayText(13, 5,"STM32H7 ˢ������");
			LCD_DisplayText(13,30,"��Ļ�ֱ���:160*80");
			LCD_DisplayText(13,55,"������:ST7735");	
		}

		else if( lcd_panle == panle_114 )
		{
			LCD_DisplayText(13, 20,"STM32H7 ˢ������");
			LCD_DisplayText(13,56,"��Ļ�ֱ���:240*135");
			LCD_DisplayText(13,92,"������:ST7789");		
		}
		else
		{	
			LCD_DisplayText(13, 70,"STM32H7 ˢ������");
			LCD_DisplayText(13,142,"������:ST7789");	
			if( lcd_panle == panle_130 )
			{
				LCD_DisplayText(13,106,"��Ļ�ֱ���:240*240");
			}	
			else if( lcd_panle == panle_169 )
			{
				LCD_DisplayText(13,106,"��Ļ�ֱ���:240*280");			
			}
			else 
			{
				LCD_DisplayText(13,106,"��Ļ�ֱ���:240*320");	
			}					
		}
			
		osDelay(1000);	// ��ʱ
	}
	
}

/*************************************************************************************************
*	�� �� ��:	LCD_Test_Color
*
*	��������:	��ɫ��
*************************************************************************************************/
void LCD_Test_Color(void)
{
	uint16_t i;					// ��������
	uint16_t y;
// ��ɫ����>>>>>	
	LCD_SetBackColor(LCD_BLACK); //���ñ���ɫ
	LCD_Clear(); //������ˢ����ɫ

	LCD_SetDirection(Direction_H_Flip);
	LCD_SetColor(LCD_WHITE);				// ���û�����ɫ
	LCD_SetTextFont(&CH_Font20);			// ��������
	
	if( lcd_panle == panle_096 )
   {

		LCD_SetTextFont(&CH_Font16);			// ��������

		LCD_DisplayText(0,0,"RGB Color:");

		//ʹ�û��ߺ�����������ɫɫ��
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

		//ʹ�û��ߺ�����������ɫɫ��
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
*	�� �� ��:	LCD_Test_Image
*
*	��������:	ͼƬ��ʾ����
*************************************************************************************************/

void LCD_Test_Image(void)
{
	LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
	LCD_Clear(); 								// ����
	LCD_SetDirection(Direction_H_Flip);
	LCD_SetColor( 0xffF6E58D);
	
	if( lcd_panle == panle_096 )
   {
		LCD_DrawImage( 10, 10, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ  

		LCD_SetColor( 0xff9DD3A8);
		LCD_DrawImage( 90, 10, 60, 60, Image_Toys_60x60) ;		// ��ʾͼƬ		
	}
	else if( lcd_panle == panle_114 )
	{
		LCD_DrawImage( 16, 37, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ

		LCD_SetColor( 0xff9DD3A8);
		LCD_DrawImage( 90, 37, 60, 60, Image_Toys_60x60) ;		// ��ʾͼƬ
		
		LCD_SetColor( 0xffFF8753);
		LCD_DrawImage( 164, 37, 60, 60, Image_Video_60x60) ;		// ��ʾͼƬ		
	}
	else  
	{
		LCD_SetDirection(Direction_V);
		LCD_DrawImage( 19, 21, 214,252 , Image_face_256x256) ;	   // ��ʾͼƬ

//		LCD_SetColor( 0xffDFF9FB);
//		LCD_DrawImage( 141, 21, 83, 83, Image_Message_83x83) ;	// ��ʾͼƬ
//		
//		LCD_SetColor( 0xff9DD3A8);
//		LCD_DrawImage( 19, 140, 83, 83, Image_Toys_83x83) ;		// ��ʾͼƬ
//		
//		LCD_SetColor( 0xffFF8753);
//		LCD_DrawImage( 141, 140, 83, 83, Image_Video_83x83) ;		// ��ʾͼƬ	

	}

	osDelay(2000);	
}



/*************************************************************************************************
*	�� �� ��:	LCD_Test_Direction
*
*	��������:	������ʾ����
*************************************************************************************************/
void  LCD_Test_Direction(void)
{
	if( lcd_panle == panle_096 )
   {
		LCD_SetDirection(Direction_V);		   
		LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
		LCD_Clear(); 								// ����
		LCD_SetTextFont(&CH_Font12);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(5,10,"Direction_V"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 10,50, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(16,112,"����");
		LCD_DisplayText(16,136,"�Ƽ�");
		osDelay(1000);	// ��ʱ	

		LCD_SetDirection(Direction_H);		   
		LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
		LCD_Clear(); 								// ����
		LCD_SetTextFont(&CH_Font16);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,0,"Direction_H"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 30,20, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(100,16,"����");
		LCD_DisplayText(100,40,"�Ƽ�");
		osDelay(1000);	// ��ʱ		

		LCD_SetDirection(Direction_V_Flip);		   
		LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
		LCD_Clear(); 								// ����
		LCD_SetTextFont(&CH_Font16);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,10,"_V_Flip"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 10,50, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(16,112,"����");
		LCD_DisplayText(16,136,"�Ƽ�");
		osDelay(1000);	// ��ʱ		

		LCD_SetDirection(Direction_H_Flip);		   
		LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
		LCD_Clear(); 								// ����
		LCD_SetTextFont(&CH_Font16);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,0,"Direction_H_Flip"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 30,20, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(100,16,"����");
		LCD_DisplayText(100,40,"�Ƽ�");
		osDelay(1000);	// ��ʱ			
	}
	else if( lcd_panle == panle_114 )
	{
		LCD_SetDirection(Direction_V);		   
		LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
		LCD_Clear(); 								// ����
		LCD_SetTextFont(&CH_Font24);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(0,10,"Direction_V"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 37,50, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ
//		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(35,130,"����");
		LCD_DisplayText(35,162,"�Ƽ�");
		osDelay(1000);	// ��ʱ	

		LCD_SetDirection(Direction_H);		   
		LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
		LCD_Clear(); 								// ����
		LCD_SetTextFont(&CH_Font24);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,10,"Direction_H"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 30,50, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ
//		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(120,45,"����");
		LCD_DisplayText(120,77,"�Ƽ�");
		osDelay(1000);	// ��ʱ		

		LCD_SetDirection(Direction_V_Flip);		   
		LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
		LCD_Clear(); 								// ����
		LCD_SetTextFont(&CH_Font16);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(0,10,"Direction_V_Flip"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 37,50, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ
		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(35,130,"����");
		LCD_DisplayText(35,162,"�Ƽ�");
		osDelay(1000);	// ��ʱ		
		
		LCD_SetDirection(Direction_H_Flip);		   
		LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
		LCD_Clear(); 								// ����
		LCD_SetTextFont(&CH_Font24);  
		LCD_SetColor( 0xffDFF9FB);  	
		LCD_DisplayText(20,10,"Direction_H_Flip"); 	
		LCD_SetColor( 0xffF6E58D);
		LCD_DrawImage( 30,50, 60, 60, Image_Android_60x60) ;	   // ��ʾͼƬ
//		LCD_SetTextFont(&CH_Font24);
		LCD_SetColor( 0xff9DD3A8);  
		LCD_DisplayText(120,45,"����");
		LCD_DisplayText(120,77,"�Ƽ�");
		osDelay(1000);	// ��ʱ		

	}
	else  
	{
		for(int i=0;i<4;i++)
		{  
			LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
			LCD_Clear(); 								// ����
			LCD_SetTextFont(&CH_Font24);  
			LCD_SetColor( 0xffDFF9FB);         
			switch (i)		// �л�����ɫ
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
			LCD_DrawImage( 19, 80, 83, 83, Image_Android_83x83) ;	   // ��ʾͼƬ
//			LCD_SetTextFont(&CH_Font32);
			LCD_SetColor( 0xff9DD3A8);  
			LCD_DisplayText(130,90,"����");
			LCD_DisplayText(130,130,"�Ƽ�");
	 
			osDelay(1000);	// ��ʱ
		}
	}
}


