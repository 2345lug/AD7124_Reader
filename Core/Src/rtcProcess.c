#include "stm32g0xx_hal.h"
#include "rtcProcess.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "platform_support.h"

extern RTC_HandleTypeDef hrtc; //Defined in main.c
static HAL_StatusTypeDef setTime (uint8_t* timeArray);
static HAL_StatusTypeDef setDate (uint8_t* dateArray);
static void parseInputTime (uint8_t* inputString);
static void parseInputDate (uint8_t* inputString);
static void errorKeyEntered (void);

static uint8_t inputStatus = 0;
static uint32_t startTicks = 0;
static uint8_t inputChars[INPUT_TIME_STRING_SIZE] = {0};
static uint8_t inputCharCount = 0;
const static uint8_t* stringTemplate = TIME_MASK;

void printTimeUart(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BIN
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	printf("\r\nTime %02d:%02d:%02d Date %02d-%02d-%02d\r\n", sTime.Hours, sTime.Minutes, sTime.Seconds, sDate.Date, sDate.Month, sDate.Year);

}

void printTimeString(uint8_t* resultStringPointer)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	static uint32_t currentTicks = 0;
	currentTicks = HAL_GetTick();
	currentTicks = currentTicks % 1000;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BIN
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	sprintf(resultStringPointer ,"%02d:%02d:%02d:%03d_%02d-%02d-%02d\t", sTime.Hours, sTime.Minutes, sTime.Seconds, currentTicks, sDate.Date, sDate.Month, sDate.Year);

}

void rtcConsoleInput(void)
{

	if (inputStatus == 0)
	{
		startTicks = HAL_GetTick();
		inputStatus = 1;
	}

	char keyPressed = 0;
	uint32_t currentTicks = HAL_GetTick();
	printf ("If you want to change time press T \r\n");

	while (inputStatus < 3)
	{

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
			printf("Input date time in next format: hh:mm_dd-MM-yy_D, where D is number of day of the week (1 - Monday) or s for skip \r\n");
		  }
		  currentTicks = HAL_GetTick();
		}



	while (inputStatus > 1 && inputCharCount < INPUT_TIME_STRING_SIZE)
	{
		switch(inputStatus)
		{
		  case 2:
			keyPressed = getchar_nonblocking();
			if (keyPressed > 0 && keyPressed < 255)
			{

              inputChars[inputCharCount] = keyPressed;

              HAL_UART_Transmit(&huart1, (uint8_t *)&keyPressed, 1, 0xFFFFFFFF);
              if (keyPressed == 's')
              {
                printf ("Skipping time setup \r\n");
                return 0;
              }

              if (stringTemplate[inputCharCount] == 'i')
              {
                if (keyPressed < 48 || keyPressed > 57)
                {
                  errorKeyEntered();
                  break;
                }
              }
              else
              {
            	if (keyPressed != stringTemplate[inputCharCount])
            	{
            	  errorKeyEntered();
            	  break;
            	}
              }
              inputCharCount++;

			}
			inputStatus = 3;
			break;

		}
	}

	}
	if (inputCharCount == INPUT_TIME_STRING_SIZE)
	{
		printf ("Got values %s \r\n", inputChars);
		parseInputTime(inputChars);
		parseInputDate(inputChars);
		printTimeUart();
	}

	return 0;
}

static void parseInputTime (uint8_t* inputString)
{
  uint8_t timeArray[3] = { 0 };
  *(timeArray + 0) = (*(inputString + 0) - 48) * 10 + (*(inputString + 1) - 48);
  *(timeArray + 1) = (*(inputString + 3) - 48) * 10 + (*(inputString + 4) - 48);
  *(timeArray + 2) = 0;
  setTime(timeArray);
}
//hh:mm_dd-MM-yy_D
static void parseInputDate (uint8_t* inputString)
{
  uint8_t dateArray[4] = { 0 } ;
  *(dateArray + 0) = (*(inputString + 6) - 48) * 10 + (*(inputString + 7) - 48);
  *(dateArray + 1) = (*(inputString + 9) - 48) * 10 + (*(inputString + 10) - 48);
  *(dateArray + 2) = (*(inputString + 12) - 48) * 10 + (*(inputString + 13) - 48);
  *(dateArray + 3) = (*(inputString + 15) - 48);
  setDate(dateArray);
}

static void errorKeyEntered (void)
{
  printf ("Error key enetered! \r\n");
  startTicks = HAL_GetTick();
  inputStatus = 1;
  inputCharCount = 0;
}

static HAL_StatusTypeDef setTime (uint8_t* timeArray)
{
  RTC_TimeTypeDef sTime = {0};
  sTime.Hours = *(timeArray + 0);
  sTime.Minutes = *(timeArray + 1);
  sTime.Seconds = *(timeArray + 2);
  sTime.SubSeconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;

  HAL_StatusTypeDef setupResult = HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  return setupResult;
}

static HAL_StatusTypeDef setDate (uint8_t* dateArray)
{
  RTC_DateTypeDef sDate = {0};
  HAL_StatusTypeDef setupResult = HAL_OK;

  sDate.WeekDay = *(dateArray + 3);
  sDate.Month = *(dateArray + 1);
  sDate.Date =  *(dateArray + 0);
  sDate.Year = *(dateArray + 2);

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }

  setupResult = HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
  return setupResult;
}
