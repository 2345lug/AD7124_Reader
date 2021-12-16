/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SD_CS_Pin GPIO_PIN_2
#define SD_CS_GPIO_Port GPIOA
#define BUTTON_Pin GPIO_PIN_5
#define BUTTON_GPIO_Port GPIOA
#define CS1_Pin GPIO_PIN_0
#define CS1_GPIO_Port GPIOB
#define CS2_Pin GPIO_PIN_1
#define CS2_GPIO_Port GPIOB
#define SD_DETECT_Pin GPIO_PIN_8
#define SD_DETECT_GPIO_Port GPIOA
#define AD_SYNC_Pin GPIO_PIN_6
#define AD_SYNC_GPIO_Port GPIOC
#define PowerMonitor_Pin GPIO_PIN_15
#define PowerMonitor_GPIO_Port GPIOA
#define CS3_Pin GPIO_PIN_3
#define CS3_GPIO_Port GPIOB
#define CS4_Pin GPIO_PIN_4
#define CS4_GPIO_Port GPIOB
#define CS5_Pin GPIO_PIN_5
#define CS5_GPIO_Port GPIOB
#define TRIG_5V_Pin GPIO_PIN_8
#define TRIG_5V_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define SD_SPI_HANDLE hspi2
#define CHANNEL_COUNT 6
#define TEMPERATURE_SYMBOLS_COUNT 6
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
