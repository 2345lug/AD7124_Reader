#include "stm32g0xx_hal.h"
#include "rtcProcess.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "platform_support.h"

extern RTC_HandleTypeDef hrtc; //Defined in main.c
static HAL_StatusTypeDef setTime (uint8_t hours, uint8_t minutes, uint8_t seconds);
static HAL_StatusTypeDef setDate (uint8_t hours, uint8_t minutes, uint8_t seconds);

void printTimeUart(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BCD
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	printf("\r\nTime %02d:%02d:%02d Date %02d-%02d-%02d\r\n", sTime.Hours, sTime.Minutes, sTime.Seconds, sDate.Date, sDate.Month, sDate.Year);

}

void rtcConsoleInput(void)
{
	static uint8_t inputStatus = 0;
	static uint32_t startTicks = 0;
	static uint8_t inputChars[INPUT_TIME_STRING_SIZE] = {0};
	if (inputStatus == 0)
	{
		startTicks = HAL_GetTick();
		inputStatus = 1;
	}

	char keyPressed = 0;
	uint32_t currentTicks = HAL_GetTick();
	printf ("If you want to change time press T \r\n");
	while ((currentTicks - startTicks) < 10000 && inputStatus == 1)
	{
	  keyPressed = toupper(getchar_nonblocking());
	  if (keyPressed > 0 && keyPressed < 255)
	  {
		  HAL_UART_Transmit(&huart1, (uint8_t *)&keyPressed, 1, 0xFFFFFFFF);
	  }
	  if (keyPressed == 'T')
	  {
		inputStatus = 2;
		printf("input date time in next format: hh:mm_dd:MM:yy_D, where D is number of day of the week (1 - Monday) \r\n");
	  }
	  currentTicks = HAL_GetTick();
	}

	int charCount = 0;
	while (inputStatus > 1 && charCount < INPUT_TIME_STRING_SIZE)
	{
		switch(inputStatus)
		{
		  case 2:
			keyPressed = getchar_nonblocking();
			if (keyPressed > 0 && keyPressed < 255)
			{

              inputChars[charCount] = keyPressed;
              charCount++;
              HAL_UART_Transmit(&huart1, (uint8_t *)&keyPressed, 1, 0xFFFFFFFF);
              //printf("%c %d %d \0", keyPressed, keyPressed, charCount);
			}
		    break;
		}
	}
	printf ("Got values is %s \r\n", inputChars);
	return 0;
}

static HAL_StatusTypeDef setTime (uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  RTC_TimeTypeDef sTime = {0};
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.SubSeconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;

  HAL_StatusTypeDef setupResult = HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
  return setupResult;
}

static HAL_StatusTypeDef setDate (uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  RTC_DateTypeDef sDate = {0};
  HAL_StatusTypeDef setupResult = HAL_OK;

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
    {
      Error_Handler();
    }

  setupResult = HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
  return setupResult;
}
