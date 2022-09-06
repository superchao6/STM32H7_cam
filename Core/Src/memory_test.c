/***
	*************************************************************************************************
	*	@file  	memory_test.c
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
	*	W25Q64��SD�����Ժ���
	*
	************************************************************************************************
***/

#include "memory_test.h"  



/****************************************** OSPI W25Q64���� ******************************************/

#define W25Qxx_NumByteToTest   	32*1024					// �������ݵĳ��ȣ�64K

int32_t OSPI_Status ; 		 //����־λ

uint32_t W25Qxx_TestAddr  =	0	;							// ���Ե�ַ
uint8_t  W25Qxx_WriteBuffer[W25Qxx_NumByteToTest];		//	д��������
uint8_t  W25Qxx_ReadBuffer[W25Qxx_NumByteToTest];		//	����������


int8_t OSPI_W25Qxx_Test(void)		//Flash��д����
{
	uint32_t i = 0;	// ��������
	
	printf ("\r\n**************************** W25Q64���� **************************** \r\n");	

// ���� >>>>>>>    
	
	OSPI_Status = OSPI_W25Qxx_BlockErase_32K(W25Qxx_TestAddr);	// ����32K�ֽ�

	if( OSPI_Status == OSPI_W25Qxx_OK )
	{
		printf ("W25Q64 �����ɹ�>>>\r\n");		
	}
	else
	{
		printf ("����ʧ��!!!!!  �������:%d\r\n",OSPI_Status);
		return OSPI_Status ;
	}	

// д�� >>>>>>>    

	for(i=0;i<W25Qxx_NumByteToTest;i++)  //�Ƚ�����д������
	{
		W25Qxx_WriteBuffer[i] = i;
	}

	OSPI_Status				= OSPI_W25Qxx_WriteBuffer(W25Qxx_WriteBuffer,W25Qxx_TestAddr,W25Qxx_NumByteToTest); // д������

	if( OSPI_Status == OSPI_W25Qxx_OK )
	{
		printf ("д��ɹ�>>>\r\n");		
	}
	else
	{
		printf ("д�����!!!!!  �������:%d\r\n",OSPI_Status);
		return OSPI_Status ;
	}		
	
// ��ȡ >>>>>>>    

	OSPI_Status = OSPI_W25Qxx_MemoryMappedMode(); // ����OSPIΪ�ڴ�ӳ��ģʽ
	if( OSPI_Status == OSPI_W25Qxx_OK )
	{
		printf ("�����ڴ�ӳ��ģʽ�ɹ�����ʼ��ȡ>>>>\r\n");		
	}
	else
	{
		printf ("�ڴ�ӳ����󣡣�  �������:%d\r\n",OSPI_Status);
		return OSPI_Status ;
	}			

	memcpy(W25Qxx_ReadBuffer,(uint8_t *)W25Qxx_Mem_Addr+W25Qxx_TestAddr,W25Qxx_NumByteToTest);  // �� OSPI_Mem_Addr +W25Qxx_TestAddr ��ַ�����������ݵ� W25Qxx_ReadBuffer

	if( OSPI_Status == OSPI_W25Qxx_OK )
	{
		printf ("��ȡ�ɹ�>>>\r\n");		
	}
	else
	{
		printf ("��ȡ����!!!!!  �������:%d\r\n",OSPI_Status);
		return OSPI_Status ;
	}			
	
// ����У�� >>>>>>>    
	
	for(i=0;i<W25Qxx_NumByteToTest;i++)	//��֤�����������Ƿ����д�������
	{
		if( W25Qxx_WriteBuffer[i] != W25Qxx_ReadBuffer[i] )	//������ݲ���ȣ��򷵻�0	
		{
			printf ("����У��ʧ��!!!!!\r\n");	
			return W25Qxx_ERROR_TRANSMIT ;
		}
	}			
	printf ("У��ͨ��!!!!! OSPI����W25Q64��������\r\n");		
	

	LCD_Printf(	"OSPI 25Q64 OK >>>",OSPI_W25Qxx_OK);	// ��ʾ�������Ļ
	
	return OSPI_W25Qxx_OK ;  // ����ͨ��	
}



/********************************************** SD������ *******************************************/

#define	NumOf_Blocks	64
#define 	Test_BlockSize  ((BLOCKSIZE * NumOf_Blocks) >> 2)	 //�������ݴ�С,SD���СΪ512�ֽڣ���Ϊ��32λ�����飬�����������4
#define	Test_Addr 	 0x00


int32_t SD_Status ; 		 //SD������־λ


// ������SD������
//	���ܣ�SD�����д����
//	����ֵ��BSP_ERROR_NONE - ��д���Գɹ�
//
int8_t SD_Test(void)
{
	uint32_t i = 0;	

	printf ("\r\n**************************** SD������ **************************** \r\n");	

	if( SD_Status == BSP_ERROR_NONE )	//����Ƿ��ʼ���ɹ�
	{		
		printf("SD����ʼ���ɹ� \r\n");		
	}
	else
	{
		printf("��ⲻ��SD��\r\n");	
		return BSP_ERROR_NO_INIT;		// �˳�
	}	
	
// ���� >>>>>>>    
		
	SD_Status = BSP_SD_Erase(SD_Instance,Test_Addr, NumOf_Blocks);
	while(BSP_SD_GetCardState(SD_Instance) != SD_TRANSFER_OK);	//�ȴ�ͨ�Ž���	

	if( SD_Status == BSP_ERROR_NONE )
	{
		printf ("SD�������ɹ�>>>\r\n");		
	}
	else
	{
		printf ("����ʧ��!!!!!  �������:%d\r\n",SD_Status);
		return BSP_ERROR_NO_INIT;
	}		
	
// д�� >>>>>>>    
	for(i=0;i<Test_BlockSize;i++)	//��Ҫд��SD��������д������
	{
		W25Qxx_WriteBuffer[i] = i;
	}
	SD_Status = BSP_SD_WriteBlocks(SD_Instance,(uint32_t *)W25Qxx_WriteBuffer, Test_Addr, NumOf_Blocks);	//��д��	
	while(BSP_SD_GetCardState(SD_Instance) != SD_TRANSFER_OK);	//�ȴ�ͨ�Ž���	
	if( SD_Status == BSP_ERROR_NONE )
	{
		printf ("д��ɹ�>>>\r\n");		
	}
	else
	{
		printf ("д�����!!!!!  �������:%d\r\n",SD_Status);
		return BSP_ERROR_NO_INIT;
	}		
	
	
// ��ȡ >>>>>>>    
	SD_Status = BSP_SD_ReadBlocks(SD_Instance,(uint32_t *)W25Qxx_ReadBuffer, Test_Addr, NumOf_Blocks);	//���ȡ
	while(BSP_SD_GetCardState(SD_Instance) != SD_TRANSFER_OK);	//�ȴ�ͨ�Ž���	

	if( SD_Status == BSP_ERROR_NONE )
	{
		printf ("��ȡ�ɹ�>>> \r\n");		
	}
	else
	{
		printf ("��ȡ����!!!!!  �������:%d\r\n",SD_Status);
		return BSP_ERROR_NO_INIT;
	}		

// У�� >>>>>>>   
	for(i=0;i<Test_BlockSize;i++)	//��֤�����������Ƿ����д�������
	{
		if( W25Qxx_WriteBuffer[i] != W25Qxx_ReadBuffer[i] )	//������ݲ���ȣ��򷵻�0	
		{
			printf ("����У��ʧ��!!!!!\r\n");	
			return BSP_ERROR_NO_INIT;
		}
	}		
	printf ("У��ͨ��!!!!!SD����������\r\n");		
	
	LCD_Printf(	"SD Card OK >>>",BSP_ERROR_NONE);
	
	return BSP_ERROR_NONE; 	//������ȷ����д����ͨ��
}

