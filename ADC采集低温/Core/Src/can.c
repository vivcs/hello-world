/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
CAN_TxHeaderTypeDef        TxMessage;    //CAN发送的消息的消息头
CAN_RxHeaderTypeDef        RxMessage;    //CAN接收的消息的消息头



/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;//更改的初始化以下都是
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_5TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = ENABLE;
  hcan1.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */



#define FilterID		0x6420

/*CAN筛选器的配置*/
static void CAN_Filter_Cogfig(void)
{
	
  CAN_FilterTypeDef hcan1_filter;	
	
	/* 接收特定ID号时取消注释，可以将ID号进行宏定义后自己修改ID号 */
  hcan1_filter.FilterIdHigh = ((((uint32_t)FilterID<<3)|CAN_ID_EXT|CAN_RTR_DATA) & 0xFFFF0000)>>16;  //要筛选的ID高16位

  hcan1_filter.FilterIdLow = ((((uint32_t)FilterID<<3)|CAN_ID_EXT|CAN_RTR_DATA) & 0xFFFF);  //要筛选的ID低16位

  hcan1_filter.FilterMaskIdHigh = 0xFFFF;  

  hcan1_filter.FilterMaskIdLow = 0xFFFF;  //设置筛选器的每一位都要匹配
	
	
	
	
	/* 接收任何ID号时取消注释 */
//	 hcan1_filter.FilterIdHigh = 0x0000;  //要筛选的ID高16位

//  hcan1_filter.FilterIdLow = 0x0000;  //要筛选的ID低16位

//  hcan1_filter.FilterMaskIdHigh = 0x0000;  

//  hcan1_filter.FilterMaskIdLow = 0x0000;  //设置筛选器任何ID都匹配




  hcan1_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0; //关联到FIFO0

  hcan1_filter.FilterBank = 1; 

  hcan1_filter.FilterMode = CAN_FILTERMODE_IDMASK ;

  hcan1_filter.FilterScale = CAN_FILTERSCALE_32BIT;

  hcan1_filter.FilterActivation = CAN_FILTER_ENABLE ;
		
  hcan1_filter.SlaveStartFilterBank = 14;
	
	HAL_CAN_ConfigFilter(&hcan1, &hcan1_filter);
  
  
}

//对CAN的初始化函数：I/O口，筛选器，使能中断
void Can_ConfigInit(void)
{
	
	
	MX_CAN1_Init();
	
	CAN_Filter_Cogfig();

	HAL_Delay(100);
	
	HAL_CAN_Start(&hcan1);
	
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	
}


//CAN的发送函数
void CAN_Send(void)
{
	HAL_StatusTypeDef status;
	uint8_t FreeTxNum = 0;
  uint32_t *pTxMailbox;
	uint8_t  temp[8]={1,2,5,7,5,4,6,3};
	
  TxMessage. StdId = 0x13; 
  TxMessage. ExtId = 0x64200000; //使用扩展ID
  TxMessage. IDE = CAN_ID_EXT ;//使用扩展模式
  TxMessage. RTR = CAN_RTR_DATA ; //发送数据帧
  TxMessage. DLC = 0x08;			//数据长度8
  TxMessage. TransmitGlobalTime = DISABLE;
	
	FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
	while(FreeTxNum == 0)FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);

		status = HAL_CAN_AddTxMessage(&hcan1,&TxMessage,(uint8_t *)temp,pTxMailbox);
		if(status ==HAL_OK)
	{	
		printf("数据发送成功 -> 0x%x\r\n",status);
	}
	else
		printf ("数据发送失败-> 0x%x\r\n",status);
} 


/*CAN发送由ADC采集到的数据*/
//uint8_t CAN1_Tx_data[8];
//uint16_t ADC_ConvertedValue[Temp_Sample_Num];


void CAN_Send_ADCvalue(void)
{
	
	HAL_StatusTypeDef status;
	uint8_t FreeTxNum = 0;
  uint32_t *pTxMailbox;
	
  TxMessage. StdId = 0x13; 
  TxMessage. ExtId = 0x1314; //使用扩展ID
  TxMessage. IDE = CAN_ID_EXT ;//使用扩展模式
  TxMessage. RTR = CAN_RTR_DATA ; //发送数据帧
  TxMessage. DLC = 0x08;			//数据长度8
  TxMessage. TransmitGlobalTime = DISABLE;
	printf("\r\n 0:OK; 1:error; 2:busy; 3:timeout;\r\n");
	for(int i =0; i <(Temp_Sample_Num/4)+1; i++)
	{
	
	CAN1_Tx_data[0] = ADC_ConvertedValue[4*i +0] >> 8;//CAN发送是先低8位，再高8位
	CAN1_Tx_data[1] = ADC_ConvertedValue[4*i +0] ;
		
	CAN1_Tx_data[2] = ADC_ConvertedValue[4*i +1]  >> 8 ;
	CAN1_Tx_data[3] = ADC_ConvertedValue[4*i +1] ;
		
	CAN1_Tx_data[4] = ADC_ConvertedValue[4*i +2]  >> 8;
	CAN1_Tx_data[5] = ADC_ConvertedValue[4*i +2] ;
		
	CAN1_Tx_data[6] = ADC_ConvertedValue[4*i +3]  >> 8;
	CAN1_Tx_data[7] = ADC_ConvertedValue[4*i +3] ;
	
	FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
	while(FreeTxNum == 0)FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
	/*等待发送邮箱有空*/
	
	status =  HAL_CAN_AddTxMessage(&hcan1, &TxMessage,CAN1_Tx_data,(uint32_t*)pTxMailbox);
	/*将数据放入邮箱*/
		
	if(status ==HAL_OK)
		{	
			printf("第%d组数据发送成功 -> 0x%x\r\n",i+1,status);
		}
		else
			printf ("第%d组数据发送失败-> 0x%x\r\n",i+1,status);
	
	}
	
}


#if 1  //回环测试时可以关闭
void HAL_CAN_RxFifo0MsgPendingCallback  ( CAN_HandleTypeDef *  hcan ) 
{
	if(hcan == &hcan1)
	{
		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxMessage,CAN1_Rx_data);
		
		
		__HAL_CAN_ENABLE_IT(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
		
		
	}
	
}
#endif
/* USER CODE END 1 */
