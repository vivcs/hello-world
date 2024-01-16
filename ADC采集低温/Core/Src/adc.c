/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
//uint16_t ADC_ConvertedValue[Temp_Sample_Num];
float Adc_temp[20] = {0};
//float Adc_temp_subzero[20] = {0};
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;


static const uint16_t ADC_NTC[301] = 
{
	3157, 																											//0.0
	3161, 3165, 3168, 3172, 3176, 3180, 3184, 3187, 3191, 3195, //-0.1 ~ -1.0
	3199, 3202, 3206, 3210, 3214, 3217, 3221, 3225, 3228, 3232, //-1.1 ~ -2.0
	3236, 3239, 3243, 3247, 3250, 3254, 3257, 3261, 3265, 3268, //-2.1 ~ -3.0
	3272, 3275, 3279, 3282, 3286, 3289, 3293, 3296, 3300, 3303, //-3.1 ~ -4.0
	3307, 3310, 3314, 3317, 3321, 3324, 3328, 3331, 3334, 3338, //-4.1 ~ -5.0
	3341, 3345, 3348, 3351, 3355, 3358, 3361, 3365, 3368, 3371, //-5.1 ~ -6.0
	3375, 3378, 3381, 3384, 3388, 3391, 3394, 3397, 3401, 3404, //-6.1 ~ -7.0
	3407, 3410, 3413, 3417, 3420, 3423, 3426, 3429, 3432, 3435, //-7.1 ~ -8.0
	3439, 3442, 3445, 3448, 3451, 3454, 3457, 3460, 3463, 3466, //-8.1 ~ -9.0
	3469, 3472, 3475, 3478, 3481, 3484, 3487, 3490, 3493, 3496, //-9.1 ~ -10.0
	3499, 3502, 3505, 3507, 3510, 3513, 3516, 3519, 3522, 3525, //-10.1 ~ -11.0
	3527, 3530, 3533, 3536, 3539, 3541, 3544, 3547, 3550, 3552,	//-11.1 ~ -12.0
	3555, 3558, 3560, 3563, 3566, 3569, 3571, 3574, 3577, 3579,	//-12.1 ~ -13.0
	3582, 3584, 3587, 3590, 3592, 3595, 3597, 3600, 3603, 3605,	//-13.1 ~ -14.0
	3608, 3610, 3613, 3615, 3618, 3620, 3623, 3625, 3628, 3630,	//-14.1 ~ -15.0
	3632, 3635, 3637, 3640, 3642, 3645, 3647, 3649, 3652, 3654, //-15.1 ~ -16.0
	3656, 3659, 3661, 3663, 3666, 3668, 3670, 3673, 3675, 3677,	//-16.1 ~ -17.0
	3679, 3682, 3684, 3686, 3688, 3691, 3693, 3695, 3697, 3699,	//-17.1 ~ -18.0
	3701, 3704, 3706, 3708, 3710, 3712, 3714, 3716, 3719, 3721,	//-18.1 ~ -19.0
	3723, 3725, 3727, 3729, 3731, 3733, 3735, 3737, 3739, 3741, //-19.1 ~ -20.0
	3743, 3745, 3747, 3749, 3751, 3753, 3755, 3757, 3759, 3761,	//-20.1 ~ -21.0
	3762, 3764, 3766, 3768, 3770, 3772, 3774, 3776, 3777, 3779, //-21.1 ~ -22.0
	3781, 3783, 3785, 3787, 3788, 3790, 3792, 3794, 3795, 3797,	//-22.1 ~ -23.0
	3799, 3801, 3802, 3804, 3806, 3808, 3809, 3811, 3813, 3814, //-23.1 ~ -24.0
	3816, 3818, 3819, 3821, 3823, 3824, 3826, 3827, 3829, 3831, //-24.1 ~ -25.0
	3832, 3834, 3835, 3837, 3838, 3840, 3842, 3843, 3845, 3846,	//-25.1 ~ -26.0
	3848, 3849, 3851, 3852, 3854, 3855, 3857, 3858, 3859, 3861,	//-26.1 ~ -27.0
	3862, 3864, 3865, 3867, 3868, 3869, 3871, 3872, 3874, 3875, //-27.1 ~ -28.0
	3876, 3878, 3879, 3880, 3882, 3883, 3884, 3886, 3887, 3888, //-28.1 ~ -29.0
	3890, 3891, 3892, 3894, 3895, 3896, 3897, 3899, 3900, 3901, //-29.1 ~ -30.0
};


/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 3;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC4     ------> ADC1_IN13
    PC5     ------> ADC1_IN14
    PB0     ------> ADC1_IN15
    */
    GPIO_InitStruct.Pin = TEMP3_Pin|TEMP2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TEMP1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TEMP1_GPIO_Port, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Request = DMA_REQUEST_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PC4     ------> ADC1_IN13
    PC5     ------> ADC1_IN14
    PB0     ------> ADC1_IN15
    */
    HAL_GPIO_DeInit(GPIOC, TEMP3_Pin|TEMP2_Pin);

    HAL_GPIO_DeInit(TEMP1_GPIO_Port, TEMP1_Pin);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#define ADC_DMA_LEN			6
uint16_t adc_raw_value[ADC_DMA_LEN] = {0};


//这是一个ADC值转温度值的函数
float Get_temper(uint16_t adc_value)
{
   unsigned int i;
    for(i=0;i<301;i++)
    {
        if(adc_value <= ADC_NTC[i]) //当没有ADC值对应温度的时候，就取相近的，也就是大于的情况
        {
            return ((-1)*(0.1*i));
            break;
        }
    }
}


static void TemperatureSwitch_Control(const uint8_t cmd)
{
  GPIO_PinState gps[3] = {GPIO_PIN_RESET};
  for(uint8_t i = 0; i < 3; i++)
  {
    if((cmd>>i)&0x01) gps[i] = GPIO_PIN_SET;//控制Y1-Y7
  }
  HAL_GPIO_WritePin(TEMP_A_GPIO_Port,TEMP_A_Pin,gps[0]);
  HAL_GPIO_WritePin(TEMP_B_GPIO_Port,TEMP_B_Pin,gps[1]);
  HAL_GPIO_WritePin(TEMP_C_GPIO_Port,TEMP_C_Pin,gps[2]);
}
//  HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);    //ADC校准
//	if(HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_raw_value, ADC_DMA_LEN)!= HAL_OK)
//		Error_Handler();
 void GetTemperature(void)
{
  const uint8_t T1_sort[7] = {5,6,0,4,1,3,2};
  uint16_t adc_value[3] = {0};
  int16_t temperatureX10[3] = {0};
	
	float temp = 0;
	
    HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);    //ADC校准
//	if(HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_raw_value, ADC_DMA_LEN)!= HAL_OK)
//		Error_Handler();
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_raw_value, ADC_DMA_LEN);
	for(uint8_t i = 0; i < 7; i++)
  {
    TemperatureSwitch_Control(i+1);
		HAL_Delay(3);
    for(uint8_t j = 0; j < 3; j++)
    {
			adc_value[j] = adc_raw_value[j];
			
				//零下温度用数组
			if(adc_value[j] >ADC_NTC[0])
			{
					temp = Get_temper(adc_value[j]);
//					printf("temp = %.1f\n ", temp);
					if(i != 2 || j != 2)
					Adc_temp[T1_sort[i]+j*7-j/2]  =temp;
					ADC_ConvertedValue[T1_sort[i]+j*7-j/2] = adc_value[j];
			}
				
			else//零上温度用公式
			{			
				temperatureX10[j] = Get_Kelvin_TemperatureX10(adc_value[j]);//得到摄氏温度 x10
			if(i != 2 || j != 2)
				Adc_temp[T1_sort[i]+j*7-j/2] = temperatureX10[j]/10.0;
			ADC_ConvertedValue[T1_sort[i]+j*7-j/2] = adc_value[j];
			}
    }
  }
		printf("\r\n");
			for(uint8_t i = 0; i < 20; i++)
		{
			printf(" T%d=%0.1f°C ; ",i+1,Adc_temp[i]);
			if(i%4 == 0)
				printf("\n");
		}
}	


/*DMA模式下的ADC多通道采集，数值存放在ADC_ConvertedValue数组*/
void ADC_DMA_Temper_Sample(void)
{

	/*对TEMPA、B、C控制的74HC地址输出分别进行3次ADC采集
	例：Y1口为TEMPA:1;TEMPB、TEMPC：0，0；对应的TEMP1、2、3：T6、T13、T19
	分别对应ADC三个通道采集*/
	
	uint32_t calibra_value;
	//ADC使能前先进行校准，将得到的校准值带入计算
	calibra_value = ADC_Calibration();
//	calibra_value +=0x02;
	ADC_Enable(&hadc1);	//使能ADC
	HAL_ADCEx_Calibration_SetValue(&hadc1,ADC_SINGLE_ENDED ,calibra_value);	//将得到的校准值代入
	

	HAL_GPIO_WritePin(GPIOC,TEMP_B_Pin|TEMP_C_Pin,GPIO_PIN_RESET);	\
											HAL_GPIO_WritePin(GPIOB,TEMP_A_Pin,GPIO_PIN_SET);		
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_ConvertedValue,3);//Y1口的ADC温度采集	
	HAL_Delay(12);
	HAL_ADC_Stop_DMA(&hadc1);
	
	HAL_Delay(12);
	ADC_Enable(&hadc1);
	HAL_ADCEx_Calibration_SetValue(&hadc1,ADC_SINGLE_ENDED ,calibra_value);
	HAL_GPIO_WritePin(GPIOC,TEMP_C_Pin,GPIO_PIN_RESET);\
										HAL_GPIO_WritePin(GPIOB,TEMP_A_Pin,GPIO_PIN_RESET)	;\
														HAL_GPIO_WritePin(GPIOC,TEMP_B_Pin,GPIO_PIN_SET);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_ConvertedValue[3],3);//Y2口的ADC温度采集
	HAL_Delay(12);
	HAL_ADC_Stop_DMA(&hadc1);	
	
	HAL_Delay(12);
	ADC_Enable(&hadc1);
	HAL_ADCEx_Calibration_SetValue(&hadc1,ADC_SINGLE_ENDED ,calibra_value);
	HAL_GPIO_WritePin(GPIOC,TEMP_C_Pin,GPIO_PIN_RESET);\
												HAL_GPIO_WritePin(GPIOB,TEMP_A_Pin,GPIO_PIN_SET)	;\
														HAL_GPIO_WritePin(GPIOC,TEMP_B_Pin,GPIO_PIN_SET);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_ConvertedValue[6],3);//Y3口的ADC温度采集
	HAL_Delay(12);
	HAL_ADC_Stop_DMA(&hadc1);
	
	HAL_Delay(12);
	ADC_Enable(&hadc1);
	HAL_ADCEx_Calibration_SetValue(&hadc1,ADC_SINGLE_ENDED ,calibra_value);	
	HAL_GPIO_WritePin(GPIOC,TEMP_B_Pin,GPIO_PIN_RESET);	\
												HAL_GPIO_WritePin(GPIOB,TEMP_A_Pin,GPIO_PIN_RESET)	;\
														HAL_GPIO_WritePin(GPIOC,TEMP_C_Pin,GPIO_PIN_SET);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_ConvertedValue[9],3);//Y4口的ADC温度采集	
	HAL_Delay(12);
	HAL_ADC_Stop_DMA(&hadc1);
	
	HAL_Delay(12);
	ADC_Enable(&hadc1);
	HAL_ADCEx_Calibration_SetValue(&hadc1,ADC_SINGLE_ENDED ,calibra_value);	
	HAL_GPIO_WritePin(GPIOC,TEMP_B_Pin|TEMP_C_Pin,GPIO_PIN_RESET);	\
												HAL_GPIO_WritePin(GPIOB,TEMP_A_Pin,GPIO_PIN_SET)	;\
														HAL_GPIO_WritePin(GPIOC,TEMP_C_Pin,GPIO_PIN_SET);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_ConvertedValue[12],3);//Y5口的ADC温度采集	
	HAL_Delay(12);
	HAL_ADC_Stop_DMA(&hadc1);
	
	HAL_Delay(12);
	ADC_Enable(&hadc1);
	HAL_ADCEx_Calibration_SetValue(&hadc1,ADC_SINGLE_ENDED ,calibra_value);	
	HAL_GPIO_WritePin(GPIOC,TEMP_B_Pin|TEMP_C_Pin,GPIO_PIN_SET);	\
												HAL_GPIO_WritePin(GPIOB,TEMP_A_Pin,GPIO_PIN_RESET);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_ConvertedValue[15],3);//Y6口的ADC温度采集	
	HAL_Delay(12);
	HAL_ADC_Stop_DMA(&hadc1);
	
	HAL_Delay(12);
	ADC_Enable(&hadc1);
	HAL_ADCEx_Calibration_SetValue(&hadc1,ADC_SINGLE_ENDED ,calibra_value);
	HAL_GPIO_WritePin(GPIOC,TEMP_B_Pin|TEMP_C_Pin,GPIO_PIN_SET);	\
												HAL_GPIO_WritePin(GPIOB,TEMP_A_Pin,GPIO_PIN_SET);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_ConvertedValue[18],3);//Y7口的ADC温度采集	
	HAL_Delay(12);									
	HAL_ADC_Stop_DMA(&hadc1);													
}


/*对热敏电阻阻值随温度变化与电压的转换公式*/
//求出ln,取e的前16位进行估算
static double myln(double a)
{
   int N = 15;//我们取了前15+1项来估算
   int k,nk;
   double x,xx,y;
   x = (a-1)/(a+1);
   xx = x*x;
   nk = 2*N+1;
   y = 1.0/nk;
   for(k=N;k>0;k--)
   {
     nk = nk - 2;
     y = 1.0/nk+xx*y;

   }
   return 2.0*x*y;
}

/*得到转换后的摄氏温度 X10 */
int16_t Get_Kelvin_TemperatureX10(uint16_t adc_value)
{
	float N1,N2,N3,N4;
  double Rntc = NTC_R_REF*adc_value/(ADC_MAX-adc_value);
  if(Rntc < 0.0)
    return -1000;
	N1 = myln(NTC_R25/Rntc)/NTC_B;
	N2 = 1/NTC_T25 - N1;
	N3 = 1/N2;
	N4 = (double)N3-273.15;
	return (int16_t)(N4*10);
}

//float Adc_temp[21];
/*对采集的ADC值进行电压和温度的转换*/
void ADC_Vol_Temper_Conver(void)
{
	float temp = 0.0;
	for(unsigned int i =0; i<Temp_Sample_Num; i++)
	{
//		printf("\r\n Temper%d=%.3f V ,",i+1,((float)ADC_ConvertedValue[i]/4096) *(float)3.3);
		
				//零下温度用数组
			if(ADC_ConvertedValue[i] >= 3157)
			{
				for(int k=0; k<302; k++)
				{
					if(ADC_ConvertedValue[i] >= ADC_NTC[k])
					{
//						
						temp =(0 - 0.1 *k);
//						temp /=10.0;
						Adc_temp[i] = temp;
					}
				}
			}
			else//零上温度用公式
			{
				temp = Get_Kelvin_TemperatureX10(ADC_ConvertedValue[i]); //得到摄氏温度 x10
				temp /=10.0;
				Adc_temp[i] = temp;
			}
	//		printf("\nCAN_Tx_data = 0x%x \n",CAN1_Tx_data[i]);	
		CAN1_Tx_data[i] = temp;		
		
	}
	
		printf(" T01=%.1f°C ; ",Adc_temp[6]);
		printf(" T02=%.1f°C ; ",Adc_temp[12]);
		printf(" T03=%.1f°C ; ",Adc_temp[18]);
		printf(" T04=%.1f°C ; ",Adc_temp[15]);
		printf(" T05=%.1f°C ; ",Adc_temp[9]);
		printf("\r\n");
	
		printf(" T06=%.1f°C ; ",Adc_temp[0]);
		printf(" T07=%.1f°C ; ",Adc_temp[3]);
		printf(" T08=%.1f°C ; ",Adc_temp[7]);
		printf(" T09=%.1f°C ; ",Adc_temp[13]);
		printf(" T10=%.1f°C ; ",Adc_temp[19]);
		printf("\r\n");
	
		printf(" T11=%.1f°C ; ",Adc_temp[16]);
		printf(" T12=%.1f°C ; ",Adc_temp[10]);
		printf(" T13=%.1f°C ; ",Adc_temp[1]);
		printf(" T14=%.1f°C ; ",Adc_temp[4]);
		printf(" T15=%.1f°C ; ",Adc_temp[14]);
		printf("\r\n");
		
		printf(" T16=%.1f°C ; ",Adc_temp[19]);
		printf(" T17=%.1f°C ; ",Adc_temp[17]);
		printf(" T18=%.1f°C ; ",Adc_temp[11]);
		printf(" T19=%.1f°C ; ",Adc_temp[2]);
		printf(" T20=%.1f°C ; ",Adc_temp[5]);
		printf("\r\n");
	
	
}



/*对ADC进行校准*/
uint32_t ADC_Calibration(void)
{
	uint32_t Calibra_Value = 0;
	HAL_ADC_Stop(&hadc1);
	
	HAL_ADCEx_Calibration_Start	(&hadc1,ADC_SINGLE_ENDED ); //开始校准
	Calibra_Value = HAL_ADCEx_Calibration_GetValue(&hadc1,ADC_SINGLE_ENDED) ;  //得到的校准值
	printf ("得到的校准值= 0x%x\r\n",Calibra_Value);
	return Calibra_Value;
	
}

/*ADC采样值测试*/
void ADC_Value_Test(void)
{
	for(int i =0; i<Temp_Sample_Num; i++)
	{
		printf("ADC_Value[%d]=%d, ",i,ADC_ConvertedValue[i]);
		if(i%4 == 0)
			printf("\n");
	}

}

/* USER CODE END 1 */
