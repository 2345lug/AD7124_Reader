/*!
 *****************************************************************************
  @file:  platform_support.c

  @brief: support functions and definitions for STM32 in general, and
           targeting a configuration of L476 device in particular

  @details:
 -----------------------------------------------------------------------------
Copyright (c) 2019 Analog Devices, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors/
    products manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
	which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its contributors
    may be used to endorse or promote products derived from this software without
    specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
	requirement that you obtain separate licenses from these patent holders
	to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

20180927-7CBSD SLA

*****************************************************************************/

// Include Files
#include <math.h>
#include <string.h>

#include "platform_support.h"


/**
  * @brief  Retargets the C library __io_putchar function to the USART.
  * @param  None
  * @retval None
  */
int __io_putchar(int ch)
{
    /* Implementation of __io_putchar */
	/* e.g. write a character to the UART1 and Loop until the end of transmission */
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFFFFFF);

    return ch;
}

/**
  * @brief  Retargets the C library __io_getchar function to the USART.
  * @param  None
  * @retval character read uart
  */
int __io_getchar(void)
{
  /* Implementation of __io_getchar */
    char rxChar;

    // This loops in case of HAL timeout, but if an ok or error occurs, we continue
    while (HAL_UART_Receive(&huart2, (uint8_t *)&rxChar, 1, 0xFFFFFFFF) == HAL_TIMEOUT);

    return rxChar;
}


/**
  * @brief  getchar, but does not block if nothing waiting to be read
  * @param  None
  * @retval character if available, -1 otherwise
  */
int16_t getchar_nonblocking()
{
	uint8_t ch;

	if (HAL_UART_Receive(&huart2, (uint8_t *)&ch, 1, 0x0) == HAL_OK) {
	    return (uint16_t)ch;
	} else {
		return (-1); // Indicates no character read
	}
}


/**
  * @brief  toggles an LED to show something has happened
  * @param  None
  * @retval None
  */
void toggle_activity_led(void){
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
