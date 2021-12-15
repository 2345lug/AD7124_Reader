#include "stm32g0xx_hal.h"
#include "rtcProcess.h"

extern RTC_HandleTypeDef hrtc; //Defined in main.c
static HAL_StatusTypeDef setTime (uint8_t hours, uint8_t minutes, uint8_t seconds);
static HAL_StatusTypeDef setDate (uint8_t hours, uint8_t minutes, uint8_t seconds);

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
