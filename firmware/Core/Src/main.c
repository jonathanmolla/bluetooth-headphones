/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "battery_monitor.h"
#include "headphone_controls.h"
#include "BM83_UART.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
    AUDIO_WAITING_FOR_BATTERY,
    AUDIO_RUNNING,
    AUDIO_LOW_BATTERY
} AudioState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BATTERY_SHUTDOWN_MV    3300U
#define BATTERY_READ_PERIOD_MS 1000U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static BatteryReading battery;
static HAL_StatusTypeDef battery_status = HAL_ERROR;
static uint32_t battery_last_read_ms;
static AudioState audio_state = AUDIO_WAITING_FOR_BATTERY;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void UpdateBatteryAndAudio(void)
{
    battery_status = BatteryMonitor_ReadTracked(&hadc1, &battery);

    /* Only make a decision from a fresh, successful reading. */
    if (battery_status != HAL_OK || audio_state == AUDIO_LOW_BATTERY)
    {
        return;
    }

    if (battery.battery_mv <= BATTERY_SHUTDOWN_MV)
    {
        HAL_GPIO_WritePin(AMP_ENABLE_GPIO_Port, AMP_ENABLE_Pin, GPIO_PIN_RESET);
        audio_state = AUDIO_LOW_BATTERY;
        return;
    }

    if (audio_state == AUDIO_WAITING_FOR_BATTERY)
    {
        if (HeadphoneControls_Init(&hi2c1) != HAL_OK)
        {
            Error_Handler();
        }

        audio_state = AUDIO_RUNNING;
    }
}
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
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */

  if (BM83_Init(&huart1) != HAL_OK)
  {
      Error_Handler();
  }
  /* Calibrate ADC once after the generated peripheral setup. */
  if (BatteryMonitor_Init(&hadc1) != HAL_OK)
  {
      Error_Handler();
  }

  HAL_Delay(20U); /* Allow the battery-divider filter to settle. */
  UpdateBatteryAndAudio();



  battery_last_read_ms = HAL_GetTick();
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  uint32_t now = HAL_GetTick();

	      if ((uint32_t)(now - battery_last_read_ms) >= BATTERY_READ_PERIOD_MS)
	      {
	          battery_last_read_ms = now;
	          UpdateBatteryAndAudio();
	      }

	      if (audio_state == AUDIO_RUNNING && HeadphoneControls_Process() != HAL_OK)
	      {
	          Error_Handler();
	      }

	      bool play_pause_pressed = false;

	      if (audio_state == AUDIO_RUNNING)
	      {
	          play_pause_pressed = HeadphoneControls_TakePlayPause();
	      }

	      BM83_Process(audio_state == AUDIO_RUNNING, play_pause_pressed);
	  }
  }
  /* USER CODE END 3 */


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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);

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
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
