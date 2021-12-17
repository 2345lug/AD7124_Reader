/*
 * rtcProcess.h
 *
 *  Created on: 15 дек. 2021 г.
 *      Author: lug
 */

#ifndef INC_RTCPROCESS_H_
#define INC_RTCPROCESS_H_

#define INPUT_TIME_STRING_SIZE  16
#define TIME_MASK (uint8_t*)"ii:ii_ii-ii-ii_i"

void rtcConsoleInput(void);
void printTimeUart(void);
void printTimeString(uint8_t* resultStringPointer);

#endif /* INC_RTCPROCESS_H_ */
