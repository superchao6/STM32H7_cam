/***
	************************************************************************************************************************************************************************************************
	*	@file   	lcd_identification.c
	*	@version V1.0
	*  @date    2021-11-23
	*	@author  ���ͿƼ�
	*	@brief   ʹ�����ģ��SPI����ȡ��ĻID�Լ�����״̬�������ж����ĸ���Ļ
   **********************************************************************************************************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨������STM32G070RBT6���İ� ���ͺţ�FK-G070M1-RBT6��
	*	�Ա���ַ��https://shop212360197.taobao.com
	*	QQ����Ⱥ��536665479
	*
	*********************************************************************************************************************************************************************************************FANKE*****
***/

#include "lcd_identification.h"

volatile uint8_t lcd_panle = 0;

/****************************************************************************************************************************************
*	�� �� ��:	Init_SPI_MspInit
*	
*	��������:	��ʼ�� SPI ����
****************************************************************************************************************************************/
void Init_SPI_MspInit(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_Init_SCLK_CLK_ENABLE;    // ʹ�� GPIO ����ʱ��
      GPIO_Init_MOSI_CLK_ENABLE;
      GPIO_Init_CS_CLK_ENABLE;
      GPIO_Init_DC_CLK_ENABLE;         

	 
// ��ʼ�� SCK��MOSI�Լ�Ƭѡ���ţ�ʹ��Ӳ�� SPI Ƭѡ
      GPIO_InitStruct.Pin 		   = Init_SCLK_PIN; 
      GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;            		// �����������
      GPIO_InitStruct.Pull 		= GPIO_NOPULL;                		// ��������
      GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;  		// ����ٶȵȼ�
      HAL_GPIO_Init(Init_SCLK_PORT, &GPIO_InitStruct);
  
      GPIO_InitStruct.Pin 		   = Init_MOSI_PIN; 
      HAL_GPIO_Init(Init_MOSI_PORT, &GPIO_InitStruct);

      GPIO_InitStruct.Pin 		   = Init_CS_PIN; 
      HAL_GPIO_Init(Init_CS_PORT, &GPIO_InitStruct);

// ��ʼ�� ����ָ��ѡ�� ����  
		GPIO_InitStruct.Pin 		= Init_DC_PIN;				      // ����ָ��ѡ�� ����
		GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;			// �������ģʽ
		GPIO_InitStruct.Pull 	= GPIO_NOPULL;						// ��������
		GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;			// �ٶȵȼ���
		HAL_GPIO_Init(Init_DC_PORT, &GPIO_InitStruct);	      // ��ʼ��  
		
//// ���� ����  
//		GPIO_InitStruct.Pin 		= Init_BLK_PIN;				      // ����ָ��ѡ�� ����
//		GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;			// �������ģʽ
//		GPIO_InitStruct.Pull 	= GPIO_NOPULL;						// ��������
//		GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;			// �ٶȵȼ���
//		HAL_GPIO_Init(Init_BLK_PORT, &GPIO_InitStruct);	      // ��ʼ��  
//				
//		Init_BLK_L;
}

/******************************************************************************
�л� MOSI���� Ϊ����
******************************************************************************/
void SPI_MOSI_InPut(void) 
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   GPIO_InitStruct.Pin 		   = Init_MOSI_PIN;                       
   GPIO_InitStruct.Mode 		= GPIO_MODE_INPUT;            		// ����
   GPIO_InitStruct.Pull 		= GPIO_NOPULL;                		// ��������
   GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;  		// ����ٶȵȼ�
   HAL_GPIO_Init(Init_MOSI_PORT, &GPIO_InitStruct);
}

/******************************************************************************
�л� MOSI���� Ϊ���
******************************************************************************/
void SPI_MOSI_OutPut(void) 
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   GPIO_InitStruct.Pin 		   = Init_MOSI_PIN;                     
   GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;            		// �������
   GPIO_InitStruct.Pull 		= GPIO_NOPULL;                		// ��������
   GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;  		// ����ٶȵȼ�
   HAL_GPIO_Init(Init_MOSI_PORT, &GPIO_InitStruct);
}

/******************************************************************************
д�Ĵ�����������֮ǰ�����ô˺���
******************************************************************************/
void  Init_WriteReg(uint8_t REG)
{
   uint8_t i;
	Init_DC_L;//д����

	for(i=0;i<8;i++)
	{			  
		Init_SCLK_L;
		if(REG&0x80)
		{
		   Init_MOSI_H;
		}
		else
		{
		   Init_MOSI_L;
		}
		Init_SCLK_H;
		REG<<=1;
	}	
	Init_DC_H;//д����
}
/******************************************************************************
��ȡ8λ����
******************************************************************************/
uint8_t LCD_Read_Bus(void) 
{	
   uint8_t i;
   uint8_t dat = 0x00;

	for(i=0;i<8;i++)
	{			  
		Init_SCLK_L;
      Init_SCLK_H;
		dat<<=1;		
		if(Init_MOSI_READ() == GPIO_PIN_SET)
		{
		   dat++;
		}
	}	
   return dat;
}

/******************************************************************************
LCD��ȡID
******************************************************************************/
uint32_t LCD_ReadID(void)
{
   uint8_t ID[4];
   uint8_t i =0;
   uint32_t  LCD_ID = 0;
// ��ȡID1
   Init_CS_L;
   Init_WriteReg(0xDA); 
   SPI_MOSI_InPut();
   for ( i = 0; i < 2; i++)
   {
      ID[i]=LCD_Read_Bus();
   }
   Init_CS_H;	
   LCD_ID = ID[0]<<16;
   // printf("ID%x,%x,\r\n", ID[0],ID[1]);

// ��ȡID2
   SPI_MOSI_OutPut() ;
   Init_CS_L;
   Init_WriteReg(0xDB); 
   SPI_MOSI_InPut();
   for ( i = 0; i < 2; i++)
   {
      ID[i]=LCD_Read_Bus();
   }
   Init_CS_H;	
   LCD_ID |= (ID[0]<<8);
   // printf("ID%x,%x,\r\n", ID[0],ID[1]);

// ��ȡID3
   SPI_MOSI_OutPut() ;
   Init_CS_L;
   Init_WriteReg(0xDC); 
   SPI_MOSI_InPut();
   for ( i = 0; i < 2; i++)
   {
      ID[i]=LCD_Read_Bus();
   }
   Init_CS_H;	
   LCD_ID |= ID[0];
   // printf("ID%x,%x,\r\n", ID[0],ID[1]);

   // SPI_MOSI_OutPut() ;

   return LCD_ID;
}

/******************************************************************************
ģ��SPI��д����
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	uint8_t i;

   Init_CS_L;

   for(i=0;i<8;i++)
   {			  
      Init_SCLK_L;
      if(dat&0x80)
      {
         Init_MOSI_H;
      }
      else
      {
         Init_MOSI_L;
      }
      Init_SCLK_H;
      dat<<=1;
   }	
   Init_CS_H;	
}

/****************************************************************************************************************************************
*	�� �� ��: Init_WriteCommand
*
*	��ڲ���: lcd_command - ��Ҫд��Ŀ���ָ��
*
*	��������: ��������Ļ������д��ָ��
*
****************************************************************************************************************************************/

void  Init_WriteCommand(uint8_t lcd_command)
{
	Init_DC_L;//д����
	LCD_Writ_Bus(lcd_command);
	Init_DC_H;//д����
}

/****************************************************************************************************************************************
*	�� �� ��: Init_WriteData_8bit
*
*	��ڲ���: lcd_data - ��Ҫд������ݣ�8λ
*
*	��������: д��8λ����
*	
****************************************************************************************************************************************/

void  Init_WriteData_8bit(uint8_t lcd_data)
{
   LCD_Writ_Bus(lcd_data);
}

/****************************************************************************************************************************************
*	�� �� ��: Init_WriteData_16bit
*
*	��ڲ���: lcd_data - ��Ҫд������ݣ�16λ
*
*	��������: д��16λ����
*	
****************************************************************************************************************************************/

void  Init_WriteData_16bit(uint16_t lcd_data)
{
	LCD_Writ_Bus(lcd_data>>8);
	LCD_Writ_Bus(lcd_data);
}


/****************************************************************************************************************************************
*	�� �� ��: Init_LCD
*	
****************************************************************************************************************************************/

void Init_LCD(void)
{
   Init_SPI_MspInit();

   osDelay(10);               	// ��Ļ����ɸ�λʱ�������ϵ縴λ������Ҫ�ȴ�����5ms���ܷ���ָ��

	Init_WriteCommand(0x36);       // �Դ���ʿ��� ָ��������÷����Դ�ķ�ʽ
	Init_WriteData_8bit(0x08);     

	Init_WriteCommand(0x3A);			// �ӿ����ظ�ʽ ָ���������ʹ�� 12λ��16λ����18λɫ
	Init_WriteData_8bit(0x05);     // �˴����ó� 16λ ���ظ�ʽ


	Init_WriteCommand(0x21);       // �򿪷��ԣ���Ϊ����ǳ����ͣ�������Ҫ������

 // �˳�����ָ�LCD�������ڸ��ϵ硢��λʱ�����Զ���������ģʽ ����˲�����Ļ֮ǰ����Ҫ�˳�����  
	Init_WriteCommand(0x11);       // �˳����� ָ��
	osDelay(50);               // ��ʱ�ȴ����õ�Դ��ѹ��ʱ�ӵ�·�ȶ�����
 
}

//
// ����״̬�л�,��DC�ͱ�����������Ϊ����
void Init_IO_InPut(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   GPIO_Init_BLK_CLK_ENABLE;
   GPIO_Init_DC_CLK_ENABLE;    
	GPIO_Init_CS_CLK_ENABLE;
	
   GPIO_InitStruct.Pin 		   = Init_DC_PIN;                       
   GPIO_InitStruct.Mode 		= GPIO_MODE_INPUT;            		// ����
   GPIO_InitStruct.Pull 		= GPIO_PULLUP;                		// ����
   GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;  		// ����ٶȵȼ�
   HAL_GPIO_Init(Init_DC_PORT, &GPIO_InitStruct);

   GPIO_InitStruct.Pull 		= GPIO_NOPULL;          // ��������		
   GPIO_InitStruct.Pin 		   = Init_BLK_PIN;    
   HAL_GPIO_Init(Init_BLK_PORT, &GPIO_InitStruct);

   GPIO_InitStruct.Pull 		= GPIO_PULLUP;          // ����	
	GPIO_InitStruct.Pin 		   = Init_CS_PIN;    		// CS
   HAL_GPIO_Init(Init_CS_PORT, &GPIO_InitStruct);
	
}

/****************************************************************************************************************************************
*	�� �� ��: ���ʶ��
*	
****************************************************************************************************************************************/

   void LCD_identification(void)
{
   uint32_t  LCD_ID = 0;

   Init_LCD();             // ��ʼ��SPI���š�Һ����
   LCD_ID = LCD_ReadID();  // ��ȡID
   Init_IO_InPut();        // ��DC�ͱ��⡢CS��������Ϊ����

   if( LCD_ID == 0x7c89f0)		// ST7735��ID
   {     
      lcd_panle = panle_096;
   }
   else 	// ST7789
   {
		lcd_panle = panle_130;// 1.3��1.54û��������
		
		if( Init_BLK_READ() == GPIO_PIN_SET ) // 1.14�����Ļ����Ϊ����
      {
			lcd_panle = panle_114;
		}
		osDelay(10);	// ��Ҫ�Ӹ���ʱ�����ӵĻ�����֪Ϊ�ε�ƽ��ȡ�����
		
		if( Init_DC_READ() == GPIO_PIN_RESET ) // 1.69�����Ļ DC����Ϊ����
		{  
			lcd_panle = panle_169;
		}
		osDelay(10);	// ��Ҫ�Ӹ���ʱ�����ӵĻ�����֪Ϊ�ε�ƽ��ȡ�����
		
		if( Init_CS_READ() == GPIO_PIN_RESET ) // 2.0�����Ļ CS����Ϊ����
		{
			lcd_panle = panle_200;
		}	
   }
	
	printf("LCD���ʶ��%.2X\r\n",lcd_panle);
}

