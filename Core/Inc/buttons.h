/*
 * buttons.h
 *
 *  Created on: 17 дек. 2021 г.
 *      Author: lug
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "stm32g0xx_hal.h"

#define DEBOUNCE_TIME_MS 50
#define RISING_BUTTON_PIN GPIO_PIN_8
#define FALLING_BUTTON_PIN GPIO_PIN_5

void buttonMonitor(void);

#endif /* INC_BUTTONS_H_ */
