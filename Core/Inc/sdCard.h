/*
 * sdCard.h
 *
 *  Created on: 16 дек. 2021 г.
 *      Author: lug
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include "stm32g0xx_hal.h";

void sdCardInit (void);
void fileCreate(void);
void fileClose(void);
void writeStringToFile(uint8_t* inputString, uint8_t bytesToWrite);

#endif /* INC_SDCARD_H_ */
