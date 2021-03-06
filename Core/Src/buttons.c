#include <buttons.h>
#include <stdio.h>
#include <sdCard.h>

extern volatile uint8_t cycleStart;
extern uint8_t sdCardPresent;
volatile uint8_t pressDetected = 0;

volatile uint32_t previousTicks = 0;
static volatile uint32_t currentTicks = 0;

void buttonMonitor(void)
{
  if (pressDetected == 1)
  {
    if (cycleStart == 0)
    {
      printf ("Trigger detected. Start cycle \r\n");
      if (sdCardPresent != 0)
      {
        fileCreate();
      }
      cycleStart = 1;
    }
    else
    {
      printf ("Trigger detected. Stop cycle \r\n");
      cycleStart = 0;
      if (sdCardPresent != 0)
      {
        fileClose();
      }
    }
  }
  pressDetected = 0;
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t pinNumber)
{

  currentTicks = HAL_GetTick();
  uint32_t tickInterval = currentTicks - previousTicks;
  if (tickInterval > 100)
  {
	  pressDetected = 1;
	  previousTicks = HAL_GetTick();
  }

}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t pinNumber)
{

  currentTicks = HAL_GetTick();
  uint32_t tickInterval = currentTicks - previousTicks;
  if (tickInterval > 100)
  {
	  pressDetected = 1;
	  previousTicks = HAL_GetTick();
  }

}
