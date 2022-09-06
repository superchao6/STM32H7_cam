/***
	************************************************************************************************************************************************************************************************
	*	@file   	lcd_identification.c
	*	@version V1.0
	*  @date    2021-11-23
	*	@author  反客科技
	*	@brief   使用软件模拟SPI，读取屏幕ID以及引脚状态，用来判断是哪个屏幕
   **********************************************************************************************************************************************************************************************
   *  @description
	*
	*	实验平台：反客STM32G070RBT6核心板 （型号：FK-G070M1-RBT6）
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
	*********************************************************************************************************************************************************************************************FANKE*****
***/

#include "lcd_identification.h"

volatile uint8_t lcd_panle = 0;

/****************************************************************************************************************************************
*	函 数 名:	Init_SPI_MspInit
*	
*	函数功能:	初始化 SPI 引脚
****************************************************************************************************************************************/
void Init_SPI_MspInit(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_Init_SCLK_CLK_ENABLE;    // 使能 GPIO 引脚时钟
      GPIO_Init_MOSI_CLK_ENABLE;
      GPIO_Init_CS_CLK_ENABLE;
      GPIO_Init_DC_CLK_ENABLE;         

	 
// 初始化 SCK、MOSI以及片选引脚，使用硬件 SPI 片选
      GPIO_InitStruct.Pin 		   = Init_SCLK_PIN; 
      GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;            		// 复用推挽输出
      GPIO_InitStruct.Pull 		= GPIO_NOPULL;                		// 无上下拉
      GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;  		// 最高速度等级
      HAL_GPIO_Init(Init_SCLK_PORT, &GPIO_InitStruct);
  
      GPIO_InitStruct.Pin 		   = Init_MOSI_PIN; 
      HAL_GPIO_Init(Init_MOSI_PORT, &GPIO_InitStruct);

      GPIO_InitStruct.Pin 		   = Init_CS_PIN; 
      HAL_GPIO_Init(Init_CS_PORT, &GPIO_InitStruct);

// 初始化 数据指令选择 引脚  
		GPIO_InitStruct.Pin 		= Init_DC_PIN;				      // 数据指令选择 引脚
		GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;			// 推挽输出模式
		GPIO_InitStruct.Pull 	= GPIO_NOPULL;						// 无上下拉
		GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;			// 速度等级低
		HAL_GPIO_Init(Init_DC_PORT, &GPIO_InitStruct);	      // 初始化  
		
//// 背光 引脚  
//		GPIO_InitStruct.Pin 		= Init_BLK_PIN;				      // 数据指令选择 引脚
//		GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;			// 推挽输出模式
//		GPIO_InitStruct.Pull 	= GPIO_NOPULL;						// 无上下拉
//		GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;			// 速度等级低
//		HAL_GPIO_Init(Init_BLK_PORT, &GPIO_InitStruct);	      // 初始化  
//				
//		Init_BLK_L;
}

/******************************************************************************
切换 MOSI引脚 为输入
******************************************************************************/
void SPI_MOSI_InPut(void) 
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   GPIO_InitStruct.Pin 		   = Init_MOSI_PIN;                       
   GPIO_InitStruct.Mode 		= GPIO_MODE_INPUT;            		// 输入
   GPIO_InitStruct.Pull 		= GPIO_NOPULL;                		// 无上下拉
   GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;  		// 最高速度等级
   HAL_GPIO_Init(Init_MOSI_PORT, &GPIO_InitStruct);
}

/******************************************************************************
切换 MOSI引脚 为输出
******************************************************************************/
void SPI_MOSI_OutPut(void) 
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   GPIO_InitStruct.Pin 		   = Init_MOSI_PIN;                     
   GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;            		// 推挽输出
   GPIO_InitStruct.Pull 		= GPIO_NOPULL;                		// 无上下拉
   GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;  		// 最高速度等级
   HAL_GPIO_Init(Init_MOSI_PORT, &GPIO_InitStruct);
}

/******************************************************************************
写寄存器，读数据之前，调用此函数
******************************************************************************/
void  Init_WriteReg(uint8_t REG)
{
   uint8_t i;
	Init_DC_L;//写命令

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
	Init_DC_H;//写数据
}
/******************************************************************************
读取8位数据
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
LCD读取ID
******************************************************************************/
uint32_t LCD_ReadID(void)
{
   uint8_t ID[4];
   uint8_t i =0;
   uint32_t  LCD_ID = 0;
// 读取ID1
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

// 读取ID2
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

// 读取ID3
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
模拟SPI，写操作
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
*	函 数 名: Init_WriteCommand
*
*	入口参数: lcd_command - 需要写入的控制指令
*
*	函数功能: 用于向屏幕控制器写入指令
*
****************************************************************************************************************************************/

void  Init_WriteCommand(uint8_t lcd_command)
{
	Init_DC_L;//写命令
	LCD_Writ_Bus(lcd_command);
	Init_DC_H;//写数据
}

/****************************************************************************************************************************************
*	函 数 名: Init_WriteData_8bit
*
*	入口参数: lcd_data - 需要写入的数据，8位
*
*	函数功能: 写入8位数据
*	
****************************************************************************************************************************************/

void  Init_WriteData_8bit(uint8_t lcd_data)
{
   LCD_Writ_Bus(lcd_data);
}

/****************************************************************************************************************************************
*	函 数 名: Init_WriteData_16bit
*
*	入口参数: lcd_data - 需要写入的数据，16位
*
*	函数功能: 写入16位数据
*	
****************************************************************************************************************************************/

void  Init_WriteData_16bit(uint16_t lcd_data)
{
	LCD_Writ_Bus(lcd_data>>8);
	LCD_Writ_Bus(lcd_data);
}


/****************************************************************************************************************************************
*	函 数 名: Init_LCD
*	
****************************************************************************************************************************************/

void Init_LCD(void)
{
   Init_SPI_MspInit();

   osDelay(10);               	// 屏幕刚完成复位时（包括上电复位），需要等待至少5ms才能发送指令

	Init_WriteCommand(0x36);       // 显存访问控制 指令，用于设置访问显存的方式
	Init_WriteData_8bit(0x08);     

	Init_WriteCommand(0x3A);			// 接口像素格式 指令，用于设置使用 12位、16位还是18位色
	Init_WriteData_8bit(0x05);     // 此处配置成 16位 像素格式


	Init_WriteCommand(0x21);       // 打开反显，因为面板是常黑型，操作需要反过来

 // 退出休眠指令，LCD控制器在刚上电、复位时，会自动进入休眠模式 ，因此操作屏幕之前，需要退出休眠  
	Init_WriteCommand(0x11);       // 退出休眠 指令
	osDelay(50);               // 延时等待，让电源电压和时钟电路稳定下来
 
}

//
// 引脚状态切换,将DC和背光引脚设置为输入
void Init_IO_InPut(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   GPIO_Init_BLK_CLK_ENABLE;
   GPIO_Init_DC_CLK_ENABLE;    
	GPIO_Init_CS_CLK_ENABLE;
	
   GPIO_InitStruct.Pin 		   = Init_DC_PIN;                       
   GPIO_InitStruct.Mode 		= GPIO_MODE_INPUT;            		// 输入
   GPIO_InitStruct.Pull 		= GPIO_PULLUP;                		// 上拉
   GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;  		// 最高速度等级
   HAL_GPIO_Init(Init_DC_PORT, &GPIO_InitStruct);

   GPIO_InitStruct.Pull 		= GPIO_NOPULL;          // 无上下拉		
   GPIO_InitStruct.Pin 		   = Init_BLK_PIN;    
   HAL_GPIO_Init(Init_BLK_PORT, &GPIO_InitStruct);

   GPIO_InitStruct.Pull 		= GPIO_PULLUP;          // 上拉	
	GPIO_InitStruct.Pin 		   = Init_CS_PIN;    		// CS
   HAL_GPIO_Init(Init_CS_PORT, &GPIO_InitStruct);
	
}

/****************************************************************************************************************************************
*	函 数 名: 面板识别
*	
****************************************************************************************************************************************/

   void LCD_identification(void)
{
   uint32_t  LCD_ID = 0;

   Init_LCD();             // 初始化SPI引脚、液晶屏
   LCD_ID = LCD_ReadID();  // 读取ID
   Init_IO_InPut();        // 将DC和背光、CS引脚设置为输入

   if( LCD_ID == 0x7c89f0)		// ST7735的ID
   {     
      lcd_panle = panle_096;
   }
   else 	// ST7789
   {
		lcd_panle = panle_130;// 1.3和1.54没有做处理
		
		if( Init_BLK_READ() == GPIO_PIN_SET ) // 1.14寸的屏幕背光为上拉
      {
			lcd_panle = panle_114;
		}
		osDelay(10);	// 需要加个延时，不加的话，不知为何电平读取会出错
		
		if( Init_DC_READ() == GPIO_PIN_RESET ) // 1.69寸的屏幕 DC引脚为下拉
		{  
			lcd_panle = panle_169;
		}
		osDelay(10);	// 需要加个延时，不加的话，不知为何电平读取会出错
		
		if( Init_CS_READ() == GPIO_PIN_RESET ) // 2.0寸的屏幕 CS引脚为下拉
		{
			lcd_panle = panle_200;
		}	
   }
	
	printf("LCD面板识别：%.2X\r\n",lcd_panle);
}

