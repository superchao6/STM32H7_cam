/***
	*************************************************************************************************
	*	@file  	memory_test.c
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
	*	W25Q64和SD卡测试函数
	*
	************************************************************************************************
***/

#include "memory_test.h"  



/****************************************** OSPI W25Q64测试 ******************************************/

#define W25Qxx_NumByteToTest   	32*1024					// 测试数据的长度，64K

int32_t OSPI_Status ; 		 //检测标志位

uint32_t W25Qxx_TestAddr  =	0	;							// 测试地址
uint8_t  W25Qxx_WriteBuffer[W25Qxx_NumByteToTest];		//	写数据数组
uint8_t  W25Qxx_ReadBuffer[W25Qxx_NumByteToTest];		//	读数据数组


int8_t OSPI_W25Qxx_Test(void)		//Flash读写测试
{
	uint32_t i = 0;	// 计数变量
	
	printf ("\r\n**************************** W25Q64测试 **************************** \r\n");	

// 擦除 >>>>>>>    
	
	OSPI_Status = OSPI_W25Qxx_BlockErase_32K(W25Qxx_TestAddr);	// 擦除32K字节

	if( OSPI_Status == OSPI_W25Qxx_OK )
	{
		printf ("W25Q64 擦除成功>>>\r\n");		
	}
	else
	{
		printf ("擦除失败!!!!!  错误代码:%d\r\n",OSPI_Status);
		return OSPI_Status ;
	}	

// 写入 >>>>>>>    

	for(i=0;i<W25Qxx_NumByteToTest;i++)  //先将数据写入数组
	{
		W25Qxx_WriteBuffer[i] = i;
	}

	OSPI_Status				= OSPI_W25Qxx_WriteBuffer(W25Qxx_WriteBuffer,W25Qxx_TestAddr,W25Qxx_NumByteToTest); // 写入数据

	if( OSPI_Status == OSPI_W25Qxx_OK )
	{
		printf ("写入成功>>>\r\n");		
	}
	else
	{
		printf ("写入错误!!!!!  错误代码:%d\r\n",OSPI_Status);
		return OSPI_Status ;
	}		
	
// 读取 >>>>>>>    

	OSPI_Status = OSPI_W25Qxx_MemoryMappedMode(); // 配置OSPI为内存映射模式
	if( OSPI_Status == OSPI_W25Qxx_OK )
	{
		printf ("进入内存映射模式成功，开始读取>>>>\r\n");		
	}
	else
	{
		printf ("内存映射错误！！  错误代码:%d\r\n",OSPI_Status);
		return OSPI_Status ;
	}			

	memcpy(W25Qxx_ReadBuffer,(uint8_t *)W25Qxx_Mem_Addr+W25Qxx_TestAddr,W25Qxx_NumByteToTest);  // 从 OSPI_Mem_Addr +W25Qxx_TestAddr 地址处，拷贝数据到 W25Qxx_ReadBuffer

	if( OSPI_Status == OSPI_W25Qxx_OK )
	{
		printf ("读取成功>>>\r\n");		
	}
	else
	{
		printf ("读取错误!!!!!  错误代码:%d\r\n",OSPI_Status);
		return OSPI_Status ;
	}			
	
// 数据校验 >>>>>>>    
	
	for(i=0;i<W25Qxx_NumByteToTest;i++)	//验证读出的数据是否等于写入的数据
	{
		if( W25Qxx_WriteBuffer[i] != W25Qxx_ReadBuffer[i] )	//如果数据不相等，则返回0	
		{
			printf ("数据校验失败!!!!!\r\n");	
			return W25Qxx_ERROR_TRANSMIT ;
		}
	}			
	printf ("校验通过!!!!! OSPI驱动W25Q64测试正常\r\n");		
	

	LCD_Printf(	"OSPI 25Q64 OK >>>",OSPI_W25Qxx_OK);	// 显示结果到屏幕
	
	return OSPI_W25Qxx_OK ;  // 测试通过	
}



/********************************************** SD卡测试 *******************************************/

#define	NumOf_Blocks	64
#define 	Test_BlockSize  ((BLOCKSIZE * NumOf_Blocks) >> 2)	 //定义数据大小,SD块大小为512字节，因为是32位的数组，所以这里除以4
#define	Test_Addr 	 0x00


int32_t SD_Status ; 		 //SD卡检测标志位


// 函数：SD卡测试
//	功能：SD卡块读写测试
//	返回值：BSP_ERROR_NONE - 读写测试成功
//
int8_t SD_Test(void)
{
	uint32_t i = 0;	

	printf ("\r\n**************************** SD卡测试 **************************** \r\n");	

	if( SD_Status == BSP_ERROR_NONE )	//检测是否初始化成功
	{		
		printf("SD卡初始化成功 \r\n");		
	}
	else
	{
		printf("检测不到SD卡\r\n");	
		return BSP_ERROR_NO_INIT;		// 退出
	}	
	
// 擦除 >>>>>>>    
		
	SD_Status = BSP_SD_Erase(SD_Instance,Test_Addr, NumOf_Blocks);
	while(BSP_SD_GetCardState(SD_Instance) != SD_TRANSFER_OK);	//等待通信结束	

	if( SD_Status == BSP_ERROR_NONE )
	{
		printf ("SD卡擦除成功>>>\r\n");		
	}
	else
	{
		printf ("擦除失败!!!!!  错误代码:%d\r\n",SD_Status);
		return BSP_ERROR_NO_INIT;
	}		
	
// 写入 >>>>>>>    
	for(i=0;i<Test_BlockSize;i++)	//将要写入SD卡的数据写入数组
	{
		W25Qxx_WriteBuffer[i] = i;
	}
	SD_Status = BSP_SD_WriteBlocks(SD_Instance,(uint32_t *)W25Qxx_WriteBuffer, Test_Addr, NumOf_Blocks);	//块写入	
	while(BSP_SD_GetCardState(SD_Instance) != SD_TRANSFER_OK);	//等待通信结束	
	if( SD_Status == BSP_ERROR_NONE )
	{
		printf ("写入成功>>>\r\n");		
	}
	else
	{
		printf ("写入错误!!!!!  错误代码:%d\r\n",SD_Status);
		return BSP_ERROR_NO_INIT;
	}		
	
	
// 读取 >>>>>>>    
	SD_Status = BSP_SD_ReadBlocks(SD_Instance,(uint32_t *)W25Qxx_ReadBuffer, Test_Addr, NumOf_Blocks);	//块读取
	while(BSP_SD_GetCardState(SD_Instance) != SD_TRANSFER_OK);	//等待通信结束	

	if( SD_Status == BSP_ERROR_NONE )
	{
		printf ("读取成功>>> \r\n");		
	}
	else
	{
		printf ("读取错误!!!!!  错误代码:%d\r\n",SD_Status);
		return BSP_ERROR_NO_INIT;
	}		

// 校验 >>>>>>>   
	for(i=0;i<Test_BlockSize;i++)	//验证读出的数据是否等于写入的数据
	{
		if( W25Qxx_WriteBuffer[i] != W25Qxx_ReadBuffer[i] )	//如果数据不相等，则返回0	
		{
			printf ("数据校验失败!!!!!\r\n");	
			return BSP_ERROR_NO_INIT;
		}
	}		
	printf ("校验通过!!!!!SD卡测试正常\r\n");		
	
	LCD_Printf(	"SD Card OK >>>",BSP_ERROR_NONE);
	
	return BSP_ERROR_NONE; 	//数据正确，读写测试通过
}

