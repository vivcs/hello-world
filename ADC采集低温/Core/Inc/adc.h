/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define		NTC_T25				298.15
#define		NTC_R25				10.0
#define		NTC_B 				3950
#define   NTC_R_REF     10.0
#define		ADC_MAX				4095	
	
#define NTC_ADC_MAX		225
#define Temp_Sample_Num		20

extern float Adc_temp[20];	
//extern uint16_t ADC_ConvertedValue[Temp_Sample_Num];
/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

void ADC_DMA_Temper_Sample(void);


 void GetTemperature(void);
 void Display_Temper(void);
 float Get_temper(uint16_t adc_value);
 
int16_t Get_Kelvin_TemperatureX10(uint16_t adc_value);
static double myln(double a);
void ADC_Vol_Temper_Conver(void);

uint32_t ADC_Calibration(void);
void ADC_Value_Test(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

