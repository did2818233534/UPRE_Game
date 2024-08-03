/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DJ_bsp.h"
#include "pid.h"
#include "Vofa.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t DATA_R[8]={0};

struct PID_Speed_HandleTypeDef PID_Speed[4] = {{0,0,0,0,0,0,0,0,0}};
struct PID_Angle_HandleTypeDef PID_Angle[4] = {{0,0,0,0,0,0,0,0}};
struct PID_Angle_and_Speed_HandleTypeDef PID_Angle_and_Speed[4]={{0,0,0,0,0,0,0}};
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
  MX_CAN1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		Vofa_Transmit_8(1,2,3,4,5,6,7,8);
//		HAL_Delay(1);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan == &hcan1)
	{	
/**********************************************************************************************************************/
//		The motor rotates at different angles	

		static uint16_t Currents[4];
		int16_t Current;
		
		static float Angles[4] = {0};
		static float Angle_last[4] = {0};
		static uint8_t a[4] = {1,1,1,1};
		CAN_RxHeaderTypeDef hCAN_RxHeader;

		if(HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&hCAN_RxHeader, DATA_R)==HAL_OK)
		{
		}
		
		if(a[hCAN_RxHeader.StdId - 0x201]) 
		{
			Angle_last[hCAN_RxHeader.StdId - 0x201] = DJ_Angle;
			a[hCAN_RxHeader.StdId - 0x201] = 0;
		}
		
		Angles[hCAN_RxHeader.StdId - 0x201] += ((DJ_Angle - Angle_last[hCAN_RxHeader.StdId - 0x201])/36);
		
		
		if(DJ_Speed > 4000)
		{
			if(((DJ_Angle - Angle_last[hCAN_RxHeader.StdId - 0x201]) < 0)&&(DJ_Speed > 0)) Angles[hCAN_RxHeader.StdId - 0x201] +=(360/36);
			else if(((DJ_Angle - Angle_last[hCAN_RxHeader.StdId - 0x201]) > 0)&&(DJ_Speed < 0)) Angles[hCAN_RxHeader.StdId - 0x201] -=(360/36);
		}
		else 
		{
			if((DJ_Angle - Angle_last[hCAN_RxHeader.StdId - 0x201]) < -180) Angles[hCAN_RxHeader.StdId - 0x201] +=(360/36);
			else if((DJ_Angle - Angle_last[hCAN_RxHeader.StdId - 0x201]) > 180) Angles[hCAN_RxHeader.StdId - 0x201] -=(360/36);
		}
		Angle_last[hCAN_RxHeader.StdId - 0x201] = DJ_Angle;
		switch(hCAN_RxHeader.StdId - 0x201)
		{
			case 0:
				pid_Angle(0,PID_Angle,PID_Speed,1800, Angles[0], DJ_Speed,0.1,0.001,0.03,6,0,10);
						Current = PID_Angle[0].Current_set;break;
			case 2:
				pid_Angle(2,PID_Angle,PID_Speed,360, Angles[2], DJ_Speed,0.1,0.001,0.03,6,0,10);
						Current = PID_Angle[2].Current_set;break;
		}
		Currents[hCAN_RxHeader.StdId - 0x201] = (uint16_t)Current;
		Motor_Set_Current(Currents);
		
//		static uint8_t num = 0;
//		num++;
//		if(num == 4) 
//		{
//			Motor_Set_Current(Currents);
//			num = 0;
//		}
		
		Vofa_Transmit_8(PID_Angle[0].err,Angles[0],PID_Angle[2].err,Angles[2],0,0,0,0);
/************************************************************************************************************************/
//		Motors rotate at different speeds

//		static uint16_t Currents[4];
//		int16_t Current;
//		CAN_RxHeaderTypeDef hCAN_RxHeader;
//		
//		
//		if(HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&hCAN_RxHeader, DATA_R)==HAL_OK)
//		{
//		}
//		
//		switch(hCAN_RxHeader.StdId - 0x201)
//		{
////			case 0:
////				pid_Speed(0,PID_Speed,0,DJ_Speed,DJ_Current,6,0.03,0);
////				Currents[0] = (int16_t)PID_Speed[0].Current_set;break;
//			case 2:
//				pid_Speed(2,PID_Speed,30,DJ_Speed,7,0,10);
//				Currents[2] = (uint16_t)PID_Speed[2].Current_set;break;
//		}
//		Motor_Set_Current(Currents);


//		Vofa_Transmit_8(DJ_Speed,PID_Speed[2].Current_set,0,0,0,0,0,0);
//		
/***********************************************************************************************************************/	

//		static uint16_t Currents[4];
//		int16_t Current;
//		
//		static int16_t Angles[4] = {0};
//		static int16_t Angle_last[4] = {0};
//		static uint8_t a[4] = {1,1,1,1};
//		CAN_RxHeaderTypeDef hCAN_RxHeader;

//		if(HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&hCAN_RxHeader, DATA_R)==HAL_OK)
//		{
//		}
//		
//		if(a[hCAN_RxHeader.StdId - 0x201]) 
//		{
//			Angle_last[hCAN_RxHeader.StdId - 0x201] = DJ_Angle;
//			a[hCAN_RxHeader.StdId - 0x201] = 0;
//		}
//		
//		Angles[hCAN_RxHeader.StdId - 0x201] += DJ_Angle - Angle_last[hCAN_RxHeader.StdId - 0x201];
//		if((DJ_Angle - Angle_last[hCAN_RxHeader.StdId - 0x201]) < (-180)) Angles[hCAN_RxHeader.StdId - 0x201] +=360;
//		else if((DJ_Angle - Angle_last[hCAN_RxHeader.StdId - 0x201]) > 180) Angles[hCAN_RxHeader.StdId - 0x201] -=360;
//		Angle_last[hCAN_RxHeader.StdId - 0x201] = DJ_Angle;
//		switch(hCAN_RxHeader.StdId - 0x201)
//		{
//			case 2:Current = pid_Angle_and_Speed(2,360,60, Angles[2], DJ_Speed,DJ_Current,9,0.01,0,Speed_pid)->Current_set;break;
//			case 0:Current = pid_Angle_and_Speed(0,360,60, Angles[0], DJ_Speed,DJ_Current,9,0.01,0,Speed_pid)->Current_set;break;
//		}
//		Currents[hCAN_RxHeader.StdId - 0x201] = (uint16_t)Current;
//		Motor_Set_Current(Currents);
//		
//		float buff = Angles[2];

//		if(!(hCAN_RxHeader.StdId - 0x201))
//		{
//			uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};
//			HAL_UART_Transmit(&huart1,(uint8_t*)&buff,4,0xff);
////			HAL_UART_Transmit(&huart1,(uint8_t*)&PID_Speed[2]->pout,4,0xff);
////			HAL_UART_Transmit(&huart1,(uint8_t*)&PID_Speed[2]->iout,4,0xff);
////			HAL_UART_Transmit(&huart1,(uint8_t*)&PID_Speed[2]->dout,4,0xff);
//			HAL_UART_Transmit(&huart1,tail,4,0xff);
//		}

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
