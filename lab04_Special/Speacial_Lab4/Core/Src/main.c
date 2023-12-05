/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
char newline[]="\r\n" , name[100] ,sendbuffer[100], recievebuffer[100], state=0, ch, readytosend=0, firstTime=0,checkRe=0;
int idx=0;
int idxbuffer=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart3,(uint8_t*) &ch,1);
  HAL_UART_Transmit(&huart3,(uint8_t*) "Man from U.A.R.T.2\r\nQuit PRESS q\r\n ", strlen("Man from U.A.R.T.2\r\nQuit PRESS q\r\n"),1000);
  //HAL_NVIC_EnableIRQ(USART3_IRQn);

  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    	// first time input for text "Mr.One is ready"
    	if(!state&&readytosend&&!firstTime){
    		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
    	   HAL_UART_Transmit(&huart3,(uint8_t*) "\tName: ", strlen("\tName: "),1000);
    	   firstTime=1;
    	}
    	//====== if not ready to send wait for uart2 input ========
    	if(!readytosend){
    		//=================== For Debug UART_FLAG_RXNE being set at firsttime I don't know why???================================
//    		if(!firstTime){
//    			while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)==RESET){}
//    			HAL_UART_Receive(&huart2,(uint8_t*) recievebuffer, 100,1000);
//    			firstTime=1;
//    		}
    		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)==RESET){}
    		HAL_UART_Receive(&huart2,(uint8_t*) recievebuffer, 100,1000);
    		if(recievebuffer[0]=='q'&&strlen(recievebuffer)==1){
    			state=2;
    			while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
    			HAL_UART_Transmit(&huart3,(uint8_t*) "Program ended from U.A.R.T.1", strlen("Program ended from U.A.R.T.1"),1000);
    			//HAL_NVIC_DisableIRQ(USART3_IRQn);
    		}
    		else{
    			while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
    			HAL_UART_Transmit(&huart3,(uint8_t*) recievebuffer, strlen(recievebuffer),1000);
    			while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
    			HAL_UART_Transmit(&huart3,(uint8_t*) "\t", 1,1000);
    			while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
    			HAL_UART_Transmit(&huart3,(uint8_t*) name, strlen(name),1000);
    			if(!checkRe){
    				checkRe=1;
    			}else{
    				while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
    				HAL_UART_Transmit(&huart3,(uint8_t*) " => ", strlen(" => "),1000);
    			}
    			readytosend=1;
    			memset(recievebuffer,0,strlen(recievebuffer));
    			//HAL_NVIC_EnableIRQ(USART3_IRQn);
    		}
    	}
    	HAL_Delay(1000);
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	//debug btn
	if(GPIO_Pin == GPIO_PIN_13){
		HAL_UART_Transmit(&huart3,(uint8_t*) "Mr.Mock", strlen("Mr.Mock"),1000);
		HAL_UART_Transmit(&huart3,(uint8_t*) " : ", strlen(" : "),100);
		HAL_UART_Transmit(&huart3,(uint8_t*) "test text", strlen("test text"),100);
		HAL_UART_Transmit(&huart3,(uint8_t*) newline, strlen(newline),100);
		readytosend=1;
		//HAL_NVIC_EnableIRQ(USART3_IRQn);
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart==&huart3){
		HAL_UART_Receive_IT(&huart3,(uint8_t*) &ch,1);
		if(readytosend){
		if(!state){
			while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
			HAL_UART_Transmit(&huart3,(uint8_t*) &ch, 1,1000);
			if(ch==13){
				 //print new line
				while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3,(uint8_t*) newline, strlen(newline),1000);
				//======= send that this person is ready!!! =============
				while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart2,(uint8_t*) "\t", 1,1000);
				while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
			    HAL_UART_Transmit(&huart2,(uint8_t*) name, strlen(name),1000);
			    while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart2,(uint8_t*) " is ready.", strlen(" is ready."),1000);
				while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart2,(uint8_t*) newline, strlen(newline),1000);
				//=======================================================
				readytosend=0;
				state=1;
				firstTime=0;
				//HAL_NVIC_DisableIRQ(USART3_IRQn);
			}
			else{
				name[idx++]=ch;
			}
		}
		else if(state==1){
		    HAL_UART_Transmit(&huart3,(uint8_t*) &ch, 1,1000);
		    if(ch==13){
		    	//print new line
		    	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		    	HAL_UART_Transmit(&huart3,(uint8_t*) newline, strlen(newline),1000);
		    	if(sendbuffer[0]=='q' && idxbuffer==1){
		    		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		    		HAL_UART_Transmit(&huart3,(uint8_t*) "Program ended", strlen("Program ended"),1000);
		    		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
		    		HAL_UART_Transmit(&huart2,(uint8_t*) "q", 1,1000);
		    		state=2;
		    	}
		    	else{
		    		//===== send message =====
		    		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
		    		HAL_UART_Transmit(&huart2,(uint8_t*) "\t", 1,1000);
		    		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
		    		HAL_UART_Transmit(&huart2,(uint8_t*) name, strlen(name),1000);
		    		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
		    		HAL_UART_Transmit(&huart2,(uint8_t*) " : ", strlen(" : "),1000);
		    		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
		    		HAL_UART_Transmit(&huart2,(uint8_t*) sendbuffer, strlen(sendbuffer),1000);
		    		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET){}
		    		HAL_UART_Transmit(&huart2,(uint8_t*) newline, strlen(newline),1000);
		    		//========================
		    		idxbuffer=0;
		    		memset(sendbuffer,0,strlen(sendbuffer));
		    		readytosend=0;

		    		//HAL_NVIC_DisableIRQ(USART3_IRQn);
		    	}
		    }
		    else{
		    	sendbuffer[idxbuffer++]=ch;
		    }
		}
		}
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
